#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#define MAX_EDGE 102

using namespace std;

class Nodes {
public:
    explicit Nodes(int n, bool isDirectional = false);
    int search_TTL(int num1, int num2);

private:
    bool node_matrix[MAX_EDGE][MAX_EDGE]{};
    bool isDirectional = false;
    unordered_map<int, int> translate_to_fake;
    vector<int> translate_to_real;
};

Nodes::Nodes(int n, bool isDirectional) {
    this->isDirectional = isDirectional;
    fill(&node_matrix[0][0], &node_matrix[0][0] + sizeof(node_matrix) / sizeof(bool), false);

    for (int i = 0; i < n; ++i) {
        int x, y;
        cin >> x >> y;

        if(translate_to_fake.find(x) == translate_to_fake.end()){
            translate_to_real.push_back(x);
            translate_to_fake[x] = translate_to_real.size() - 1;
        }
        if(translate_to_fake.find(y) == translate_to_fake.end()){
            translate_to_real.push_back(y);
            translate_to_fake[y] = translate_to_real.size() - 1;
        }

        node_matrix[translate_to_fake[x]][translate_to_fake[y]] = true;
        if(!isDirectional){
            node_matrix[translate_to_fake[y]][translate_to_fake[x]] = true;
        }
    }
}

int Nodes::search_TTL(int num1, int num2){
    if(translate_to_fake.find(num1) == translate_to_fake.end()){
        return translate_to_fake.size();
    }

    vector<pair<int, int>> queue;
    queue.emplace_back(num1, 0);
    unordered_set<int> seen;
    seen.insert(num1);

    while (!queue.empty()) {
        auto [current_node, current_ttl] = queue.front();
        queue.erase(queue.begin());

        if (current_ttl < num2) {
            int ind_start_node = translate_to_fake[current_node];
            for (int j = 0; j < translate_to_real.size(); ++j) {
                if (node_matrix[ind_start_node][j] && seen.find(translate_to_real[j]) == seen.end()) {
                    queue.emplace_back(translate_to_real[j], current_ttl + 1);
                    seen.insert(translate_to_real[j]);
                }
            }
        }
    }

    return translate_to_real.size() - seen.size();
}

int main() {
    int n, count = 1;
    while(cin >> n && n) {
        Nodes node(n);

        int x, y;
        while(cin >> x >> y && (x != 0 || y != 0)) {
            cout << "Case " << count++ << ": " << node.search_TTL(x, y) << " nodes not reachable from node " << x << " with TTL = " << y << "." << endl;
        }
    }

    return 0;
}

