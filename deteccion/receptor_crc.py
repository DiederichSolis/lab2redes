def custom_crc32(data):
    crc = 0xFFFFFFFF
    poly = 0x04C11DB7

    for c in data:
        byte = ord(c)
        for i in range(7, -1, -1):
            bit = ((byte >> i) & 1) ^ ((crc >> 31) & 1)
            crc <<= 1
            if bit:
                crc ^= poly
            crc &= 0xFFFFFFFF  # Asegura 32 bits

    return crc ^ 0xFFFFFFFF

def bin_to_ascii(binary):
    text = ''
    for i in range(0, len(binary), 8):
        byte = binary[i:i+8]
        if len(byte) == 8:
            text += chr(int(byte, 2))
    return text

def verify_crc(data_ascii, crc_received_bin):
    crc_expected = custom_crc32(data_ascii)
    crc_received = int(crc_received_bin, 2)

    print("\n🧾 Texto reconstruido:", repr(data_ascii))
    print("🔧 CRC esperado : ", format(crc_expected, '032b'))
    print("📩 CRC recibido : ", crc_received_bin)

    if crc_expected == crc_received:
        print("✅ No se detectaron errores.")
    else:
        print("❌ Error detectado. Trama descartada.")

if __name__ == "__main__":
    trama = input("Ingrese la trama completa (binario de datos + CRC): ").strip()

    if len(trama) < 40 or len(trama) % 8 != 0:
        print("⚠️ Trama inválida. Verifique longitud.")
    else:
        data_bin = trama[:-32]
        crc_bin = trama[-32:]
        data_ascii = bin_to_ascii(data_bin)
        verify_crc(data_ascii, crc_bin)
