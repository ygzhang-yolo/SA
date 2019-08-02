#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define T0 5000.0
#define T_end (1e-8)
#define q 0.77
#define L 10
#define MAX 3000
#define MAXV 6000
#define Max 6000
#define INF 1000000.0

int path_DFS[2][Max] = { 0 };            //path[1]用来储存最短路径
int visited_DFS[Max] = { 0 };            //cnt用来储存最短路径对应的长度, lengthformer用来储存最短路径
int cntDfs = 0; double lenmin_DFS = 100000.0;
int path_Dijkstra[2][Max];  int cntDijkstra;  //Dijkstra的最短路径path[1]中存储, 最短长度在dist[v]中,cntDijkstra记录路径数组长度
int path_SPFA[2][Max]; int cntSPFA;           //SPFA的最短路径在path[1]中储存, 最短长度在dis[v]中,cntSPFA记录路径数组长度
int path_BFS[Max];
int path_Astar[Max];
double A[MAX][MAX];
int path[MAX][MAX];

typedef struct node {
	int data;
	int  g, h;
	int parent;
}node;
typedef struct {
	int data;
	int parent;
}QUERE1;
typedef struct ANode {
	int adjvex;
	struct ANode *nextarc;
	double weight;
	char character;
}ArcNode;             //边结点的类型
typedef struct Vnode {
	char info;
	ArcNode *firstarc;
}VNode;
typedef struct {
	VNode adjlist[MAXV];
	int n, e;
}AdjGraph;

typedef struct {
	int x;
	int y;
	int parent;
	char data;
}Point;

int visited[MAX][MAX] = { 0 };
int dx[] = { -1, 1, 0, 0 };
int dy[] = { 0, 0, -1, 1 };
int step = 0;
int lenmin = 0;
int n, m;
int count = 0;
int count1 = 0;
int cnt = 0;
Point op, ed;
char p[Max];
int n_food[100];
int pathway[100];

void Init(game_state_t state, AdjGraph* G);
void BFS(AdjGraph G, int start, int end);
void transfor(int v, game_state_t *state);
void Floyd(AdjGraph G);
void swap(int* a, int *b);
int main() {
	game_state_t state;
	memset(&state, 0, sizeof(state));
	init(&state);
	// write your code here
	int i, j;
	double T = T0;
	double minpath = 0;
	double minpath1 = 0;
	n = state.n;
	m = state.m;
	op.x = state.start_x, op.y = state.start_y;
	AdjGraph G;
	Init(state, &G);
	Floyd(G);
	int num_in = (state.start_x * m + state.start_y);
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			if (state.food[i][j] == 1) {
				n_food[count] = i * m + j;
				count++;
			}
		}
	}
	count1 = count;
	while (count > 0) {
		int nearest;
		double MIN = 9999.9;
		for (i = 0; i < m * n; i++) {
			int u = i / m;
			int v = i % m;
			if (state.food[u][v] == 1 && A[num_in][i] < MIN && num_in != i) {
				MIN = A[num_in][i];
				nearest = i;
			}
		}
		minpath += MIN;
		int num_out = nearest;
		num_in = num_out;
		state.food[num_out / m][num_out % m] = 0;
		pathway[count1 - count] = num_out;
		count--;
	}
	num_in = (state.start_x * m + state.start_y);
	srand((unsigned)time(NULL));
	while (T > T_end) {
		for (int k = 0;k < L; k++) {
			int n1, n2;
			n1 = rand() % count1;
			do {
				n2 = rand() % count1;
			} while (n1 == n2);
			if (n1 > n2) {
				swap(&n1, &n2);
			}
			int left = n1, right = n2;
			while (left < right) {
				swap(&pathway[left], &pathway[right]);
				left++; right--;
			}
			minpath1 = 0;
			for (i = 0; i < count1 - 1; i++) {
				minpath1 += A[pathway[i]][pathway[i + 1]];
			}
			minpath1 += A[num_in][pathway[0]];
			if (minpath1 < minpath) {
				minpath = minpath1;
			}
			else {
				double r = (rand() % 100000)*0.00001;
				if (exp((minpath - minpath1) / T <= r)) {
					left = n1, right = n2;
					while (left < right) {
						swap(&pathway[left], &pathway[right]);
						left++; right--;
					}
				}
			}
		}
		T *= q;
	}
	for (i = 0; i < count1; i++) {
		int num_out = pathway[i];
		BFS(G, num_in, num_out);
		transfor(num_out, &state);
		j = 0;
		while (path_BFS[j] != num_out) {
			printf("%c", p[j]);
			j++;
		}
		num_in = num_out;
	}
	destroy(&state);
	system("PAUSE");
	return 0;
}
void BFS(AdjGraph G, int start, int end) {
	int i, w;
	int path1[MAXV];
	//int path2[Max];
	int count = 0;
	ArcNode *p;
	QUERE1 qu[MAXV];
	int front = -1; int rear = -1;
	int visited_BFS[MAXV] = { 0 };
	rear++;
	qu[rear].data = start;
	qu[rear].parent = -1;
	visited_BFS[start] = 1;
	while (front != rear) {
		front++;
		w = qu[front].data;
		if (w == end) {
			i = front;
			while (qu[i].parent != -1)
			{
				path1[count] = qu[i].data;
				count++;
				i = qu[i].parent;
			}
			path1[count] = qu[i].data;
			int m = count;
			for (int k = 0; k < m; k++) {
				path_BFS[k] = path1[count];
				count--;
			}
			path_BFS[m] = end;
			//for (int i = 0; i <= m; i++)
				//printf("%d ", path2[i]);
			return;
		}
		p = G.adjlist[w].firstarc;
		while (p != NULL) {
			if (visited_BFS[p->adjvex] == 0) {
				visited_BFS[p->adjvex] = 1;
				rear++;
				qu[rear].data = p->adjvex;
				qu[rear].parent = front;
			}
			p = p->nextarc;
		}
	}
}
void Init(game_state_t state, AdjGraph* G) {
	int i, j; ArcNode *p;
	for (i = 0; i < n * m; i++) {
		G->adjlist[i].firstarc = NULL;
	}
	for (i = 0; i < n * m; i++) {
		Point u;
		u.x = i / m;
		u.y = i % m;
		if (state.grid[u.x][u.y] == 0) {
			for (j = 0; j < 4; j++) {
				Point t;
				t.x = u.x + dx[j];
				t.y = u.y + dy[j];
				switch (j) {
				case 0: t.data = 'N'; break;
				case 1: t.data = 'S'; break;
				case 2: t.data = 'W'; break;
				case 3: t.data = 'E'; break;
				}
				if (t.x >= 0 && t.x < n && t.y >= 0 && t.y < m
					&& state.grid[t.x][t.y] == 0) {
					p = (ArcNode*)malloc(sizeof(ArcNode));
					p->adjvex = (t.x * m + t.y);
					p->weight = 1;
					p->nextarc = G->adjlist[i].firstarc;
					G->adjlist[i].firstarc = p;
				}
			}
		}
	}
	G->n = n * m;
}
void transfor(int num_out, game_state_t *state) {
	int i = 0;
	while (path_BFS[i] != num_out) {
		int u = path_BFS[i] / m;
		int v = path_BFS[i] % m;
		if (state->food[u][v] == 1) {
			state->food[u][v] = 0;
			pathway[count1 - count] = num_out;
			count--;
		}
		int x = path_BFS[i + 1] - path_BFS[i];
		x %= (m - 2);
		switch (x) {
		case -2: p[i] = 'N'; break;
		case 2:  p[i] = 'S'; break;
		case -1: p[i] = 'W'; break;
		case 1:  p[i] = 'E'; break;

		}
		i++;
	}
}
void Floyd(AdjGraph G)							//Floyd算法
{
	int i, j, k;
	for (i = 0; i < MAX; i++)
		for (j = 0; j < MAX; j++) {
			path[i][j] = -1;
			if (i == j) {
				A[i][j] = 0;
			}
			else
				A[i][j] = INF;
		}
	ArcNode *p;
	for (i = 0; i < G.n; i++) {
		p = G.adjlist[i].firstarc;
		while (p != NULL) {
			A[i][p->adjvex] = p->weight;
			path[i][p->adjvex] = i;
			p = p->nextarc;
		}
	}
	for (k = 0; k < G.n; k++)						//依次考察所有顶点
	{
		for (i = 0; i < G.n; i++)
			for (j = 0; j < G.n; j++)
				if (A[i][j] > A[i][k] + A[k][j])
				{
					A[i][j] = A[i][k] + A[k][j];	//修改最短路径长度
					path[i][j] = path[k][j];		//修改最短路径
				}
	}
	//Dispath(g, A, path);							//输出最短路径
}
void swap(int* a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}
