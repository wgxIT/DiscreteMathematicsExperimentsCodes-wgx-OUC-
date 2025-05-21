#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <sstream>

using namespace std;

// Dijkstra�㷨ʵ��
void Dijkstra(int start, vector<int>& distance, vector<int>& precursor, vector<vector<int>> adjacencyMatrix)
{
	// ���������ľ���ֵ
	const int INF = INT_MAX;
	int n = adjacencyMatrix.size();
	distance.resize(n, INF); // ��ʼ����������Ϊ�����
	distance[start] = 0; // ��㵽����ľ���Ϊ0
	precursor.resize(n, -1); // ��ʼ��ǰ���ڵ����飬��ʾ��δ���ʵĽڵ�ǰ��Ϊ-1

	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; // С���Ѵ洢δ���ʽڵ�
	pq.push(make_pair(0, start)); // ��������С����

	while (!pq.empty()) {
		int dist = pq.top().first;
		int currNode = pq.top().second;
		pq.pop();

		// �����ǰ�ڵ��Ѿ������ʹ�������
		if (dist > distance[currNode]) {
			continue;
		}

		// ������ǰ�ڵ�����ڽڵ�
		for (int i = 0; i < n; ++i)
		{
			if (adjacencyMatrix[currNode][i] != INF)
			{ // �жϵ�ǰ�ڵ������ڽڵ��Ƿ��������
				int newDistance = dist + adjacencyMatrix[currNode][i];
				if (newDistance < distance[i])
				{ // ����µľ����С
					distance[i] = newDistance;
					precursor[i] = currNode; // ����ǰ���ڵ�
					pq.push(make_pair(newDistance, i));
				}
			}
		}
	}
}

void dfs(int u, int n, int m, int grid[][1000], bool edgest[], bool pointst[], vector<vector<int>>& ans, vector<int>& setsize)
{
	if (u > m)
	{
		vector<int>path;
		for (int i = 0; i < m; i++)
		{
			if (edgest[i])path.push_back(i);
		}
		setsize.push_back(path.size());
		ans.push_back(path);
		return;
	}
	int temp = 0;//��ʱ��������˱߹����ĵ���û�б�������
	int edge[2] = { 0 };//����������������
	for (int i = 0; i < n; i++)
	{
		if (grid[i][u] && !pointst[i])
		{
			temp++;
			if (temp == 1)edge[0] = i;
			else edge[1] = i;
		}
	}
	if (temp == 2)//˵������������
	{
		edgest[u] = true;
		pointst[edge[0]] = true;
		pointst[edge[1]] = true;
		dfs(u + 1, n, m, grid, edgest, pointst, ans, setsize);
		edgest[u] = false;
		pointst[edge[0]] = false;
		pointst[edge[1]] = false;
		dfs(u + 1, n, m, grid, edgest, pointst, ans, setsize);
	}
	else
	{   //��߲�����Ҳ���ѣ���û��������ô�࣬��Ϊ���øı�״̬
		dfs(u + 1, n, m, grid, edgest, pointst, ans, setsize);
	}
}

void copyGraph(int src[][100], int dest[][100], int n) {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			dest[i][j] = src[i][j];
		}
	}
}

void SearchPath(int x, int graph[][100], int n, vector<int>& ans)
{
	for (int y = 0; y < n; ++y)
	{
		if (graph[x][y] > 0)
		{
			graph[x][y]--;
			graph[y][x]--;
			SearchPath(y, graph, n, ans);
		}
	}
	ans.push_back(x);
}



// �������·��
void shortestPath(const vector<int>& precursor, int start, int end, vector<int>& path)
{
	path.clear();
	int curr = end;
	while (curr != start)
	{ // ���յ���ݵ����
		path.push_back(curr);
		curr = precursor[curr];
	}
	path.push_back(start);

	reverse(path.begin(), path.end()); // ��ת·����ʹ�����㵽�յ�
}



void circuit(int n, int m, int incMartix[][1000])
{
	bool flag1 = false, flag2 = false, flag3 = false;//�����������Ӧ
	vector<int> path;//���ÿ�������
	for (int i = 0; i < n; i++)
	{
		int sum = 0;
		for (int j = 0; j < m; j++)
		{
			if (incMartix[i][j])
				sum++;
		}
		path.push_back(sum);
	}
	int odd = 0;//��¼�ж��ٸ���������,ͬʱ���������ȶ���
	vector<int> oddnum;
	for (int i = 0; i < n; i++)
		if (path[i] % 2 == 1)
		{
			odd++;
			oddnum.push_back(i);
		}
	vector<int>result;//��Ž��
	//��oddΪ0�������Ϊŷ��ͼ,���еĶ�����Ϊż��
	//����������ת��Ϊ�ڽӾ���ͱ߾���
	int graph[50][100] = { 0 }, bian[50][100] = { 0 }, graph2[50][100] = { 0 };
	for (int j = 0; j < m; j++)
	{
		vector<int>arr;
		for (int i = 0; i < n; i++)
		{
			if (incMartix[i][j])arr.push_back(i);
		}
		int L = arr[0], r = arr[1];
		graph[L][r] = 1;
		graph[r][L] = 1;
		bian[L][r] = j;
		bian[r][L] = j;
	}

	if (odd != 0)
	{
		int dfsgridst[100] = { 0 };
		for (int i = 0; i < path.size(); i++)dfsgridst[path[i]] = i;
		int oddgrid[100][100] = { 0 };//����������Ķ�Ӧ�����·�ľ���
		vector<vector<vector<int>>>oddpath(odd, vector<vector<int>>(odd));
		// ���������ľ���ֵ
		const int INF = INT_MAX;
		vector<vector<int>> adjacencyMatrix(n, vector<int>(n, INF));
		vector<int> ans;
		//��adjacencyMatrix���ڽӾ���
		for (int j = 0; j < m; j++)
		{
			vector<int>arr;
			for (int i = 0; i < n; i++)
			{
				//������ֱ�ӵ�����ô����������
				if (incMartix[i][j])arr.push_back(i);
			}
			int L = arr[0], r = arr[1];
			adjacencyMatrix[L][r] = 1;
			adjacencyMatrix[r][L] = 1;
		}
		for (int i = 0; i < n; i++)adjacencyMatrix[i][i] = 0;

		for (int i = 0; i < odd; i++)
		{
			int start = oddnum[i]; // ���
			vector<int> distance; // ������̾��������
			vector<int> precursor; // ����ǰ���ڵ������

			Dijkstra(start, distance, precursor, adjacencyMatrix);
			for (int k = 0; k < distance.size(); k++)//distance�������㵽��k�������̾���
			{
				for (int z = 0; z < odd; z++)
				{
					if (oddnum[z] == k)
					{
						oddgrid[i][z] = distance[k];
					}
				}
			}
			for (int j = 0; j < odd; j++)
			{
				if (i == j)
				{
					oddpath[i][j].push_back(0);
					continue;
				}
				vector<int> path; // �������·��������
				int end = oddnum[j]; // �յ�
				shortestPath(precursor, start, end, path);
				for (auto x : path)//���·��
				{
					oddpath[i][j].push_back(x);
				}
			}
		}

		vector<vector<int>>ansdfs;
		vector<int>setsize;
		bool edgest[100] = { false }, pointst[100] = { false };
		//��oddnumת��Ϊ��������,��n*��n-1��/2����
		int dfsgrid[20][1000] = { 0 };
		int eddfs[100][100] = { 0 };
		for (int i = 0; i < odd; i++)
			for (int j = 0; j < odd; j++)eddfs[i][j] = -1;
		int idx = 0;
		for (int p = 0; p < odd; p++)
		{
			for (int z = p + 1; z < odd; z++)
			{
				if (oddgrid[p][z])//���б߾ͽ���������������
				{
					dfsgrid[p][idx] = 1;
					dfsgrid[z][idx] = 1;
					eddfs[p][z] = idx;
					eddfs[z][p] = idx;
					idx++;
				}
			}
		}
		dfs(0, odd, idx, dfsgrid, edgest, pointst, ansdfs, setsize);
		int len = ansdfs.size();
		int max = 0;//��¼���ƥ���λ��;
		for (int i = 0; i < len; i++)
		{
			if (setsize[i] > max)
				max = i;
		}
		int reslong = ansdfs[max].size();//��¼�ҵ�������ĳ���
		//�����е����ƥ�伯���
		int mindfs = INF;
		int flagdfs = 0;
		for (int k = 0; k < len; k++)
		{
			if (setsize[k] == reslong)//�������ƥ�伯�ĳ��Ⱦͽ���Ӧ��·���ӵ�������
			{
				int sum = 0;
				for (int i = 0; i < reslong; i++)
				{
					for (int edi = 0; edi < odd; edi++)
					{
						for (int edj = edi + 1; edj < odd; edj++)
						{
							if (ansdfs[k][i] == eddfs[edi][edj])
							{
								sum += oddpath[edi][edj].size() - 1;
							}
						}
					}
				}
				if (sum < mindfs)
				{
					mindfs = sum;
					flagdfs = k;
				}
			}
		}
		vector<vector<int>>finalans;
		for (int i = 0; i < reslong; i++)
		{
			for (int edi = 0; edi < odd; edi++)
			{
				for (int edj = edi + 1; edj < odd; edj++)
				{
					if (ansdfs[flagdfs][i] == eddfs[edi][edj])
					{
						finalans.push_back(oddpath[edi][edj]);
					}
				}
			}
		}
		for (int i = 0; i < finalans.size(); i++)
		{
			for (int j = 0; j < finalans[i].size() - 1; j++)
			{
				int Le = finalans[i][j], ri = finalans[i][j + 1];
				graph[Le][ri] += 1;
				graph[ri][Le] += 1;
			}
		}
	}
	int copiedGraph[100][100];
	copyGraph(graph, copiedGraph, n);
	vector<int> first;
	SearchPath(1, graph, n, first);
	int needlen = first.size();
	vector<int> turned;
	printf("\n��һ��{");
	for (int i = 0; i < needlen - 1; i++)
	{
		printf("e%d", bian[first[i]][first[i + 1]] + 1);
	}
	printf("}");
	vector<int> second;
	SearchPath(3, copiedGraph, n, second);
	int len2 = second.size();
	printf("\n�ڶ���{");
	for (int i = 0; i < len2 - 1; i++)
	{
		printf("e%d", bian[second[i]][second[i + 1]] + 1);
	}
	printf("}");

}

int main()
{
	int temp1[100][1000] = { 0 };
	int n, m;

	vector<vector<int>>martix1;
	vector<int>temp2;
	bool edgest[100] = { false }, pointst[100] = { false };
	cout << "��ֱ����붥�����ͱ�����" << endl;
	cin >> n >> m;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			cin >> temp1[i][j];
	}
	dfs(0, n, m, temp1, edgest, pointst, martix1, temp2);
	int len = martix1.size();

	int max = 0;//��¼���ƥ���λ��;
	for (int i = 0; i < len; i++)
	{
		if (temp2[i] > temp2[max])
			max = i;
	}
	int reslong = martix1[max].size();//��¼�ҵ�������ĳ���
	//�����е����ƥ�伯���
	cout << "���ƥ����= " << temp2[max] << endl << "���ƥ�䣺" << endl;
	for (int k = 0; k < len; k++)
	{
		if (temp2[k] == reslong)//�������ƥ�伯�ĳ��Ⱦ����
		{
			printf("{");
			for (int i = 0; i < reslong; i++)
			{
				printf("e%d,", martix1[k][i] + 1);
			}
			printf("\b}\n");
		}

	}
	circuit(n, m, temp1);
	return 0;
}
