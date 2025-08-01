#include <iostream>
#include <string>
#include <bitset>
#include <random>
#include <cstring>       // Para memset
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// Calcular CRC-32
uint32_t crc32(const std::string &data) {
    uint32_t crc = 0xFFFFFFFF;
    uint32_t poly = 0x04C11DB7;

    for (size_t idx = 0; idx < data.size(); ++idx) {
        char c = data[idx];
        for (int i = 7; i >= 0; --i) {
            bool bit = ((c >> i) & 1) ^ ((crc >> 31) & 1);
            crc <<= 1;
            if (bit) crc ^= poly;
        }
    }
    return crc ^ 0xFFFFFFFF;
}

// Convertir CRC a binario
std::string to_binary(uint32_t crc) {
    std::string result;
    for (int i = 31; i >= 0; --i) {
        result += ((crc >> i) & 1) ? '1' : '0';
    }
    return result;
}

// Convertir texto ASCII a binario
std::string ascii_to_bin(const std::string &text) {
    std::string bin;
    for (size_t i = 0; i < text.size(); ++i) {
        bin += std::bitset<8>(text[i]).to_string();
    }
    return bin;
}

// Aplicar ruido a la trama
std::string apply_noise(const std::string &data, double error_prob) {
    std::string noisy = data;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution d(error_prob);

    for (size_t i = 0; i < noisy.size(); ++i) {
        if (d(gen)) {
            noisy[i] = (noisy[i] == '0') ? '1' : '0';
        }
    }
    return noisy;
}

int main() {
    std::string input;
    double error_prob;

    std::cout << "Ingresa una trama en ASCII: ";
    std::cin >> input;

    std::cout << "Ingresa la probabilidad de error (ej. 0.01): ";
    std::cin >> error_prob;

    // Calcular CRC y generar trama
    uint32_t crc = crc32(input);
    std::string data_bin = ascii_to_bin(input);
    std::string crc_bin = to_binary(crc);
    std::string final_bin = data_bin + crc_bin;

    // Aplicar ruido
    std::string final_bin_with_noise = apply_noise(final_bin, error_prob);

    // Mostrar información
    std::cout << "\nTrama ASCII en binario: " << data_bin << "\n";
    std::cout << "CRC-32 en binario:      " << crc_bin << "\n";
    std::cout << "Trama + CRC final:      " << final_bin << "\n";
    std::cout << "Trama con ruido:        " << final_bin_with_noise << "\n";

    // Conectar con el receptor y enviar la trama
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "❌ Error al crear el socket\n";
        return 1;
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5050);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);

    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "❌ Error al conectar con el receptor\n";
        close(sock);
        return 1;
    }

    // Enviar la trama con ruido
    ssize_t sent = send(sock, final_bin_with_noise.c_str(), final_bin_with_noise.size(), 0);
    if (sent < 0) {
        std::cerr << "❌ Error al enviar la trama\n";
        close(sock);
        return 1;
    }

    std::cout << "📤 Trama enviada al receptor\n";

    close(sock);
    return 0;
}
