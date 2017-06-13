#include<bits/stdc++.h>
using namespace std;
#define NIL -1
#define INF INT_MAX
 
// Класс для представления двудольного графа
class BipGraph {
  // Количество вершин слева и справа у двудольного графа
  int m, n;
 
  // adj[u] хранит смежные ребра левой стороны вершины 'u'
  // Значение u находится от 1 до m.
  // 0 используется для виртуальной вершнины
  list<int> *adj;
 
  // Базовые указатели необходимые для массивов в hopcroftKarp()
  int *lpair, *rpair, *cur_rprev, *lused, *rused;
 
  // Возвращает true, если есть увеличивающий путь
  bool bfs();
public:
  BipGraph(int m, int n);
  ~BipGraph();
  void addEdge(int u, int v); // Добавить ребро

  // Возвращает ребра максимального паросочетания
  void hopcroftKarp(int *res);
};

class DestinationsTask {
  void deductMinimals(int *mat, int n);
public:
  void solve(int *mat, int n, int *res);
};
 
// Возвращает максимальное количество паросочитаний
void BipGraph::hopcroftKarp(int *res) {

    // Продолжать обновлять результат,
    // пока существует увеличивающий путь
    while (bfs());
    for (int i = 0; i < n; i++) {
      res[i] = lpair[i];
    }
}

void DestinationsTask::solve(int *mat, int n, int *res) {
  bool isSolution = false;
  int *matchings = res;
  do {
    deductMinimals(mat, n);   
    for (int i = 0 ; i < n; i++) {
      for (int j = 0; j < n; j++) {
        cout << mat[i*n+j] << "\t";
      }
      cout << endl;
    }
    cout << endl;
    BipGraph g(n, n);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (mat[i*n+j] == 0) {
          g.addEdge(i, j);
        }
      }
    }
    g.hopcroftKarp(matchings);
    isSolution = true;
    for (int i = 0; i < n; i++) {
      if (matchings[i] == NIL) {
        isSolution = false;
        break;
      }
    }
    if (!isSolution) {
      queue<int> raws, cols;
      bool rawSelected[n], colSelected[n];
      for (int i = 0; i < n; i++) {
        rawSelected[i] = matchings[i] == NIL;
        if (rawSelected[i])
          raws.push(i);
        colSelected[i] = false;
      }
      while (!raws.empty()) {
        while (!raws.empty()) {
          int raw = raws.front();
          raws.pop();
          for (int i = 0; i < n; i++) {
            if (!colSelected[i] && mat[raw*n+i] == 0 && matchings[raw] != i) {
              cols.push(i);
              colSelected[i] = true;
            }
          }
        }
        while (!cols.empty()) {
          int col = cols.front();
          cols.pop();
          for (int i = 0; i < n; i++) {
            if (!rawSelected[i] && mat[i*n+col] == 0 && matchings[i] == col) {
              raws.push(i);
              rawSelected[i] = true;
            }
          }
        }
      }
      for (int i = 0; i < n; i++) {
        rawSelected[i] = !rawSelected[i];
      }
      int min = INF;
      for (int i = 0; i < n; i++) {
        if (!rawSelected[i]) {
          for (int j = 0; j < n; j++) {
            if (!colSelected[j]) {
              if (mat[i*n+j] < min) {
                min = mat[i*n+j];
              }
            }
          }
        }
      }
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          if (rawSelected[i] && colSelected[j]) {
            mat[i*n+j] += min;
          }
          if (!rawSelected[i] && !colSelected[j]) {
            mat[i*n+j] -= min;
          }
        }
      }
    }
  } while (!isSolution);
}

BipGraph::~BipGraph() {
  delete[] lpair;
  delete[] rpair;
  delete[] cur_rprev;
  delete[] lused;
  delete[] rused;
}
 
// Возвращает существует ли увеличивающий путь
bool BipGraph::bfs()
{
  queue<int> Q;
  for (int i = 0; i < n; i++) {
    if (lpair[i] == NIL)
      Q.push(i);
    lused[i] = false;
  }
  for (int i = 0; i < m; i++) {
    cur_rprev[i] = NIL;    
    rused[i] = false;
  }
  while (!Q.empty()) {
    int u = Q.front();
    Q.pop();

    list<int>::iterator i;
    for (i=adj[u].begin(); i!=adj[u].end(); ++i)
    {
      // Смежный к u
      int v = *i;
 
      if (rpair[v] != NIL) {
        if (!rused[v]) {
          cur_rprev[v] = u;
          rused[v] = true;
          Q.push(rpair[v]);  
        }
      } else {
        cur_rprev[v] = u;
        int lvertex = u;
        int vi = v;
        while (lpair[lvertex] != NIL) {
          int lprev = lpair[lvertex];
          lpair[lvertex] = vi;
          rpair[vi] = lvertex;
          vi = lprev;
          lvertex = cur_rprev[vi];
        };
        lpair[lvertex] = vi;
        rpair[vi] = lvertex;
        return true;
      }
    }
    lused[u] = true;
  }
  return false;
}
 
// Конструктор
BipGraph::BipGraph(int m, int n)
{
    this->m = m;
    this->n = n;
    adj = new list<int>[m];
 
 

    lpair = new int[n];
    rpair = new int[m];
    cur_rprev = new int[m];
    lused = new int[n];
    rused = new int[m];
    for (int i =0; i < n; i++) {
      lpair[i] = NIL;
    }
    for (int i = 0; i < m; i++) {
      rpair[i] = NIL;
    }
 }
 
// To add edge from u to v and v to u
// Чтобы добавить ребро от u к v и от v к u
void BipGraph::addEdge(int u, int v)
{
    adj[u].push_back(v); // Добавить v к список u.
}
 
void DestinationsTask::deductMinimals(int *mat, int n) {
  for (int i = 0; i < n; i++) {
    int min = mat[i*n];
    for (int j = 1; j < n; j++) {
      if (mat[i*n+j] < min) {
        min = mat[i*n+j];
      }
    }

    for (int j = 0; j < n; j++) {
      mat[i*n+j] -= min;
    }
  }

  for (int i = 0; i < n; i++) {
    int min = mat[i];
    for (int j = 1; j < n; j++) {
      if (mat[j*n+i] < min) {
        min = mat[j*n+i];
      }
    }

    for (int j = 0; j < n; j++) {
      mat[j*n+i] -= min;
    }
  }
}



int main()
{
  int n = 4, m = 3;
  int mat[n * n] = {
     0, 30, 30, 30,
     0, 10,  0,  0,
     0,  0,  0,  0,
    20,  0,  0, 10
  };


  //for (int i = 0; i < n; i++) {
    //cout << matchings[i] << " ";
  //}
  //cout << endl;
  return 0;
}
