import socket
import sys

def es_potencia_de_2(x):
    return x and not (x & (x - 1))

def calcular_r(n):
    r = 0
    while (2 ** r) < (n + 1):
        r += 1
    return r

def detectar_y_corregir(trama_codificada):
    n = len(trama_codificada)
    r = calcular_r(n)
    bits = [int(b) for b in trama_codificada]
    bits.insert(0, None)

    error_pos = 0
    for i in range(r):
        pos = 2 ** i
        paridad = 0
        for j in range(1, n + 1):
            if j & pos:
                paridad ^= bits[j]
        if paridad != 0:
            error_pos += pos

    corregida = bits[1:]
    if error_pos == 0:
        return "correcto", ''.join(str(b) for b in corregida)
    elif error_pos <= n:
        bits[error_pos] ^= 1
        return "corregido", ''.join(str(b) for b in bits[1:])
    else:
        return "fallido", ''.join(str(b) for b in bits[1:])

def binario_a_texto(binario):
    chars = []
    for i in range(0, len(binario), 8):
        byte = binario[i:i+8]
        if len(byte) == 8:
            chars.append(chr(int(byte, 2)))
    return ''.join(chars)

def recibir_trama(puerto):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(('', puerto))
    s.listen(1)
    conn, addr = s.accept()
    datos = conn.recv(4096).decode()
    conn.close()
    return datos

if __name__ == "__main__":
    puerto = int(sys.argv[1]) if len(sys.argv) > 1 else 5000
    trama = recibir_trama(puerto)
    if not all(c in '01' for c in trama):
        print("resultado: fallido")
    else:
        estado, corregida = detectar_y_corregir(trama)
        mensaje = binario_a_texto(corregida)
        print(f"resultado: {estado}")
        print(f"mensaje: {mensaje}")
