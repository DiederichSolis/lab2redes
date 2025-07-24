#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// Verifica si un número es potencia de 2 (posiciones de paridad)
bool esPotenciaDe2(int x) {
    return x && !(x & (x - 1));
}

// Calcula la cantidad de bits de paridad necesarios para m bits de datos
int calcularR(int m) {
    int r = 0;
    while (pow(2, r) < (m + r + 1)) {
        r++;
    }
    return r;
}

// Genera la trama codificada usando Hamming
string generarTramaHamming(const string& data) {
    int m = data.length();
    int r = calcularR(m);
    int n = m + r;

    vector<char> hamming(n + 1); // índice 1-based
    int j = 0; // índice para data

    // Insertar bits de datos y reservar posiciones para paridad
    for (int i = 1; i <= n; i++) {
        if (esPotenciaDe2(i)) {
            hamming[i] = '0'; // marcador temporal para bits de paridad
        } else {
            hamming[i] = data[j++];
        }
    }

    // Calcular bits de paridad
    for (int i = 0; i < r; i++) {
        int pos = pow(2, i);
        int paridad = 0;
        for (int j = 1; j <= n; j++) {
            if (j & pos) {
                paridad ^= (hamming[j] - '0');
            }
        }
        hamming[pos] = paridad + '0';
    }

    // Construir la trama codificada final
    string tramaCodificada;
    for (int i = 1; i <= n; i++) {
        tramaCodificada += hamming[i];
    }

    return tramaCodificada;
}

int main() {
    string data;

    cout << "Ingrese la trama en binario (ej. 1011): ";
    cin >> data;

    // Validación básica
    if (!all_of(data.begin(), data.end(), [](char c){ return c == '0' || c == '1'; })) {
        cout << "Error: solo se permiten bits binarios (0 o 1)." << endl;
        return 1;
    }

    string codificada = generarTramaHamming(data);

    cout << "Trama codificada con Hamming: " << codificada << endl;

    return 0;
}
