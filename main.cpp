#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>

using namespace std;

class Nodes {
public:
    explicit Nodes(int n, bool isDirectional = false);
    void print_matrix();
    vector<int> search_TTL(vector<pair<int, int>>);

private:
    bool node_matrix[50][50]{};
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
        int x, y;
        cin >> x >> y;
        pairs.emplace_back(x, y);

        if(translate_to_fake.find(x) == translate_to_fake.end()){
            translate_to_fake[x] = i+i;
            translate_to_real.emplace_back(x);
        }
        if(translate_to_fake.find(y) == translate_to_fake.end()){
            translate_to_fake[y] = i+i+1;
            translate_to_real.emplace_back(y);
        }

        if(isDirectional){
            node_matrix[translate_to_fake[x]][translate_to_fake[y]] = true;
        }
        else{
            node_matrix[translate_to_fake[x]][translate_to_fake[y]] = true;
            node_matrix[translate_to_fake[y]][translate_to_fake[y]] = true;
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

vector<int> Nodes::search_TTL(vector<pair<int, int>>){

}

int main() {
    int n; cin >> n;
    while(n != 0) {
        Nodes node(n);
        node.print_matrix();

        vector<pair<int, int>> solve;
        int x, y, counter = 1; cin >> x >> y;
        while(x != 0 && y != 0) {
            solve.emplace_back(x, y);
            cin >> x >> y;
        }
        vector<int> answer = node.search_TTL(solve);


        cin >> n;
    }

    return 0;
}

