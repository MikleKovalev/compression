//
// Created by User on 07.11.2020.
//

#ifndef COMPRESSION_UNARCHIVER_H
#define COMPRESSION_UNARCHIVER_H

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class Unarchiver {
private:
    ifstream input;
    ofstream output;

    inline static int string_to_int(const string& value) {
        int result = 0;
        for (char i : value) {
            result = result * 10 + (i - '0');
        }
        return result;
    }

public:
    explicit Unarchiver(const string& archive_file_name, const string& unarchive_file_name) {
        input.open(archive_file_name, ios::in | ios::binary);
        output.open(unarchive_file_name, ios::out | ios::trunc);
    }

    void unarchive() {
        string line;
        getline(input, line);
        int codes_table_size = string_to_int(line);
        map<string, char> table;
        for (int i = 0; i < codes_table_size; ++i) {
            getline(input, line);
            if (line.empty()) {
                getline(input, line);
                table[line] = '\n';
            } else {
                table[line.substr(1)] = line[0];
            }
        }
        string data;
        while (!input.eof()) {
            getline(input, line);
            data += line;
            data += '\n';
        }
        string binary;
        for (const char& d : data) {
            for (int i = 7; i >= 0; --i) {
                bool bit = d & (1 << i);
                binary += char(bit + '0');
            }
        }
        string buffer;
        string unarchive;
        for (const char& bit : binary) {
            buffer += bit;
            if (table.count(buffer)) {
                unarchive += table[buffer];
                buffer = "";
            }
        }
        output << unarchive;
    }

    ~Unarchiver() {
        input.close();
        output.close();
    }
};

#endif //COMPRESSION_UNARCHIVER_H
