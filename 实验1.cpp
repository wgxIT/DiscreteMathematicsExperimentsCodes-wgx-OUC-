#include <iostream>  
#include <vector>  
#include <algorithm> // ���� std::sort  
#include <cstring>   // ���� memset  
using namespace std;

int sq[1000][1000] = { 0 }; // ��ʼ���ڽӾ���Ϊ0  
pair<int, int> d[1000];   // �洢������ԭ����������  
bool st[1000] = { 0 };      // ״̬���飬��ʼ��Ϊfalse  
int l = 1;                // �ڵ������  

// �������������н�С��һ��  
int min(int x, int y) {
    return (x <= y) ? x : y;
}

// �����������Ƿ���Թ���ͼ  
bool is_graphable(const vector<int>& degrees) {
    int sum = 0;
    for (int degree : degrees)
        sum += degree; // ����������ܺ�  
    return sum % 2 == 0; // �ܶ���������ż��  
}

// �����������Ƿ���Թ��ɼ�ͼ  
bool is_easy_graphable(const vector<int>& degrees) {
    if (!is_graphable(degrees)) return false; // ���ж��Ƿ���Թ���ͼ  

    int n = degrees.size();
    for (int i = 1; i <= n; i++) {
        int sum2 = 0;
        for (int j = 0; j < i; j++)
            sum2 += degrees[j]; // ����ǰi���ڵ�Ķ���֮��  
        int sum3 = 0;
        for (int k = i; k < n; k++)
            sum3 += min(degrees[k], i); // ��������ڵ��У����������Ϊi��ĺ�  
        if (sum2 > sum3 + i * (i - 1)) return false; // �ж��Ƿ���������  
    }
    return true; // ���Թ��ɼ�ͼ  
}

// ����������������ͼ����ͨ��  
void dfs(int x) {
    if (st[x]) return; // ����ýڵ��Ѿ������ʣ�ֱ�ӷ���  
    st[x] = true; // ��Ǹýڵ�Ϊ�ѷ���  
    for (int i = 1; i <= l; i++) {
        if (sq[x][i]) dfs(i); // ���x��i�����ӣ���������i  
    }
}

// ���ݶ������й����ڽӾ���  
void printAdjMartix() {
    // ����������������  
    sort(d + 1, d + l + 1, greater<pair<int, int>>());
    for (int i = 1; i <= l; i++) {
        int top = d[i].first; // ��ǰ�ڵ�Ķ���  
        if (top == 0) break; // �������Ϊ0������ѭ��  

        for (int j = 1; j <= top; j++) {
            if (i + j <= l) { // ��ֹ����Խ��  
                sq[d[i].second][d[i + j].second] = 1; // ��������  
                sq[d[i + j].second][d[i].second] = 1; // ����ͼ����  
                d[i + j].first--; // ���ٱ����ӽڵ�Ķ���  
            }
        }

        // �ٴΰ���������ʣ��Ķ���  
        sort(d + i + 1, d + l + 1, greater<pair<int, int>>());
    }

    // ����ڽӾ���  
    printf("   ");
    for (int i = 1; i <= l; i++) {
        printf("V%d ", i); // ��ӡ��ͷ  
    }
    cout << endl;
    for (int i = 1; i <= l; i++) {
        printf("V%d ", i); // ��ӡ��ͷ  
        for (int j = 1; j <= l; j++) {
            printf(" %d ", sq[i][j]); // ��ӡ�ڽӾ��������  
        }
        cout << endl;
    }
}

int main() {
    cout << "**********************************\n";
    cout << "����������еĳ��ȣ�\n";
    cin >> l; // ����ڵ�����  
    memset(sq, 0, sizeof(sq)); // ��ʼ���ڽӾ���  
    memset(st, 0, sizeof(st)); // ��ʼ��״̬����  

    cout << "����������У�\n";
    for (int i = 1; i <= l; i++) {
        int a;
        cin >> a;
        d[i] = { a, i }; // �洢��������ԭʼ����  
    }

    vector<int> degrees(l);
    for (int i = 1; i <= l; i++) {
        degrees[i - 1] = d[i].first; // ����������  
    }

    // �ж��Ƿ���Թ���ͼ  
    if (is_graphable(degrees)) {
        cout << "��ͼ��  ";
    }
    else {
        cout << "����ͼ��  ";
        return 0; // �������ͼ������ǰ�˳�  
    }

    // �ж��Ƿ���Թ��ɼ�ͼ  
    if (is_easy_graphable(degrees)) {
        cout << "�ɼ�ͼ��\n" << endl;
        printAdjMartix(); // �����ڽӾ������  
        int fr = 0;
        for (int i = 1; i <= l; i++) {
            if (!st[i]) {
                fr++; // ͳ����ͨ��֧����  
                dfs(i); // ���ʸ����  
            }
        }
        if (fr == 1) {
            printf("\n��ͨ\n");
        }
        else {
            printf("����ͨ����ͨ��֧��Ϊ%d\n", fr);
        }
    }
    else {
        cout << "���ɼ�ͼ��\n" << endl;
    }

    return 0;
}