#include<iostream>
#include<vector>
#include<stack>
#include<unordered_set>
using namespace std;

vector<vector<int>> Correlation_matrix(vector<vector<int>> adjacency_matrices)//��ȡ��������
{
    vector<vector<int>> t;
    for (int i = 0; i < adjacency_matrices.size(); i++) {
        for (int j = 0; j < adjacency_matrices.size(); j++) {
            if (adjacency_matrices[i][j] > 0) {
                for (int k = 0; k < adjacency_matrices[i][j]; k++) {
                    vector<int> line;
                    line.assign(adjacency_matrices.size(), 0);
                    line[i] = 1;
                    line[j] = 1;
                    t.push_back(line);
                    adjacency_matrices[i][j]--;
                    adjacency_matrices[j][i]--;
                }
            }
        }
    }
    return t;
}

int Calculate_determinant(int n, vector<vector<int> >a)
{
    // ʹ�õݹ��㷨��������ʽ��ֵ
    if (n == 1)
    {
        return a[0][0];
    }
    else if (n == 2)
        return a[0][0] * a[1][1] - a[0][1] * a[1][0];
    else
    {
        int sum = 0;
        for (int k = 0; k < n; k++)
        {
            vector<vector<int> > b;
            for (int i = 1; i < n; i++)
            {
                vector<int>c;
                for (int j = 0; j < n; j++)
                {
                    if (j == k)
                        continue;
                    c.push_back(a[i][j]);
                }
                b.push_back(c);
            }
            sum = k % 2 == 0 ? sum + a[0][k] * Calculate_determinant(n - 1, b) : sum - a[0][k] * Calculate_determinant(n - 1, b);
        }
        return sum;
    }
}

vector<vector<int>> Make_degree_matrix(vector<vector<int>>adjacency_matrices)//�����������
{
    vector<vector<int>> degree_matrix;
    for (int i = 0; i < adjacency_matrices[0].size(); i++) {
        int sum_degree_temp = 0;
        for (auto& j : adjacency_matrices) {
            sum_degree_temp += j[i];
        }
        vector<int> temp;
        temp.assign(adjacency_matrices.size(), 0);
        temp[i] = sum_degree_temp;
        degree_matrix.push_back(temp);
    }
    return degree_matrix;
}

int tree_num(vector<vector<int>>adjacency_matrices)//����������������
{
    vector<vector<int>> degree_matrix = Make_degree_matrix(adjacency_matrices);
    for (int i = 0; i < adjacency_matrices.size(); i++) {
        for (int j = 0; j < adjacency_matrices.size(); j++) {
            degree_matrix[i][j] -= adjacency_matrices[i][j];//����������˹����
        }
    }
    degree_matrix.pop_back();//ɾ�����������˹��������һ�������һ��
    for (auto& row : degree_matrix) {
        row.pop_back();
    }
    int row = degree_matrix.size();
    int value_determinant = Calculate_determinant(row, degree_matrix);//��������ʽ��ֵ
    return value_determinant;
}

void Show_correlation_matrix(vector<vector<int>> correlation_matrix)//��ʾ��������
{
    cout << "���ڽӾ���Ĺ�����������" << endl;
    cout << "\t";
    for (int i = 0; i < correlation_matrix.size(); i++) {
        cout << "e" << i << "\t";
    }
    cout << endl;

    for (int i = 0; i < correlation_matrix[0].size(); i++) {
        cout << "v" << i << "\t";
        for (auto& j : correlation_matrix) {
            cout << j[i] << "\t";
        }
        cout << endl;
    }
}

void Show_adjacency_matrices(vector<vector<int>> adjacency_matrices)//��ʾ�ڽӾ���
{
    cout << "�ڽӾ�����ʾ����" << endl;
    cout << "\t";
    for (int i = 0; i < adjacency_matrices.size(); i++) {
        cout << "v" << i << "\t";
    }
    cout << endl;
    for (int i = 0; i < adjacency_matrices.size(); i++) {
        cout << "v" << i << "\t";
        for (int j = 0; j < adjacency_matrices.size(); j++) {
            cout << adjacency_matrices[i][j] << "\t";
        }
        cout << endl;
    }
}

void Show_tree_num(vector<vector<int>> adjacency_matrices) {
    int value_determinant = tree_num(adjacency_matrices);
    cout << "������������Ϊ: " << value_determinant << endl;
}

// �������������DFS������������
void dfs(int node, vector<vector<int>>adjacency_matrices, vector<bool>& visited, vector<vector<int>>& tree) {
    visited[node] = true;
    for (int i = 0; i < adjacency_matrices.size(); ++i) {
        if (adjacency_matrices[node][i] != 0 && !visited[i]) {
            // ��¼�� i->node �� node->i
            tree[node][i] = 1;
            tree[i][node] = 1;
            dfs(i, adjacency_matrices, visited, tree);
        }
    }
}

vector<vector<int>> Generate_tree(vector<vector<int>>adjacency_matrices) {
    int n = adjacency_matrices.size();
    vector<bool> visited(n, false);  // ��ǽڵ��Ƿ���ʹ�
    vector<vector<int>> tree(adjacency_matrices.size(), vector<int>(adjacency_matrices.size(), 0));       // �洢�������ı�

    // �ӵ�һ���ڵ㿪ʼDFS����������
    dfs(0, adjacency_matrices, visited, tree);
    return tree;
}

vector<vector<int>> Find_trimming(vector<vector<int>>adjacency_matrices, vector<vector<int>> tree) { // �ҳ����
    vector<vector<int>> trimming;
    for (int i = 0; i < adjacency_matrices.size(); i++) {
        for (int j = 0; j < adjacency_matrices.size(); j++) {
            adjacency_matrices[i][j] -= tree[i][j];
            if (adjacency_matrices[i][j] == 1 && i < j) {
                vector<int>temp;
                temp.push_back(i);
                temp.push_back(j);
                trimming.push_back(temp);
            }
        }
    }
    return trimming;
}

vector<vector<int>> Find_tree(vector<vector<int>> tree) {  // �ҳ����ı�
    vector<vector<int>> tree_side;
    for (int i = 0; i < tree.size(); i++) {
        for (int j = 0; j < tree.size(); j++) {
            if (tree[i][j] == 1 && i < j) {
                vector<int>temp;
                temp.push_back(i);
                temp.push_back(j);
                tree_side.push_back(temp);
            }
        }
    }
    return tree_side;
}

void dfs_now(int begin, int end, vector<vector<int>>tree, vector<int>& loop_now, int symbol) { // ��·dfs
    loop_now.push_back(end);
    for (int i = 0; i < tree[end].size(); i++) {
        if (tree[end][i] == 1 && i != begin)
        {
            if (i == symbol) {

                loop_now.push_back(symbol);
                return;
            }
            else {
                dfs_now(end, i, tree, loop_now, symbol);
            }
        }
    }
}

vector<vector<int>>Construct_loops(vector<vector<int>>trimming, vector<vector<int>> tree) { //���������·ϵͳ
    vector<vector<int>>loops;
    for (int i = 0; i < trimming.size(); i++) {
        vector<int>loop_now;
        int begin = trimming[i][0];
        int end = trimming[i][1];
        dfs_now(begin, end, tree, loop_now, begin);
        auto it = find(loop_now.begin(), loop_now.end(), begin);
        if (it != loop_now.end()) {
            loop_now.erase(it + 1, loop_now.end());
        }
        loops.push_back(loop_now);
    }
    return loops;
}

void dfs_cut(int node, vector<vector<int>>& adj, vector<bool>& visited, unordered_set<int>& component) {
    stack<int> s;
    s.push(node);
    visited[node] = true;
    while (!s.empty()) {
        int current = s.top();
        s.pop();
        component.insert(current);

        for (int neighbor = 0; neighbor < adj.size(); ++neighbor) {
            if (adj[current][neighbor] == 1 && !visited[neighbor]) {
                visited[neighbor] = true;
                s.push(neighbor);
            }
        }
    }
}

// ��ͼ��ȥ��������һ���ߺ�Ķϼ�
vector<vector<int>> Find_cut_set(vector<vector<int>>& adj, vector<int>edge_to_remove, vector<vector<int>>& tree) {
    int n = adj.size();
    vector<bool> visited(n, false);
    adj[edge_to_remove[0]][edge_to_remove[1]] = 0;
    adj[edge_to_remove[1]][edge_to_remove[0]] = 0;
    tree[edge_to_remove[0]][edge_to_remove[1]] = 0;
    tree[edge_to_remove[1]][edge_to_remove[0]] = 0;

    // ��ȡȥ���ñߺ����ͨ����
    unordered_set<int> component;
    dfs_cut(edge_to_remove[0], tree, visited, component);

    // �ҵ���Ͽ���������ͨ�������ӵı�
    vector<vector<int>> cut_set;
    for (int u = 0; u < n; ++u) {
        for (int v = u + 1; v < n; ++v) {
            if (adj[u][v] == 1) {
                // ���u��v���ڲ�ͬ����ͨ�飬���������Ǹ��
                if (component.find(u) == component.end() && component.find(v) != component.end() || component.find(u) != component.end() && component.find(v) == component.end()) {
                    vector<int> temp;
                    temp.push_back(u);
                    temp.push_back(v);
                    cut_set.push_back(temp);
                }
            }
        }
    }
    adj[edge_to_remove[0]][edge_to_remove[1]] = 1;
    adj[edge_to_remove[1]][edge_to_remove[0]] = 1;
    tree[edge_to_remove[0]][edge_to_remove[1]] = 1;
    tree[edge_to_remove[1]][edge_to_remove[0]] = 1;
    return cut_set;
}
vector<int> trans_V_to_E(vector<vector<int>>correlation_matrix, vector<vector<int>>side_v) {
    int n = correlation_matrix.size();
    int n_v = side_v.size();
    vector<int> side_e;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n_v; j++) {
            if (correlation_matrix[i][side_v[j][0]] == 1 && correlation_matrix[i][side_v[j][1]] == 1)
                side_e.push_back(i);
        }
    }
    return side_e;
}

void show_system(vector<vector<int>>side_e) {
    for (int i = 0; i < side_e.size(); i++) {
        for (int j = 0; j < side_e[i].size(); j++) {
            cout << "e" << side_e[i][j];
        }
        cout << ",";
    }
}


vector<int> Union_sets(const vector<int>& vec1, const vector<int>& vec2) {
    unordered_set<int> set1(vec1.begin(), vec1.end());
    unordered_set<int> set2(vec2.begin(), vec2.end());
    unordered_set<int> common;  // �����洢�������ϵĽ���
    // �����������ϵĽ���
    for (int num : set1) {
        if (set2.find(num) != set2.end()) {
            common.insert(num);  // �����set2��Ҳ���ڣ����뽻��
        }
    }
    vector<int> result;
    for (int num : vec1) {
        if (common.find(num) == common.end()) {
            result.push_back(num);
        }
    }
    for (int num : vec2) {
        if (common.find(num) == common.end()) {
            result.push_back(num);
        }
    }
    return result;
}


bool Is_exist(const vector<vector<int>>& loopSpace, const vector<int>& loop) {
    unordered_set<int> loopSet(loop.begin(), loop.end());
    // ������·�ռ��е�ÿһ����·
    for (const auto& existingLoop : loopSpace) {
        unordered_set<int> existingLoopSet(existingLoop.begin(), existingLoop.end());
        if (loopSet == existingLoopSet) {
            return true; // �����·�Ѵ��ڣ��򷵻� true
        }
    }
    return false;
}

void Generate_space_recursive(const vector<vector<int>>& basicCycles, vector<vector<int>>& loopSpace, vector<vector<int>>& newCombinations, size_t idx) {
    if (idx >= basicCycles.size()) {
        return;
    }

    // ���Խ���ǰ��·ֱ�Ӽ��뵽��·�ռ���
    const auto& cycle = basicCycles[idx];
    if (!Is_exist(loopSpace, cycle)) {
        loopSpace.push_back(cycle);
    }

    for (size_t i = 0; i < loopSpace.size(); ++i) {
        vector<int> newCycle = Union_sets(loopSpace[i], cycle);
        if (!Is_exist(loopSpace, newCycle) && !Is_exist(newCombinations, newCycle)) {
            newCombinations.push_back(newCycle);
        }
    }

    if (!newCombinations.empty()) {
        loopSpace.insert(loopSpace.end(), newCombinations.begin(), newCombinations.end());
        newCombinations.clear();
        Generate_space_recursive(basicCycles, loopSpace, newCombinations, idx + 1);
    }
}

vector<vector<int>> Generate_space(const vector<vector<int>>& basicCycles) {
    vector<vector<int>> loopSpace;
    vector<vector<int>> newCombinations;
    Generate_space_recursive(basicCycles, loopSpace, newCombinations, 0);
    return loopSpace;
}

int main()
{
    int num_vertices, t;
    cout << "�������ĸ���:" << endl;
    cin >> num_vertices;

    cout << "���������" << endl;
    vector<vector<int>> adjacency_matrices;
    for (int i = 0; i < num_vertices; i++) {
        vector<int> temp;
        for (int j = 0; j < num_vertices; j++) {
            cin >> t;
            temp.push_back(t);
        }
        adjacency_matrices.push_back(temp);
    }

    vector<vector<int>> correlation_matrix = Correlation_matrix(adjacency_matrices);
    Show_correlation_matrix(correlation_matrix);//��ʾ��������
    Show_tree_num(adjacency_matrices);
    vector<vector<int>> tree = Generate_tree(adjacency_matrices);

    cout << "��ͼ���������ڽӾ���" << endl;
    Show_adjacency_matrices(tree);

    vector<vector<int>>trimming = Find_trimming(adjacency_matrices, tree);
    vector<vector<int>>loops = Construct_loops(trimming, tree);

    vector<vector<int>>loops_side_e;// �ñߴ���Ļ�·ϵͳ
    for (int i = 0; i < loops.size(); i++) {
        vector<vector<int>>loops_side_v;//�õ����Ļ�·ϵͳ
        for (int j = 0; j < loops[i].size(); j++) {
            int temp = j + 1;
            if (temp == loops[i].size())
                temp = 0;
            vector<int>temp_v;
            temp_v.push_back(loops[i][j]);
            temp_v.push_back(loops[i][temp]);
            loops_side_v.push_back(temp_v);
        }
        vector<int>temp_e = trans_V_to_E(correlation_matrix, loops_side_v);
        loops_side_e.push_back(temp_e);

    }

    cout << "������·ϵͳ:{ ";
    show_system(loops_side_e);
    cout << "}" << endl;
    cout << "��·�ռ�: { ��,";
    show_system(Generate_space(loops_side_e));
    cout << "}" << endl;
    vector<vector<int>> tree_side = Find_tree(tree);
    vector<vector<int>>cut_side_e;
    for (int i = 0; i < tree_side.size(); i++) {
        vector<vector<int>>cut_side_v;
        vector<int> temp_v;
        temp_v.push_back(tree_side[i][0]);
        temp_v.push_back(tree_side[i][1]);
        cut_side_v.push_back(temp_v);
        vector<vector<int>> cut_set = Find_cut_set(adjacency_matrices, tree_side[i], tree);
        for (int i = 0; i < cut_set.size(); i++) {
            vector<int> temp_v;
            temp_v.push_back(cut_set[i][0]);
            temp_v.push_back(cut_set[i][1]);
            cut_side_v.push_back(temp_v);
        }
        vector<int>temp_e = trans_V_to_E(correlation_matrix, cut_side_v);
        cut_side_e.push_back(temp_e);
    }

    cout << "�����ϵͳ:{ ";
    show_system(cut_side_e);
    cout << "}" << endl;
    cout << "�ϼ��ռ�:{ ��,";
    show_system(Generate_space(cut_side_e));
    cout << "}" << endl;
    return 0;
}