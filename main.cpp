#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>

#define MAX_EDGE 51

using namespace std;

class Nodes {
public:
    explicit Nodes(int n, bool isDirectional = false);
    void print_matrix();
    int search_TTL(long long int num1, int num2);

private:
    bool node_matrix[MAX_EDGE][MAX_EDGE]{};
    bool isDirectional = false;
    int node_num = 0;
    int n;
    unordered_map<int, int> translate_to_fake;
    vector<int> translate_to_real;
    vector<pair<int, int>> pairs;
};

Nodes::Nodes(int n, bool isDirectional) {
    this->n = n;
    this->isDirectional = isDirectional;
    fill(&node_matrix[0][0], &node_matrix[0][0] + sizeof(node_matrix) / sizeof(bool), false);

    for (int i = 0; i < n; ++i) {
        long long int x, y;
        cin >> x >> y;
        pairs.emplace_back(x, y);

        if(translate_to_fake.find(x) == translate_to_fake.end()){
            translate_to_real.emplace_back(x);
            translate_to_fake[x] = translate_to_real.size()-1;
        }
        if(translate_to_fake.find(y) == translate_to_fake.end()){
            translate_to_real.emplace_back(y);
            translate_to_fake[y] = translate_to_real.size()-1;
        }

        if(isDirectional){
            node_matrix[translate_to_fake[x]][translate_to_fake[y]] = true;
        }
        else{
            node_matrix[translate_to_fake[x]][translate_to_fake[y]] = true;
            node_matrix[translate_to_fake[y]][translate_to_fake[x]] = true;
        }
    }
}

void Nodes::print_matrix() {
    cout << "  ";
    for(int i = 0; i < translate_to_real.size(); i++){
        cout << translate_to_real[i] << " ";
    }
    cout << endl;
    for(int i = 0; i < translate_to_real.size(); i++){
        cout << translate_to_real[i] << " ";
        for(int j = 0; j < translate_to_real.size(); j++){
            bool first_bool = node_matrix[translate_to_fake[translate_to_real[i]]][translate_to_fake[translate_to_real[j]]] || node_matrix[translate_to_fake[translate_to_real[j]]][translate_to_fake[translate_to_real[i]]];
            cout << first_bool << " ";
        }
        cout << endl;
    }
}

int Nodes::search_TTL(long long int num1, int num2){
    if(find(translate_to_real.begin(), translate_to_real.end(), num1) == translate_to_real.end()){
        return translate_to_fake.size();
    }
    if (num2 == 0) {
        return (translate_to_real.size() - 1);
    }

    vector<pair<long long int, int>> queue;
    queue.emplace_back(num1, 0);
    int ttl = num2;
    long long int seen[MAX_EDGE] = {queue.front().first};
    int seen_i = 1;

    while (queue.size() != 0) {
        long long int start_node = queue.front().first;
        if (queue.front().second < ttl) {
            for (int j = 0; j < n * 2; j++) {
                int ind_start_node = translate_to_fake[start_node];
                bool check = node_matrix[ind_start_node][j];
                bool isNodeSeen = false;
                if (check) {
                    for (int k = 0; k < seen_i; ++k) {
                        if (seen[k] == translate_to_real[j]) {
                            isNodeSeen = true;
                            break;
                        }
                    }

                    if (!isNodeSeen) {
                        queue.emplace_back(translate_to_real[j], queue.front().second + 1);
                        seen[seen_i] = translate_to_real[j];
                        seen_i++;
                    }
                }
            }
        }
        queue.erase(queue.begin());
    }

    return (max(0, static_cast<int>(translate_to_real.size() - seen_i)));
}

int main() {
    int n, count = 1;
    while(cin >> n && n) {
        Nodes node(n);

        long long int x; int y;
        while(cin >> x >> y && (x != 0 || y != 0)) {
            cout << "Case " << count++ << ": " << node.search_TTL(x, y) << " nodes not reachable from node " << x << " with TTL = " << y << "." << endl;
        }
    }

    return 0;
}

