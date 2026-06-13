#ifndef ENVIRONMENT
#define ENVIRONMENT

#include "Vector.h"
using namespace mathtool;
#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum Type { EXPLORED, FRONTIER, NOTEXPLORED, PATH };
enum SearchType { BFS, ASTAR, BESTFIRST };

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class Cell {
public:
  Cell();
  void Init(int _i, int _j, Vector3d _center, double _dx, double _dy, bool _blocked);
  void Draw(bool isStart, bool isGoal);
  Vector3d GetCenter() { return Vector3d(center.GetX(),center.GetY(),0); }
  bool IsCollision(Vector3d pNew, double radius);

  int xi;
  int xj;
  double dx;
  double dy;
  Vector3d center;
  Vector3d minPt;
  Vector3d maxPt;
  bool blocked;
  bool isCol;
  //for finding path
  double tentativeDist;
  pair<int,int> parent;
  bool visited;
  Type type;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class Node {
public:
  Node(Cell* s, Node* p, double _f, double _g, double _h) { 
    state = s; 
    parent = p; 
    f_n = _f; 
    g_n = _g;
    h_n = _h;
  }
  double GetF() { return f_n; }
  double GetG() { return g_n; }
  double GetH() { return h_n; }
  
  //bool operator < (Node* cur) {
  //  return (GetF() < cur->GetF());
  //}

  Cell* state;
  Node* parent;
  double f_n;
  double g_n;
  double h_n;
};

bool myfunc(Node* a, Node*b);// { return a->GetF() < b->GetF(); }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class Environment {
public:
  Environment();
  Environment(string _inputFile, double _xmin, double _xmax, double _ymin, double _ymax );
  ~Environment();

  void Draw();

  pair<int,int> GetCellIndices(Vector3d p);
  void MakeEmptyEnv();
  void ClearPath();

  //void FindPathToRandomFreeCell(Vector3d p, vector< pair<int,int> >& cellPath);
  //void DijkstrasFindPath(pair<int,int> curCellInd, pair<int,int> goalCellInd, vector< pair<int,int> >& cellPath); 

  void FindPathFunction();

  void ResetCellValues();

  void ToggleBlockedCell(double tx, double ty);
  void SetStartCell(double tx, double ty);
  void SetGoalCell(double tx, double ty);
  void SetRandomStartGoalCells();

  void SetSearchType(SearchType st) { searchType = st; }
  void InitializeSearch();
  void Search(int iteration);
  void BFS(int iteration);
  void AStar(int iteration);

  Cell** cells;

  double xmin, xmax, ymin, ymax; 
  double dx, dy;
  int rows, columns;
  string inputFile;
  bool initialized;
 
  //actual path 
  SearchType searchType;
  vector< pair<int,int> > path;
  pair<int,int> startCell;
  pair<int,int> goalCell;

  vector< Node* > openSet;
  vector< Node* > closedSet;
};

#endif
