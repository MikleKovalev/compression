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

class CodeTable {
private:
    map<char, string> codes;

    void build(vector<Node> nodes, const string& code = "") {
        if (nodes.empty()) {
            return ;
        }
        if (nodes.size() == 1) {
            codes[nodes[0].character] = code;
        }
        sort(nodes.rbegin(), nodes.rend(), [&](Node& first, Node& second) {
            return first.probability < second.probability;
        });
        double prob_sum = 0.0;
        for (const Node& node : nodes) {
            prob_sum += node.probability;
        }
        double half_prob_sum = 0.0;
        for (int i = 0; i < int(nodes.size()); ++i) {
            half_prob_sum += nodes[i].probability;
            if (half_prob_sum >= prob_sum / 2) {
                build(vector<Node>(
                        nodes.begin(),
                        nodes.begin() + i + 1),
                      code + '1');
                build(vector<Node>(
                        nodes.begin() + i,
                        nodes.end()),
                      code + '0');
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
        int node_id = 0;
        for (const auto& record : count) {
            nodes[node_id] = Node {
                .character = record.first,
                .probability = (double) record.second / characters.size(),
            };
            ++node_id;
        }
        build(nodes);
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
