#ifndef COMPRESSION_CODETABLE_H
#define COMPRESSION_CODETABLE_H

#include <map>
#include <vector>
#include <string>

using namespace std;

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

    void build_codes(Node *node, const string& code);

    void destroy_node(Node *node);

public:
    explicit CodeTable(const vector<char>& data);

    string& operator[](char character);

    friend ostream& operator<<(ostream& out, const CodeTable& codes);
};


#endif //COMPRESSION_CODETABLE_H
