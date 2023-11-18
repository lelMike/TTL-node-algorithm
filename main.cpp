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
    vector<int> search_TTL(vector<pair<long long int, int>> solve);

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

vector<int> Nodes::search_TTL(vector<pair<long long int, int>> solve){
    vector<int> answer;
    for(int i = 0; i < solve.size(); i++){
        if(find(translate_to_real.begin(), translate_to_real.end(), solve[i].first) == translate_to_real.end()){
            answer.emplace_back(translate_to_fake.size());
        }
        else {
            if (solve[i].second == 0) {
                answer.emplace_back(translate_to_real.size() - 1);
            } else {
                vector<pair<long long int, int>> queue;
                queue.emplace_back(solve[i].first, 0);
                int ttl = solve[i].second;
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
                answer.emplace_back(max(0, static_cast<int>(translate_to_real.size() - seen_i)));
            }
        }
    }

    return answer;
}

int main() {
    int n, count = 1; cin >> n;
    while(n != 0) {
        Nodes node(n);

        vector<pair<long long int, int>> solve;
        int x, y; cin >> x >> y;
        while(x != 0 || y != 0) {
            solve.emplace_back(x, y);
            cin >> x >> y;
        }

        vector<int> answer = node.search_TTL(solve);
        for(int i = 0; i < answer.size(); i++){
            cout << "Case " << count++ << ": " << answer[i] << " nodes not reachable from node " << solve[i].first << " with TTL = " << solve[i].second << "." << endl;
        }

        cin >> n;
    }

    return 0;
}

