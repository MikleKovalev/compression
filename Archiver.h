#ifndef COMPRESSION_ARCHIVER_H
#define COMPRESSION_ARCHIVER_H

#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include "CodeTable.h"

using namespace std;

class Archiver {
private:
    ifstream input;
    ofstream output;

    void read_data(vector<char>& data) {
        string line;
        vector<string> lines;
        while (!input.eof()) {
            getline(input, line);
            for (const char& character : line) {
                data.push_back(character);
            }
            data.push_back('\n');
        }
        data.pop_back();
    }

public:
    explicit Archiver(const string& data_file_name, const string& archive_file_name) {
        input.open(data_file_name, ios::in);
        output.open(archive_file_name, ios::out | ios::trunc | ios::binary);
    }

    void archive() {
        vector<char> characters;
        read_data(characters);
        CodeTable codes(characters);
        string binary;
        for (const char& character : characters) {
            binary += codes[character];
        }
        vector<char> compressed_binary;
        int buffer = 0;
        int bit_number = 7;
        for (const char& bit : binary) {
            if (bit == '1') {
                buffer = buffer | (1 << bit_number);
            }
            --bit_number;
            if (bit_number == -1) {
                compressed_binary.push_back((char) buffer);
                bit_number = 7;
                buffer = 0;
            }
        }
        if (bit_number != 7) {
            compressed_binary.push_back((char) buffer);
        }
        output << codes;
        for (const char& data : compressed_binary) {
            output << data;
        }
    }

    ~Archiver() {
        input.close();
        output.close();
    }
};


#endif //COMPRESSION_ARCHIVER_H
