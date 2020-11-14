#ifndef COMPRESSION_CODETABLE_H
#define COMPRESSION_CODETABLE_H

#include <map>
#include <vector>
#include <string>
#include <numeric>

using namespace std;

struct Node {
    char character;
    double probability;
};

// Fano code table
class CodeTable {
private:
    map<char, string> codes;

    void build(vector<Node>::iterator begin, vector<Node>::iterator end, const string& code = "") {
        if (begin == end) {
            return;
        }
        if (end - begin == 1) {
            if (code.empty()) {
                codes[begin->character] = "0";
            } else {
                codes[begin->character] = code;
            }
            return;
        }
        double halfProbability = 0.0;
        for (auto it = begin; it != end; ++it) {
            halfProbability += it->probability;
        }
        halfProbability /= 2;
        double probability = 0.0;
        for (auto it = begin; it != end; ++it) {
            probability += it->probability;
            if (probability >= halfProbability) {
                build(begin, it + 1, code + '0');
                build(it + 1, end, code + '1');
                return;
            }
        }
    }

public:
    explicit CodeTable(const vector<char>& characters) {
        map<char, int> count;
        for (const char& character : characters) {
            ++count[character];
        }
        vector<Node> nodes(count.size());
        int nodeId = 0;
        for (const auto& record : count) {
            nodes[nodeId] = Node {
                    .character = record.first,
                    .probability = (double) record.second / characters.size(),
            };
            ++nodeId;
        }
        sort(nodes.rbegin(), nodes.rend(), [&](const Node& first, const Node& second) {
            return first.probability < second.probability;
        });
        build(nodes.begin(), nodes.end());
    }

    string& operator[](char character) {
        return codes[character];
    }

    friend ostream& operator<<(ostream& out, const CodeTable& codes);
};

ostream& operator<<(ostream& out, const CodeTable& codes) {
    out << codes.codes.size() << endl;
    for (const auto& record : codes.codes) {
        out << record.first << record.second << endl;
    }
    return out;
}


#endif //COMPRESSION_CODETABLE_H
