#include <iostream>
#include <string>
#include <bitset>

uint32_t crc32(const std::string &data) {
    uint32_t crc = 0xFFFFFFFF;
    uint32_t poly = 0x04C11DB7;

    for (char c : data) {
        for (int i = 7; i >= 0; --i) {
            bool bit = ((c >> i) & 1) ^ ((crc >> 31) & 1);
            crc <<= 1;
            if (bit) crc ^= poly;
        }
    }

    return crc ^ 0xFFFFFFFF;
}

std::string to_binary(uint32_t crc) {
    std::string result;
    for (int i = 31; i >= 0; --i) {
        result += ((crc >> i) & 1) ? '1' : '0';
    }
    return result;
}

std::string ascii_to_bin(const std::string &text) {
    std::string bin = "";
    for (char c : text) {
        bin += std::bitset<8>(c).to_string();
    }
    return bin;
}

int main() {
    std::string input;
    std::cout << "Ingresa una trama en ASCII: ";
    std::cin >> input;

    uint32_t crc = crc32(input);
    std::string data_bin = ascii_to_bin(input);
    std::string crc_bin = to_binary(crc);
    std::string final_bin = data_bin + crc_bin;

    std::cout << "Trama ASCII en binario: " << data_bin << "\n";
    std::cout << "CRC-32 en binario:      " << crc_bin << "\n";
    std::cout << "Trama + CRC final:      " << final_bin << "\n";

    return 0;
}
