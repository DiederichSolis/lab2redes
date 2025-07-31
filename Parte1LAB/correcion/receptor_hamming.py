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
    bits.insert(0, None)  # índice 1-based

    error_pos = 0

    # Verificación de paridad
    for i in range(r):
        pos = 2 ** i
        paridad = 0
        for j in range(1, n + 1):
            if j & pos:
                paridad ^= bits[j]
        if paridad != 0:
            error_pos += pos

    if error_pos == 0:
        print("✅ No se detectaron errores. Trama recibida: ", ''.join(str(b) for b in bits[1:]))
    elif error_pos <= n:
        print(f"⚠️ Error detectado en la posición: {error_pos}")
        bits[error_pos] ^= 1  # corregir bit
        print("✅ Trama corregida: ", ''.join(str(b) for b in bits[1:]))
    else:
        print("❌ Error detectado pero la posición está fuera del rango. Trama inválida.")

if __name__ == "__main__":
    trama = input("Ingrese la trama codificada (ej. 0110011): ")
    
    if not all(c in '01' for c in trama):
        print("❌ Solo se permiten caracteres binarios (0 o 1).")
    else:
        detectar_y_corregir(trama)
