#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <bitset>

using namespace std;

//////////////////////
// CAPA APLICACIÓN
//////////////////////
string capaAplicacion_Emisor() {
    string mensaje;
    cout << "Ingrese el mensaje a enviar: ";
    getline(cin, mensaje);
    return mensaje;
}

//////////////////////
// CAPA PRESENTACIÓN
//////////////////////
string textoABinarioASCII(const string& texto) {
    string binario;
    for (char c : texto) {
        binario += bitset<8>(c).to_string(); // convierte cada char a 8 bits ASCII
    }
    return binario;
}

//////////////////////
// CAPA ENLACE (Hamming)
//////////////////////

bool esPotenciaDe2(int x) {
    return x && !(x & (x - 1));
}

int calcularR(int m) {
    int r = 0;
    while (pow(2, r) < (m + r + 1)) {
        r++;
    }
    return r;
}

string aplicarHamming(const string& data) {
    int m = data.length();
    int r = calcularR(m);
    int n = m + r;

    vector<char> hamming(n + 1); // índice 1-based
    int j = 0;

    // Insertar bits de datos y reservar posiciones de paridad
    for (int i = 1; i <= n; i++) {
        hamming[i] = esPotenciaDe2(i) ? '0' : data[j++];
    }

    // Calcular paridad
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

    string codificada;
    for (int i = 1; i <= n; i++) {
        codificada += hamming[i];
    }
    return codificada;
}

//////////////////////
// CAPA RUIDO
//////////////////////

string aplicarRuido(const string& trama, double probabilidadError) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    string ruidosa = trama;
    for (char& bit : ruidosa) {
        if (dis(gen) < probabilidadError) {
            bit = (bit == '0') ? '1' : '0'; // voltear bit
        }
    }
    return ruidosa;
}

//////////////////////
// MAIN - Simulación emisor con capas
//////////////////////

int main() {
    // CAPA APLICACIÓN
    string mensaje = capaAplicacion_Emisor();

    // CAPA PRESENTACIÓN
    string binario = textoABinarioASCII(mensaje);
    cout << "\n[Presentación] Binario ASCII: " << binario << endl;

    // CAPA ENLACE
    string hamming = aplicarHamming(binario);
    cout << "[Enlace] Trama con Hamming: " << hamming << endl;

    // CAPA RUIDO
    double probabilidadError = 0.01; // 1% de error
    string ruidosa = aplicarRuido(hamming, probabilidadError);
    cout << "[Ruido] Trama con ruido (p=" << probabilidadError << "): " << ruidosa << endl;

    return 0;
}
