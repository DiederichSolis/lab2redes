# 🛰️ LABORATORIO 2 REDES – Detección y corrección de errores
Jorge Luis Lopez 
Diederich Solis

Este repositorio contiene la implementación de algoritmos para detección y corrección de errores usando CRC-32 y código de Hamming.

- `deteccion`: Implementación de CRC-32 para detectar errores en transmisiones.
- `correccion`: Implementación del código Hamming para corregir errores simples.

---

## 📁 Estructura del proyecto

LAB2REDES/  
├── correccion/  
│   ├── emisor_hamming.cpp  
│   └── receptor_hamming.py  
├── deteccion/  
│   ├── emisor_crc.cpp  
│   └── receptor_crc.py  
├── documento/  
│   └── lab2.pdf  
└── README.md  

---

## 🚀 Instrucciones de uso

### 🔧 Requisitos
Python 3.x y Compilador g++ (GCC)

### 🔍 Detección de errores (CRC-32)

1. Compilar el emisor CRC:

```bash
cd deteccion
g++ emisor_crc.cpp -o emisor_crc
```

2. Ejecutar el emisor:

```bash
./emisor_crc
```

3. Ejecutar el receptor:

```bash
python3 receptor_crc.py
```

Ingresa la trama generada para verificar su integridad.

---

### 🛠️ Corrección de errores (Hamming)

1. Compilar el emisor Hamming:

```bash
cd correccion
g++ emisor_hamming.cpp -o emisor_hamming
```

2. Ejecutar el emisor:

```bash
./emisor_hamming
```

3. Ejecutar el receptor:

```bash
python3 receptor_hamming.py
```

Ingresa la trama y se corregirá automáticamente si hay un error simple.

---
## 📄 Informe
Consulta el archivo documento/lab2.pdf para ver resultados, capturas, explicaciones y conclusiones.

## ✅ Conclusiones finales

La práctica permitió implementar desde cero algoritmos de verificación y corrección de errores. CRC-32 mostró su efectividad detectando errores de transmisión, mientras que el código Hamming demostró su utilidad al identificar y corregir errores simples. El laboratorio refuerza los conceptos fundamentales de confiabilidad en la capa de enlace de datos.


