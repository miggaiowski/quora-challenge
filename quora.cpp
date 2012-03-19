// This code is my solution to the Quora Challenge. 
// It is just a solution I wrote in an afternoon, 
// where I don't really care for good practice of
// software engineering (see all the global variables).
// Anyway, it solves the instace given in less than 30
// seconds on a Macbook with a Core 2 Duo 2.2 GHz. 

#include <iostream>
#include <vector>
#include <queue>

#define MAX 100

using namespace std;

int W, H; // dimensions of the grid
int si, sj, gi, gj;
int grid[MAX][MAX];
int numUsed = 0;
int size;

void addEdges(vector < vector <int> > &adj, int i, int j) {
  int v = j + (W * i);
  if (i > 0 && grid[i-1][j] != 1) {
    adj[v].push_back(v-W); // connect with the vertex above
  }
  if (j > 0 && grid[i][j-1] != 1) {
    adj[v].push_back(v-1); // edge to the vertex on the left
  }
  if (j < W-1 && grid[i][j+1] != 1) {
    adj[v].push_back(v+1); // edge to the vertex on the right
  }
  if (i < H-1 && grid[i+1][j] != 1) {
    adj[v].push_back(v+W); // connect with the vertex below
  }
}

void printEdges(vector <vector <int> > &adj) {
  for (int i = 0; i < adj.size(); i++)   {
    cout << i << ": ";
    for (int j = 0; j < adj[i].size(); j++) {
      cout << adj[i][j] << " ";
    }
    cout << endl;
  }
}

void printCurrentGrid(vector<bool> &used, int p, int q) {
  cout << endl;
  for (int i = 0; i < H; i++) {
    for (int j = 0; j < W; j++) {
      if (i == p && j == q) {
	cout << "X ";
      }
      else if (used[j+W*i] == false) {
	cout << "0 ";
      }
      else {
	cout << "x ";
      }
    }
    cout << endl;
  }	
}

bool allReachableAndWithGoodDegree(vector <vector <int> > &adj, vector<bool> &used, int p, int q) {
  // this is a BFS to see if all vertices all still reachable
  int numReached = 0;
  queue<int> Q;
  vector<bool> reached (W*H, false);
  int v = q+W*p;
  int startVertex = q+W*p;
  int goalVertex = gj+W*gi;
  Q.push(v);
  while (!Q.empty()) {
    v = Q.front(); Q.pop();
    for (int k = 0; k < adj[v].size(); k++) {
      int c = adj[v][k];
      if (used[c] == false && reached[c] == false) {

	// the next few lines check the number of available neighboors of vertex c
	int deg = 0;
	for (int m = 0; m < adj[c].size(); m++) {
	  if (used[adj[c][m]] == false || adj[c][m] == startVertex) {
	    deg++;
	  }
	}
	// we can't have a vertex with less than 2 open neighboors, otherwise we wouldn't be able to get out of there
	if (deg < 2 && c != goalVertex) { // unless it's the goal vertex, then it's fine
	  return false;
	}

	reached[c] = true;
	Q.push(c);
	numReached++;
      }
    }
  }
  return numReached == (size - numUsed);
}

int numPaths(vector <vector <int> > &adj, int i, int j, vector<bool> &used) {
  // base: we have gone through all the necessary vertices
  if (numUsed == size) {
    return 1;
  }

  // this prunes the tree when bad solutions are detected ahead of time
  if (!allReachableAndWithGoodDegree(adj, used, i, j)) {
    return 0;
  }

  int n = 0;

  int v = j + W*i;
  for (int k = 0; k < adj[v].size(); k++) {
    if (used[adj[v][k]] == false && (grid[i][j] != 3 || numUsed == size-1)) {
      used[adj[v][k]] = true; numUsed++;
      n += numPaths(adj, adj[v][k] / W, adj[v][k] % W, used);
      used[adj[v][k]] = false; numUsed--;
    }
  }
  return n;
}

int main() {

  cin >> W >> H;

  vector< vector<int> > adj (W*H); // adjacency list
  vector <bool> used (W*H, false);
  
  size = W*H;
  // the number of a vertex is j + W*i, where i is the line and j the column
  // read input into grid[][]
  for (int i = 0; i < H; i++) {
    for (int j = 0; j < W; j++) {
      cin >> grid[i][j];
      if (grid[i][j] == 2) {
	si = i; sj = j;
	used[j+W*i] = true;
	numUsed++;
      }
      else if (grid[i][j] == 3) {
	gi = i; gj = j;
      }
      else if (grid[i][j] == 1) {
	used[j+W*i] = true;
	numUsed++;
      }
    }
  }

  for (int i = 0; i < H; i++) {
    for (int j = 0; j < W; j++) {
      if (grid[i][j] != 1)
	addEdges(adj, i, j);
    }
  }

  // for (int i = 0; i < H; i++) {
  //   for (int j = 0; j < W; j++) {
  //     cout << grid[i][j] << " ";
  //   }
  //   cout << endl;
  // }
  // printEdges(adj); 

  cout << numPaths(adj, si, sj, used) << endl;
  
  return 0;
}
