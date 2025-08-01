#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <bitset>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

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

    for (int i = 1; i <= n; i++) {
        hamming[i] = esPotenciaDe2(i) ? '0' : data[j++];
    }

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
            bit = (bit == '0') ? '1' : '0';
        }
    }
    return ruidosa;
}

//////////////////////
// CAPA TRANSMISIÓN - SOCKET CLIENTE
//////////////////////
void enviarTramaPorSocket(const string& trama, const string& ip = "127.0.0.1", int puerto = 5000) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "❌ Error al crear el socket\n";
        return;
    }

    sockaddr_in servidor{};
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(puerto);
    inet_pton(AF_INET, ip.c_str(), &servidor.sin_addr);

    cout << "\n🔌 Conectando al receptor en " << ip << ":" << puerto << "...\n";
    if (connect(sock, (struct sockaddr*)&servidor, sizeof(servidor)) < 0) {
        cerr << "❌ Error al conectar con el receptor\n";
        close(sock);
        return;
    }

    send(sock, trama.c_str(), trama.size(), 0);
    cout << "✅ Trama enviada al receptor con éxito.\n";

    close(sock);
}

//////////////////////
// MAIN
//////////////////////
int main(int argc, char* argv[]) {
    int puerto = 5000;
    if (argc > 1) {
        puerto = atoi(argv[1]);  // lee el puerto desde la línea de comandos
    }

    // APLICACIÓN
    string mensaje = capaAplicacion_Emisor();

    // PRESENTACIÓN
    string binario = textoABinarioASCII(mensaje);
    cout << "\n[Presentación] Binario ASCII: " << binario << endl;

    // ENLACE
    string hamming = aplicarHamming(binario);
    cout << "[Enlace] Trama con Hamming: " << hamming << endl;

    // RUIDO
    double probabilidadError = 0.01;
    string ruidosa = aplicarRuido(hamming, probabilidadError);
    cout << "[Ruido] Trama ruidosa (p=" << probabilidadError << "): " << ruidosa << endl;

    // TRANSMISIÓN
    enviarTramaPorSocket(ruidosa, "127.0.0.1", puerto);

    return 0;
}
