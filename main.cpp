#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <fstream>

using namespace std;

string archive_binary;

string int_to_string(int value) {
    string result;
    while (value > 0) {
        result += (value % 10) + '0';
        value /= 10;
    }
    reverse(result.begin(), result.end());
    return result;
}

int string_to_int(const string& value) {
    int result = 0;
    for (const char& digit : value) {
        result = result * 10 + (digit - '0');
    }
    return result;
}

struct Node {
    bool leaf;
    char character;
    double probability;
    Node *left;
    Node *right;
};

class CodeTable {
private:
    map<char, string> codes;

    void build_codes(Node *node, const string& code) {
        if (node->leaf) {
            codes[node->character] = code;
        } else {
            build_codes(node->left, code + '0');
            build_codes(node->right, code + '1');
        }
    }

    void destroy_node(Node *node) {
        if (node != nullptr) {
            destroy_node(node->left);
            destroy_node(node->right);
            delete node;
        }
    }

public:
    explicit CodeTable(const vector<char>& data) {
        map<char, int> count;
        for (const char& character : data) {
            ++count[character];
        }
        vector<Node*> nodes;
        for (const auto& record : count) {
            Node *node = new Node;
            node->leaf = true;
            node->character = record.first;
            node->probability = (double) record.second / data.size();
            node->left = nullptr;
            node->right = nullptr;
            nodes.push_back(node);
        }
        while (nodes.size() > 1) {
            sort(nodes.begin(), nodes.end(), [](Node* first, Node* second) {
                return first->probability < second->probability;
            });
            cout << nodes.size() << endl;
            for (Node* node : nodes) {
                cout << node->probability << " ";
            }
            cout << endl;
            Node *parent = new Node;
            parent->leaf = false;
            parent->character = -1;
            parent->probability = nodes[0]->probability + nodes[1]->probability;
            parent->left = nodes[0];
            parent->right = nodes[1];
            nodes.push_back(parent);
            nodes.erase(nodes.begin());
            nodes.erase(nodes.begin());
        }
        if (!nodes.empty()) {
            build_codes(nodes[0], "");
            destroy_node(nodes[0]);
        }
    }

    string& operator[](char character) {
        return codes[character];
    }

    friend ostream& operator<<(ostream& out, const CodeTable& codes);
};

ostream& operator<<(ostream& out, const CodeTable& codes_table) {
    out << codes_table.codes.size() << endl;
    for (const auto& record : codes_table.codes) {
        out << record.first << record.second << endl;
    }
    return out;
}

void solve(int number) {
    string archive_file_name = "archive_" + int_to_string(number);
    ifstream input(archive_file_name, ios::in);
    vector<string> lines;
    string line;
    while (!input.eof()) {
        getline(input, line);
        lines.push_back(line);
    }
    int codes_table_size = string_to_int(lines[0]);
    map<string, char> codes;
    for (int i = 1; i < codes_table_size + 1; ++i) {
        codes[lines[i].substr(1)] = lines[i][0];
    }
    string binary;
    for (int i = codes_table_size + 1; i < int(lines.size()); ++i) {
        for (const char& byte : lines[i]) {
            for (int j = 7; j >= 0; --j) {
                bool bit = byte & (1 << j);
                binary += (char) (bit + '0');
            }
        }
    }
    cout << (binary == archive_binary) << endl;
    string buffer;
    string result;
    for (const char& bit : binary) {
        buffer += bit;
        if (codes.count(buffer)) {
            result += codes[buffer];
            buffer = "";
        }
    }
    string unzip_file_name = "unzip_" + int_to_string(number) + ".txt";
    ofstream output(unzip_file_name, ios::out | ios::trunc | ios::binary);
    output << result;
}

int main() {
    for (int i = 1; i < 11; ++i) {
        cout << "Test #" << i << endl;
        string file_name = "Тест_" + int_to_string(i) + ".txt";
        ifstream input(file_name, ios::in);
        vector<char> data;
        string line;
        while (!input.eof()) {
            getline(input, line);
            for (const char& c : line) {
                data.push_back(c);
            }
        }
        CodeTable codes(data);
        string archive_file_name = "archive_" + int_to_string(i);
        ofstream output(archive_file_name, ios::out | ios::trunc | ios::binary);
        output << codes;
        string binary;
        for (const char& character : data) {
            binary += codes[character];
        }
        archive_binary = binary;
        char buffer;
        int bit_number = 7;
        for (const char& bit : binary) {
            if (bit == '1') {
                buffer = (char) (buffer | (1 << bit_number));
            }
            --bit_number;
            if (bit_number == -1) {
                output << buffer;
                buffer = 0;
                bit_number = 7;
            }
        }
        if (bit_number != 7) {
            output << buffer;
        }
        output.close();
        solve(i);
    }
    return 0;
}
