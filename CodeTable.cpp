#include <ostream>
#include <algorithm>
#include "CodeTable.h"

using namespace std;

void CodeTable::build_codes(Node *node, const string& code) {
    if (node->leaf) {
        codes[node->character] = code;
    } else {
        build_codes(node->left, code + '0');
        build_codes(node->right, code + '1');
    }
}

void CodeTable::destroy_node(Node *node) {
    if (node != nullptr) {
        destroy_node(node->left);
        destroy_node(node->right);
        delete node;
    }
}

CodeTable::CodeTable(const vector<char>& data) {
    map<char, int> count;
    for (const char& character : data) {
        ++count[character];
    }
    if (count.size() == 1) {
        codes[(*count.begin()).first] = "0";
        return ;
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

string& CodeTable::operator[](char character) {
    return codes[character];
}

ostream& operator<<(ostream& out, const CodeTable& codes_table) {
    out << codes_table.codes.size() << endl;
    for (const auto& record : codes_table.codes) {
        out << record.first << record.second << endl;
    }
    return out;
}