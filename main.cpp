#include <iostream>
#include <vector>
#include <queue>

#include "Board.h"


using namespace std;


vector<vector<int> > level1 = {{1,2,3}, {4,5,6}, {7,8,0}};
vector<vector<int> > level2 = {{1,2,3}, {4,5,6}, {0,7,8}};
vector<vector<int> > level3 = {{1,2,3}, {5,0,6}, {4,7,8}};
vector<vector<int> > level4 = {{1,3,6}, {5,0,2}, {4,7,8}};


int main(){

  BoardState* theInitial = new BoardState(level1);
  expandStates.push(theInitial);

  //theInitial.printBoardState();
  //theInitial.findBlankSpace();
  //theInitial.legalMoves();
  //theInitial.expandBoardState();

  int solvedDepth = theInitial->solvePuzzle();

  cout << "Depth is: " << solvedDepth << endl;

  delete theInitial;

  return 0;
}