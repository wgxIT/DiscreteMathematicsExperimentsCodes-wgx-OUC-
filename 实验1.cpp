#include <iostream>  
#include <vector>  
#include <algorithm> // 用于 std::sort  
#include <cstring>   // 用于 memset  
using namespace std;

int sq[1000][1000] = { 0 }; // 初始化邻接矩阵为0  
pair<int, int> d[1000];   // 存储度数和原索引的数组  
bool st[1000] = { 0 };      // 状态数组，初始化为false  
int l = 1;                // 节点的数量  

// 返回两个整数中较小的一个  
int min(int x, int y) {
    return (x <= y) ? x : y;
}

// 检查度数序列是否可以构成图  
bool is_graphable(const vector<int>& degrees) {
    int sum = 0;
    for (int degree : degrees)
        sum += degree; // 计算度数的总和  
    return sum % 2 == 0; // 总度数必须是偶数  
}

// 检查度数序列是否可以构成简单图  
bool is_easy_graphable(const vector<int>& degrees) {
    if (!is_graphable(degrees)) return false; // 先判断是否可以构成图  

    int n = degrees.size();
    for (int i = 1; i <= n; i++) {
        int sum2 = 0;
        for (int j = 0; j < i; j++)
            sum2 += degrees[j]; // 计算前i个节点的度数之和  
        int sum3 = 0;
        for (int k = i; k < n; k++)
            sum3 += min(degrees[k], i); // 计算后续节点中，限制其度数为i后的和  
        if (sum2 > sum3 + i * (i - 1)) return false; // 判断是否满足条件  
    }
    return true; // 可以构成简单图  
}

// 深度优先搜索，检查图的连通性  
void dfs(int x) {
    if (st[x]) return; // 如果该节点已经被访问，直接返回  
    st[x] = true; // 标记该节点为已访问  
    for (int i = 1; i <= l; i++) {
        if (sq[x][i]) dfs(i); // 如果x与i有连接，继续访问i  
    }
}

// 根据度数序列构建邻接矩阵  
void printAdjMartix() {
    // 将度数按降序排列  
    sort(d + 1, d + l + 1, greater<pair<int, int>>());
    for (int i = 1; i <= l; i++) {
        int top = d[i].first; // 当前节点的度数  
        if (top == 0) break; // 如果度数为0，跳出循环  

        for (int j = 1; j <= top; j++) {
            if (i + j <= l) { // 防止数组越界  
                sq[d[i].second][d[i + j].second] = 1; // 创建连接  
                sq[d[i + j].second][d[i].second] = 1; // 无向图连接  
                d[i + j].first--; // 减少被连接节点的度数  
            }
        }

        // 再次按降序排列剩余的度数  
        sort(d + i + 1, d + l + 1, greater<pair<int, int>>());
    }

    // 输出邻接矩阵  
    printf("   ");
    for (int i = 1; i <= l; i++) {
        printf("V%d ", i); // 打印列头  
    }
    cout << endl;
    for (int i = 1; i <= l; i++) {
        printf("V%d ", i); // 打印行头  
        for (int j = 1; j <= l; j++) {
            printf(" %d ", sq[i][j]); // 打印邻接矩阵的内容  
        }
        cout << endl;
    }
}

int main() {
    cout << "**********************************\n";
    cout << "请输入度数列的长度：\n";
    cin >> l; // 输入节点数量  
    memset(sq, 0, sizeof(sq)); // 初始化邻接矩阵  
    memset(st, 0, sizeof(st)); // 初始化状态数组  

    cout << "请输入度数列：\n";
    for (int i = 1; i <= l; i++) {
        int a;
        cin >> a;
        d[i] = { a, i }; // 存储度数及其原始索引  
    }

    vector<int> degrees(l);
    for (int i = 1; i <= l; i++) {
        degrees[i - 1] = d[i].first; // 填充度数向量  
    }

    // 判定是否可以构成图  
    if (is_graphable(degrees)) {
        cout << "可图化  ";
    }
    else {
        cout << "不可图化  ";
        return 0; // 如果不可图化则提前退出  
    }

    // 判定是否可以构成简单图  
    if (is_easy_graphable(degrees)) {
        cout << "可简单图化\n" << endl;
        printAdjMartix(); // 构建邻接矩阵并输出  
        int fr = 0;
        for (int i = 1; i <= l; i++) {
            if (!st[i]) {
                fr++; // 统计连通分支数量  
                dfs(i); // 访问该组件  
            }
        }
        if (fr == 1) {
            printf("\n连通\n");
        }
        else {
            printf("不连通，连通分支数为%d\n", fr);
        }
    }
    else {
        cout << "不可简单图化\n" << endl;
    }

    return 0;
}