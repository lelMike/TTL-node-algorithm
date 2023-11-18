#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <string>

#define MAX_EDGE 300

using namespace std;

class Nodes {
public:
    explicit Nodes(bool isDirectional = false);
    void print_matrix();
    int max_subgraphs();

private:
    bool node_matrix[MAX_EDGE][MAX_EDGE]{};
    bool isDirectional = false;
    int node_num = 0;
    unordered_map<char, int> translate_to_fake;
    vector<char> translate_to_real;
    vector<pair<char, char>> pairs;
};

Nodes::Nodes(bool isDirectional) {
    this->isDirectional = isDirectional;
    fill(&node_matrix[0][0], &node_matrix[0][0] + sizeof(node_matrix) / sizeof(bool), false);
    char x, y;
    string line;
    string buffer; getline(cin, buffer);
    getline(cin, line); x = line[0]; y = line[1];
    pairs.emplace_back(x, y);

    while (pairs.back().first != '\000') {
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
        getline(cin, line); x = line[0];
        if(x == '\000'){
            pairs.emplace_back('\000', '\000');
        }
        else{
            y = line[1];
            pairs.emplace_back(x, y);
        }
    }
    pairs.pop_back();
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
    cout << endl;
}

int Nodes::max_subgraphs(){
    vector<char> unseen; int max = 0; vector<char> queue, buff_seen;
    for(int i = 0; i < translate_to_real.size(); i++){
        unseen.emplace_back(translate_to_real[i]);
    }

    while(!unseen.empty()){
        max++;
        queue.emplace_back(unseen.front());
        while(!queue.empty()) {
            char curr_node = queue.front();
            buff_seen.emplace_back(curr_node);
            unseen.erase(find(unseen.begin(), unseen.end(), curr_node));
            queue.erase(queue.begin());
            for (int j = 0; j < unseen.size(); j++) {
                bool check = node_matrix[translate_to_fake[curr_node]][translate_to_fake[unseen[j]]];

                if (check) {
                    if (find(buff_seen.begin(), buff_seen.end(), translate_to_real[translate_to_fake[unseen[j]]]) == buff_seen.end()) {
                        queue.emplace_back(translate_to_real[translate_to_fake[unseen[j]]]);
                    }
                }
            }
        }

    }

    return max;
}

int main() {
    int n, count = 1; cin >> n;
    while(n != 0) {
        char max_letter; cin >> max_letter;
        Nodes node;

        node.print_matrix();

        int answer = node.max_subgraphs();
        cout << answer << endl;

        n--;
    }

    return 0;
}

