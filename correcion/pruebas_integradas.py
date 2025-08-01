import subprocess
import time
import random
import string
import matplotlib.pyplot as plt
import os
import signal


def generar_mensaje_texto(n=4):
    return ''.join(random.choices(string.ascii_uppercase + string.digits, k=n))

def correr_receptor():
    return subprocess.Popen(['python3', 'receptor_hamming.py'],
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE,
                            text=True)

def analizar_salida_receptor(salida):
    if "resultado: correcto" in salida:
        return "correcto"
    elif "resultado: corregido" in salida:
        return "corregido"
    elif "resultado: fallido" in salida:
        return "fallido"
    else:
        return "desconocido"


def ejecutar_pruebas(n=100, longitud=4, puerto_base=5000):
    resultados = {"correcto": 0, "corregido": 0, "fallido": 0, "desconocido": 0}

    for i in range(n):
        mensaje = generar_mensaje_texto(longitud)
        puerto = puerto_base + i  # puerto único para esta prueba
        print(f"\n📤 Prueba {i+1}/{n} — mensaje: {mensaje} (puerto {puerto})")

        receptor = subprocess.Popen(['python3', 'receptor_hamming.py', str(puerto)],
                                    stdout=subprocess.PIPE,
                                    stderr=subprocess.PIPE,
                                    text=True)
        time.sleep(1.0)

        out_emisor, err_emisor = subprocess.Popen(
            ['./emisor', str(puerto)], stdin=subprocess.PIPE,
            stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True
        ).communicate(input=mensaje + "\n")

        print(f"[DEBUG EMISOR STDOUT]:\n{out_emisor}")
        print(f"[DEBUG EMISOR STDERR]:\n{err_emisor}")

        try:
            salida, error = receptor.communicate(timeout=3)
            print(f"[DEBUG RECEPTOR STDOUT]:\n{salida}")
            print(f"[DEBUG RECEPTOR STDERR]:\n{error}")
            estado = analizar_salida_receptor(salida)
            resultados[estado] += 1
        except Exception as e:
            print("⚠️ Error leyendo del receptor:", e)
            receptor.terminate()

    return resultados


def graficar(resultados):
    etiquetas = list(resultados.keys())
    valores = [resultados[k] for k in etiquetas]

    plt.bar(etiquetas, valores, color=['green', 'orange', 'red', 'gray'])
    plt.title("Resultados de pruebas integradas (emisor + receptor)")
    plt.xlabel("Resultado")
    plt.ylabel("Cantidad")
    plt.grid(axis='y')
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    total_pruebas = 100  # puedes cambiar a 10000 para entrega final
    resultados = ejecutar_pruebas(n=total_pruebas, longitud=4)

    print("\n📊 Resumen:")
    for k, v in resultados.items():
        print(f"  {k.upper()}: {v} ({v/total_pruebas:.2%})")

    graficar(resultados)
