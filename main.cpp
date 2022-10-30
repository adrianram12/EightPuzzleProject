#include <iostream>
#include <vector>
#include <queue>
#include <ctime>

#include "Board.h"
//#include "BoardCompare.h"


using namespace std;

//struct BoardCompare;


vector<vector<int> > level1 = {{1,2,3}, {4,5,6}, {7,8,0}};
vector<vector<int> > level2 = {{1,2,3}, {4,5,6}, {0,7,8}};
vector<vector<int> > level3 = {{1,2,3}, {5,0,6}, {4,7,8}};
vector<vector<int> > level4 = {{1,3,6}, {5,0,2}, {4,7,8}};
vector<vector<int> > level5 = {{1,3,6}, {5,0,7}, {4,8,2}};
vector<vector<int> > level6 = {{1,6,7}, {5,0,3}, {4,8,2}};
vector<vector<int> > level7 = {{7,1,2}, {4,8,5}, {6,3,0}};
vector<vector<int> > level8 = {{0,7,2}, {4,6,1}, {3,5,8}};

int main(){


  int heuristic = 1;
  ActualPuzzle puzzle;
  BoardState* theInitial = new BoardState(level6);
  expandStates.push(theInitial);


  //theInitial.printBoardState();
  //theInitial.findBlankSpace();
  //theInitial.legalMoves();
  //theInitial.expandBoardState();

  clock_t startTime = clock();
  int solvedDepth = puzzle.solvePuzzle(heuristic);

  cout << "Depth is: " << solvedDepth << endl; 

  cout << "Total time: " << (clock() - startTime) / (CLOCKS_PER_SEC / 1000) << endl;
  delete theInitial;

  return 0;
}