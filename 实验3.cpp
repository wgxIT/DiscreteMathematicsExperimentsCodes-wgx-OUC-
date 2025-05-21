#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <sstream>

using namespace std;

// Dijkstra算法实现
void Dijkstra(int start, vector<int>& distance, vector<int>& precursor, vector<vector<int>> adjacencyMatrix)
{
	// 定义无穷大的距离值
	const int INF = INT_MAX;
	int n = adjacencyMatrix.size();
	distance.resize(n, INF); // 初始化距离数组为无穷大
	distance[start] = 0; // 起点到自身的距离为0
	precursor.resize(n, -1); // 初始化前驱节点数组，表示还未访问的节点前驱为-1

	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; // 小顶堆存储未访问节点
	pq.push(make_pair(0, start)); // 将起点加入小顶堆

	while (!pq.empty()) {
		int dist = pq.top().first;
		int currNode = pq.top().second;
		pq.pop();

		// 如果当前节点已经被访问过，跳过
		if (dist > distance[currNode]) {
			continue;
		}

		// 遍历当前节点的相邻节点
		for (int i = 0; i < n; ++i)
		{
			if (adjacencyMatrix[currNode][i] != INF)
			{ // 判断当前节点与相邻节点是否存在连接
				int newDistance = dist + adjacencyMatrix[currNode][i];
				if (newDistance < distance[i])
				{ // 如果新的距离更小
					distance[i] = newDistance;
					precursor[i] = currNode; // 更新前驱节点
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
	int temp = 0;//临时用来计算此边关联的点有没有被搜索过
	int edge[2] = { 0 };//用来看是哪两个点
	for (int i = 0; i < n; i++)
	{
		if (grid[i][u] && !pointst[i])
		{
			temp++;
			if (temp == 1)edge[0] = i;
			else edge[1] = i;
		}
	}
	if (temp == 2)//说明这条边能用
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
	{   //这边不能用也能搜，但没有上面那么多，因为不用改变状态
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



// 构造最短路径
void shortestPath(const vector<int>& precursor, int start, int end, vector<int>& path)
{
	path.clear();
	int curr = end;
	while (curr != start)
	{ // 从终点回溯到起点
		path.push_back(curr);
		curr = precursor[curr];
	}
	path.push_back(start);

	reverse(path.begin(), path.end()); // 反转路径，使其从起点到终点
}



void circuit(int n, int m, int incMartix[][1000])
{
	bool flag1 = false, flag2 = false, flag3 = false;//与上面情况对应
	vector<int> path;//存放每个点度数
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
	int odd = 0;//记录有多少个奇数顶点,同时存下奇数度顶点
	vector<int> oddnum;
	for (int i = 0; i < n; i++)
		if (path[i] % 2 == 1)
		{
			odd++;
			oddnum.push_back(i);
		}
	vector<int>result;//存放结果
	//若odd为0则代表其为欧拉图,所有的度数均为偶数
	//将关联矩阵转化为邻接矩阵和边矩阵
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
		int oddgrid[100][100] = { 0 };//存奇数顶点的对应的最短路的矩阵
		vector<vector<vector<int>>>oddpath(odd, vector<vector<int>>(odd));
		// 定义无穷大的距离值
		const int INF = INT_MAX;
		vector<vector<int>> adjacencyMatrix(n, vector<int>(n, INF));
		vector<int> ans;
		//用adjacencyMatrix存邻接矩阵
		for (int j = 0; j < m; j++)
		{
			vector<int>arr;
			for (int i = 0; i < n; i++)
			{
				//若不能直接到达那么距离是无穷
				if (incMartix[i][j])arr.push_back(i);
			}
			int L = arr[0], r = arr[1];
			adjacencyMatrix[L][r] = 1;
			adjacencyMatrix[r][L] = 1;
		}
		for (int i = 0; i < n; i++)adjacencyMatrix[i][i] = 0;

		for (int i = 0; i < odd; i++)
		{
			int start = oddnum[i]; // 起点
			vector<int> distance; // 保存最短距离的数组
			vector<int> precursor; // 保存前驱节点的数组

			Dijkstra(start, distance, precursor, adjacencyMatrix);
			for (int k = 0; k < distance.size(); k++)//distance里面存起点到第k个点的最短距离
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
				vector<int> path; // 保存最短路径的数组
				int end = oddnum[j]; // 终点
				shortestPath(precursor, start, end, path);
				for (auto x : path)//存好路径
				{
					oddpath[i][j].push_back(x);
				}
			}
		}

		vector<vector<int>>ansdfs;
		vector<int>setsize;
		bool edgest[100] = { false }, pointst[100] = { false };
		//将oddnum转化为关联矩阵,有n*（n-1）/2条边
		int dfsgrid[20][1000] = { 0 };
		int eddfs[100][100] = { 0 };
		for (int i = 0; i < odd; i++)
			for (int j = 0; j < odd; j++)eddfs[i][j] = -1;
		int idx = 0;
		for (int p = 0; p < odd; p++)
		{
			for (int z = p + 1; z < odd; z++)
			{
				if (oddgrid[p][z])//若有边就将其存入关联矩阵当中
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
		int max = 0;//记录最大匹配的位置;
		for (int i = 0; i < len; i++)
		{
			if (setsize[i] > max)
				max = i;
		}
		int reslong = ansdfs[max].size();//记录找到的数组的长度
		//将所有的最大匹配集输出
		int mindfs = INF;
		int flagdfs = 0;
		for (int k = 0; k < len; k++)
		{
			if (setsize[k] == reslong)//若是最大匹配集的长度就将对应的路径加到矩阵当中
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
	printf("\n第一条{");
	for (int i = 0; i < needlen - 1; i++)
	{
		printf("e%d", bian[first[i]][first[i + 1]] + 1);
	}
	printf("}");
	vector<int> second;
	SearchPath(3, copiedGraph, n, second);
	int len2 = second.size();
	printf("\n第二条{");
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
	cout << "请分别输入顶点数和边数：" << endl;
	cin >> n >> m;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			cin >> temp1[i][j];
	}
	dfs(0, n, m, temp1, edgest, pointst, martix1, temp2);
	int len = martix1.size();

	int max = 0;//记录最大匹配的位置;
	for (int i = 0; i < len; i++)
	{
		if (temp2[i] > temp2[max])
			max = i;
	}
	int reslong = martix1[max].size();//记录找到的数组的长度
	//将所有的最大匹配集输出
	cout << "最大匹配数= " << temp2[max] << endl << "最大匹配：" << endl;
	for (int k = 0; k < len; k++)
	{
		if (temp2[k] == reslong)//若是最大匹配集的长度就输出
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
