import subprocess
import socket
import time
import matplotlib.pyplot as plt

MENSAJE = "HELLO"
ITERACIONES = 1000
PROBABILIDADES = [0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.1]

def ejecutar_receptor():
    host = "127.0.0.1"
    port = 5050
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind((host, port))
    server.listen(1)

    conn, _ = server.accept()
    trama = conn.recv(8192).decode().strip()
    conn.close()
    server.close()
    return trama

def calcular_crc(data):
    crc = 0xFFFFFFFF
    poly = 0x04C11DB7
    for c in data:
        byte = ord(c)
        for i in range(7, -1, -1):
            bit = ((byte >> i) & 1) ^ ((crc >> 31) & 1)
            crc = (crc << 1) & 0xFFFFFFFF
            if bit:
                crc ^= poly
    return crc ^ 0xFFFFFFFF

def bin_to_ascii(binary):
    text = ""
    for i in range(0, len(binary) - 32, 8):
        text += chr(int(binary[i:i+8], 2))
    return text

def verificar_trama(trama):
    if len(trama) < 40:
        return False
    data_bin = trama[:-32]
    crc_bin = trama[-32:]
    data_ascii = bin_to_ascii(data_bin)
    crc_calc = calcular_crc(data_ascii)
    return format(crc_calc, "032b") == crc_bin

resultados = {}

for prob in PROBABILIDADES:
    print(f"\n🔍 Probando con probabilidad de error {prob}")
    errores_detectados = 0

    for _ in range(ITERACIONES):
        # Inicia el receptor
        receptor_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        receptor_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        receptor_socket.bind(("127.0.0.1", 5050))
        receptor_socket.listen(1)

        # Lanza el emisor con inputs automáticos
        subprocess.Popen(
            ["./emisor_crc"],
            stdin=subprocess.PIPE,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
            text=True
        ).communicate(f"{MENSAJE}\n{prob}\n")

        conn, _ = receptor_socket.accept()
        trama = conn.recv(8192).decode().strip()
        conn.close()
        receptor_socket.close()

        if not verificar_trama(trama):
            errores_detectados += 1

    tasa_error = (errores_detectados / ITERACIONES) * 100
    resultados[prob] = tasa_error
    print(f"➡ Errores detectados: {errores_detectados}/{ITERACIONES} ({tasa_error:.2f}%)")

# Graficar resultados
plt.figure(figsize=(8, 5))
plt.plot(list(resultados.keys()), list(resultados.values()), marker="o")
plt.title("Detección de errores con CRC")
plt.xlabel("Probabilidad de error")
plt.ylabel("Errores detectados (%)")
plt.grid()
plt.show()
