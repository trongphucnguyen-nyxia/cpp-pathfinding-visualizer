#ifndef MACOS
#include <windows.h>
#pragma comment(lib, "user32.lib") 
#endif

#include "Environment.h"
#include <fstream>

#include <GL/glut.h>  // (or others, depending on the system in use)

bool envDebug = false; //true;//false;
///////////////////////////////////////////////////////////
Cell::Cell() {  
  blocked = false;
  isCol = false;
  type = NOTEXPLORED;
}
void Cell::Init(int _i, int _j, Vector3d _center, double _dx, double _dy, bool _blocked) {
  xi = _i;
  xj = _j;
  dx = _dx;
  dy = _dy;
  center = _center;
  minPt[0] = center[0]-dx/2.0;
  minPt[1] = center[1]-dy/2.0;
  maxPt[0] = center[0]+dx/2.0;
  maxPt[1] = center[1]+dy/2.0;
  blocked = _blocked;
  isCol = false;
  type = NOTEXPLORED;
}

void Cell::Draw(bool isStart, bool isGoal) {

  glLineWidth(1);
  if( blocked ) {
    glColor3f(0.6,0.6,0.6);
    glBegin(GL_POLYGON);
  }
  else {
    if( isStart ) {
      //cout << " cell is start i=" << xi << " j=" << xj << endl;
      glColor3f(0.9,0.0,0.0);
      glBegin(GL_POLYGON);
    }
    else if( isGoal ) {
      //cout << " cell is goal i=" << xi << " j=" << xj << endl;
      glColor3f(0.1,0.8,0.0);
      glBegin(GL_POLYGON);
    }
    else if( type == PATH ) {
      glColor3f(0.1,0.8,0.0);
      glBegin(GL_POLYGON);
    }
    else if( type == EXPLORED ) {
      glColor3f(0.0,0.0,0.9);
      glBegin(GL_POLYGON);
    }
    else if( type == FRONTIER ) {
      glColor3f(0.1,0.8,0.9);
      glBegin(GL_POLYGON);
    }
    else {
      glColor3f(0.6, 0.6, 0.6);
      glBegin(GL_LINE_LOOP);
    }
  }
  glVertex3f(minPt[0], 0, minPt[1]);
  glVertex3f(maxPt[0], 0, minPt[1]);
  glVertex3f(maxPt[0], 0, maxPt[1]);
  glVertex3f(minPt[0], 0, maxPt[1]);
  glEnd();
}

bool Cell::IsCollision(Vector3d pNew, double radius) {
  if( (pNew[0]<(center.GetX()+dx/2+radius)) &&
      (pNew[0]>(center.GetX()-dx/2-radius)) &&
      (pNew[1]<(center.GetY()+dy/2+radius)) &&
      (pNew[1]>(center.GetY()-dy/2-radius)) ) {
    //cout << "pNew=" << pNew << " radius=" << radius << " dx= " << dx << " dy=" << dy << endl;
    //cout << "condition1=" << (center.GetX()+dx+radius) << endl;
    //cout << "condition2=" << (center.GetX()-dx-radius) << endl;
    //cout << "condition3=" << (center.GetY()+dy+radius) << endl;
    //cout << "condition4=" << (center.GetY()-dy-radius) << endl;
    isCol = true;
    return true;
  }
  return false;
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
bool myfunc(Node* a, Node*b) { return a->GetF() < b->GetF(); }
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
Environment::Environment() {
  searchType = SearchType::ASTAR;
  initialized = false;
  startCell.first = -1;
  startCell.second = -1;
  goalCell.first = -1;
  goalCell.second = -1;
}

Environment::Environment(string _inputFile, double _xmin, double _xmax, double _ymin, double _ymax ) {
  searchType = SearchType::ASTAR;
  inputFile = _inputFile;

  xmin = _xmin;
  xmax = _xmax;
  ymin = _ymin;
  ymax = _ymax;
  cout << " Environment boundaries: [" << xmin << ", " << xmax << " | " << ymin << ", " << ymax << "]"<< endl;

  cout << "Attempting to load environment file: " << inputFile << endl;
  ifstream infile;
  infile.open(inputFile.c_str());
  if( !infile ) {
    cout << "Could NOT open infile: " << inputFile << endl;
	//exit(-1);
  }
  if(infile >> rows) {
    cout << "There will be " << rows << " rows ";
  }
  if(infile >> columns) {
    cout << "There will be " << columns << " rows ";
  }
  cells = new Cell*[rows];
  for(int i=0; i<rows; i++) {
    cells[i] = new Cell[columns];
  }
  dx = (_xmax-_xmin)/columns;
  dy = (_ymax-_ymin)/rows;
  string colData;
  string tmpStr;
  getline(infile, tmpStr); //just to get rid of carriage return
  int rowIndex=0;
  while( getline(infile, colData) ) {
    //cout << rowIndex << " going to process '"<<colData<<"'"<<endl;
    for(int j=0; j<(int)colData.length(); j++) {
      Cell& cell = cells[rowIndex][j];
      bool isBlocked = (colData.at(j)=='x'||colData.at(j)=='X')? true: false;
      Vector3d center(xmin+ dx/2+dx*j,ymin+ dy/2+dy*rowIndex,0);
      //cout << "Initializing cell: " << rowIndex << ", " << i << " center: " << center.GetX() << " " << center.GetY() << " dx " << dx << " dy " << dy << " isBlocked " << isBlocked << endl;
      cell.Init(rowIndex,j,center,dx,dy,isBlocked);
    }
    rowIndex++;
  }
  infile.close();
  initialized = true;
  SetRandomStartGoalCells();
}

Environment::~Environment() {
  startCell.first = -1;
  InitializeSearch();
  delete [] cells;
}

void Environment::Draw() {
  if (!initialized) return;
  for(int i=0; i<rows; i++) 
    for(int j=0; j<columns; j++) {
      bool isStart = false;
      bool isGoal = false;
      if( startCell.first == i && startCell.second == j ) isStart = true;
      if( goalCell.first == i && goalCell.second == j ) isGoal = true;
      cells[i][j].Draw(isStart, isGoal);
    }
}
pair<int,int> Environment::GetCellIndices(Vector3d p) {
  if(envDebug) cout << "Env::GCI p= " << p << " [" << xmin << "," << xmax << "|" << ymin << "," << ymax << "] dx="<<dx<<" dy="<<dy<< endl;
  int j = int((p[0]-xmin)/dx);
  int i = int((p[1]-ymin)/dy);
  //cout << "1i " << int((p[0]-xmin)/(xmax-xmin)) << " j " << j << endl;
  if( i<0 ) i = 0;
  if( j<0 ) j = 0;
  if( j>=columns) j = columns-1;
  if( i>=rows ) i = rows-1;
  //cout << "2i " << i << " j " << j << endl;
  return make_pair(i,j);
}

void Environment::MakeEmptyEnv() {
  for(int i=0; i<rows; i++) {
    for(int j=0; j<columns; j++) {
      cells[i][j].blocked = false;
    }
  }
}
void Environment::ClearPath() {
  path.clear();
  for(int i=0; i<rows; i++) {
    for(int j=0; j<columns; j++) {
      cells[i][j].type = NOTEXPLORED;
    }
  }
}

void Environment::ToggleBlockedCell(double tx, double ty) {
  Vector3d p(tx,ty,0);
  pair<int,int> cellInd = GetCellIndices(p);
  cout << "Cell indices = " << cellInd.first << "," << cellInd.second << endl;
  cells[cellInd.first][cellInd.second].blocked = !cells[cellInd.first][cellInd.second].blocked;
}
void Environment::SetStartCell(double tx, double ty) {
  Vector3d p(tx,ty,0);
  pair<int,int> cellInd = GetCellIndices(p);
  cout << "Cell indices = " << cellInd.first << "," << cellInd.second << " now start. " << endl;
  startCell = cellInd;
}
void Environment::SetGoalCell(double tx, double ty) {
  Vector3d p(tx,ty,0);
  pair<int,int> cellInd = GetCellIndices(p);
  cout << "Cell indices = " << cellInd.first << "," << cellInd.second << " now goal. " << endl;
  goalCell = cellInd;
}

void Environment::SetRandomStartGoalCells() {
    bool inBlockedCell = false;
    int r_i;
    int r_j;
    do {
      r_i = rand() % rows;
      r_j = rand() % columns;
      inBlockedCell = cells[r_i][r_j].blocked;
    } while( inBlockedCell );

    startCell = make_pair(r_i, r_j);
    do {
      r_i = rand() % rows;
      r_j = rand() % columns;
      inBlockedCell = cells[r_i][r_j].blocked;
    } while( inBlockedCell );

    goalCell = make_pair(r_i, r_j);
}

void Environment::FindPathFunction() {
  if(path.size() == 0) {
    //find a random grid cell
    bool inBlockedCell = false;
    int r_i;
    int r_j;
    do {
      r_i = rand() % rows;
      r_j = rand() % columns;
      inBlockedCell = cells[r_i][r_j].blocked;
    } while( inBlockedCell );

    pair<int,int> startCell = make_pair(r_i, r_j);
    do {
      r_i = rand() % rows;
      r_j = rand() % columns;
      inBlockedCell = cells[r_i][r_j].blocked;
    } while( inBlockedCell );

    pair<int,int> goalCell = make_pair(r_i, r_j);
    //DijkstrasFindPath(startCell, goalCell, path);
    for(int i=0; i<path.size(); i++) {
      pair<int,int>& curCellInd = path[i];
      cells[curCellInd.first][curCellInd.second].type = PATH;
    }
  }
  else {
    cout << "path exists, want to find a new path, clear it out!" << endl;
  }
}

void Environment::ResetCellValues() {
  for(int i=0; i<rows; i++) {
    for(int j=0; j<columns; j++) {
      cells[i][j].tentativeDist = 1e7;
      cells[i][j].parent = make_pair(-1,-1);
      cells[i][j].visited = false;
    }
  }
}

void Environment::InitializeSearch() {
  ClearPath();
  while(!openSet.empty()) {
    Node* tn = openSet[0];
    openSet.erase(openSet.begin());
    delete tn;
  }
  while(!closedSet.empty()) {
    Node* tn = closedSet[0];
    closedSet.erase(closedSet.begin());
    delete tn;
  }
  if( startCell.first != -1 && startCell.second != -1 &&
      goalCell.first != -1 && goalCell.second != -1 ) {
    Cell* initialState = &(cells[startCell.first][startCell.second]);
    Cell* goalState = &(cells[goalCell.first][goalCell.second]);
    double g = 0;
    double h = (goalState->GetCenter()-initialState->GetCenter()).norm();
    double f = g+h;
    Node* nn = new Node(initialState, NULL, f, g, h);
    openSet.push_back(nn);
  }
}

void Environment::Search(int iteration) {
  if( searchType == SearchType::BFS ) {
    //cout << "BFS iteration=" << iteration << endl;
    BFS(iteration);
  }
  else if( searchType == SearchType::ASTAR ) {
    //cout << "AStar iteration=" << iteration << endl;
    AStar(iteration);
  }
  else if( searchType == SearchType::BESTFIRST ) {
    //cout << "BestFirst iteration=" << iteration << endl;
  }
  else {
    cout << "Unknown search type: " << searchType << endl;
  }
}

void PrintSet(vector<Node*>& thisSet, string tag) {
  cout << tag << " size: " << thisSet.size() << endl;
  for(int i=0; i<(int)thisSet.size(); i++) {
    cout << "i=" << i << " [";
    Cell* cur = thisSet[i]->state;
    cout << cur->xi << "," << cur->xj << "|"; 
    Node* parent = thisSet[i]->parent;
    if(parent != NULL) {
    Cell* par = parent->state;
    cout << par->xi << "," << par->xj; 
    }
    cout << "|"<<thisSet[i]->GetF()<<","<<thisSet[i]->GetG()<<","<<thisSet[i]->GetH();
    cout << "] ";
  }
}

bool InSet(vector<Node*>& thisSet, Cell* c, int& setIndex) {
  for(int i=0; i<(int)thisSet.size(); i++) {
    Cell* cur = thisSet[i]->state;
    if( cur->xi == c->xi &&
	cur->xj == c->xj ) {
      setIndex = i;
      return true;
    }
  }
  return false;
}

void Environment::BFS(int iteration) {
  if( path.empty() ) {
    //cout << "BFS iteration=" << iteration << " size of openSet=" << openSet.size() << " size of closedSet=" << closedSet.size() << endl;
    //path is empty, so I should find one if I can
    bool foundPath = false;
    Node* pathEnd;

    int iterN = 0;
    while( iterN < iteration && !openSet.empty() && !foundPath ) {
      //PrintSet(openSet,"openSet");
      Node* curNode = openSet[0];
      //cout << "curNode i=" << curNode->state->xi << ", j=" << curNode->state->xj << endl;
      Cell* cur = curNode->state;
      openSet.erase(openSet.begin());
      cur->type = EXPLORED;
      closedSet.push_back( curNode );
      
      for(int i=cur->xi-1; (i<=cur->xi+1) && !foundPath; i++) {
	for(int j=cur->xj-1; (j<=cur->xj+1) && !foundPath; j++) {
	  if( i==cur->xi && j==cur->xj ) continue; //this will be cur node
	  if( i<0 || i>=rows ) continue; //out of bounds
	  if( j<0 || j>=columns ) continue; //out of bounds
          if( cells[i][j].blocked ) continue;//don't add blocked cells

	  if( i!=cur->xi && j!=cur->xj ) { //disallow diagonal moves in certain cases
	    continue; //disallow diagonal moves
	    //if( cells[cur->xi][j].blocked ) continue;
	    //if( cells[i][cur->xj].blocked ) continue;
	  }
	  Cell* nextState = &(cells[i][j]);
	  int setIndex = -1;
	  bool inClosed = InSet(closedSet, nextState, setIndex);
	  bool inOpen   = InSet(openSet,   nextState, setIndex);
	  if( !inOpen && !inClosed ) {
	    //check goal
	    if( i == goalCell.first && j == goalCell.second ) {
	      //cout << "inside goal check...found path. " << endl;
	      foundPath = true;
	      pathEnd = curNode;
	    }
	    //insert into openSet/frontier
	    openSet.push_back( new Node(nextState, curNode, 0,0,0) );
	    nextState->type = FRONTIER;
	    //cout << "making frontier. i="<<i<< " j=" << j << " nextState i="<<nextState->xi <<" j=" <<nextState->xj << endl;
	  }
	}//endfor j
      }//endfor i

      iterN++;
    }//endwhile

    //cout << "--done! BFS iteration=" << iteration << " size of openSet=" << openSet.size() << " size od closedSet=" << closedSet.size() << endl;
    if( foundPath ) {
      //cout << "found path...backtracking. " << endl;
      path.clear();
      //backtrack
      int z=0;
      while( pathEnd != NULL ) {
	//cout << "backtracking z=" << z << endl;
	Cell* c = pathEnd->state;
	c->type = PATH;
	path.push_back( make_pair(c->xi, c->xj) );
	pathEnd = pathEnd->parent;
	z++;
      }//endwhile

    }

  }
}

void Environment::AStar(int iteration) {
  bool foundPath = false;
  if( path.empty() ) {
    //cout << "AStar iteration=" << iteration << " size of openSet=" << openSet.size() << " size of closedSet=" << closedSet.size() << endl;
    //path is empty, so I should find one if I can
    Node* pathEnd;

    int iterN = 0;
    while( iterN < iteration && !openSet.empty() && !foundPath ) {
	      
	      
	      //double oldG = curNode->GetG();
	      //double g = oldG + (cur->GetCenter()-nextState->GetCenter()).norm();


	      //need goal cell
	      //Cell* goalC = &(cells[goalCell.first][goalCell.second]);
	      //double h = (nextState->GetCenter()-goalC->GetCenter()).norm();
	      
	      
	//sort(openSet.begin(), openSet.end(), myfunc);

	iterN++;
      }
    }//endwhile

    //cout << "--done! AStar iteration=" << iteration << " size of openSet=" << openSet.size() << " size od closedSet=" << closedSet.size() << endl;
    if( foundPath ) {
    }

}
