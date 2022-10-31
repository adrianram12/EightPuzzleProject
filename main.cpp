#include <ctime>
#include <iostream>
#include <queue>
#include <vector>

//#include "Board.h"

using namespace std;

int numNodesExpanded = 0;


class BoardState;

priority_queue<BoardState *> visitedStates;

vector<vector<int>> goalState = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};

// create the node for each generated board state
class BoardState {

public:
  vector<vector<int>> board;
  int xcoordinate;
  int ycoordinate;

  int gnCost = 0;
  int fnCost = 0;
  int hnCost = 0;

  BoardState *parent = nullptr;
  BoardState *child = nullptr;

  void expandBoardState(int);
  int solvePuzzle(int);

  BoardState() {
    this->findBlankSpace();
    gnCost = 0;
    fnCost = 0;
    hnCost = 0;
  }

  BoardState(vector<vector<int>> x) {
    this->board = x;

    this->findBlankSpace();

    gnCost = 0;
    fnCost = 0;
    hnCost = 0;
  }

  BoardState(BoardState &copyConstr) {

    xcoordinate = copyConstr.xcoordinate;
    ycoordinate = copyConstr.ycoordinate;

    gnCost = copyConstr.gnCost;
    fnCost = copyConstr.fnCost;
    board = copyConstr.board;

    parent = new BoardState();
    *parent = copyConstr.parent;

    child = new BoardState(); //ADDED THISSSSS
    *child = copyConstr.child; //ADDED THISSSSS
  }

  BoardState operator=(BoardState *overload) {

    BoardState newBoard;

    newBoard.xcoordinate = overload->xcoordinate;
    newBoard.ycoordinate = overload->ycoordinate;
    newBoard.gnCost = overload->gnCost;
    newBoard.fnCost = overload->fnCost;
    newBoard.board = overload->board;
    newBoard.parent = overload->parent;
    newBoard.child = overload->child; // ADDED THISSSSSSSSS

    return *this;
  }

  int MisplacedTile() {

    int misplacedTiles = 0;

    for (int i = 0; i < this->board.size(); i++) {
      for (int j = 0; j < this->board.size(); j++) {

        if ((this->board.at(i).at(j) != 0) &&
            (this->board.at(i).at(j) != goalState.at(i).at(j))) {

          misplacedTiles = misplacedTiles + 1;
        }
      }
    }

    return misplacedTiles;
  }

  int ManhattanDistance() {
    int manhattanD = 0;
    int x1Coordinate = 0;
    int x2Coordinate = 0;
    int y1Coordinate = 0;
    int y2Coordinate = 0;

    for (int i = 0; i < this->board.size(); i++) {
      for (int j = 0; j < this->board.size(); j++) {

        if ((this->board.at(i).at(j) != 0) &&
            (this->board.at(i).at(j) != goalState.at(i).at(j))) {

          x1Coordinate = i;
          y1Coordinate = j;

          for (int k = 0; k < goalState.size(); k++) {
            for (int l = 0; l < goalState.size(); l++) {

              if (this->board.at(x1Coordinate).at(y1Coordinate) ==
                  goalState.at(k).at(l)) {
                x2Coordinate = k;
                y2Coordinate = l;
                break;
              }
            }
          }

          manhattanD = abs(x1Coordinate - x2Coordinate) +
                       abs(y1Coordinate - y2Coordinate) + manhattanD;
        }
      }
    }

    return manhattanD;
  }

  void findBlankSpace() {

    for (unsigned int i = 0; i < 3; i++) {

      for (unsigned int j = 0; j < 3; j++) {

        if (this->board.at(i).at(j) == 0) {
          this->xcoordinate = i;
          this->ycoordinate = j;
        }
      }
    }
  }

  vector<vector<int>> legalMoves() {

    vector<vector<int>> potentialMoves = {{xcoordinate - 1, ycoordinate},
                                          {xcoordinate + 1, ycoordinate},
                                          {xcoordinate, ycoordinate - 1},
                                          {xcoordinate, ycoordinate + 1}};

    vector<vector<int>> validMoves;

    for (unsigned int i = 0; i < potentialMoves.size(); i++) {

      if ((potentialMoves.at(i).at(0) >= 0 &&
           potentialMoves.at(i).at(0) <= 2) &&
          (potentialMoves.at(i).at(1) >= 0 &&
           potentialMoves.at(i).at(1) <= 2)) {

        validMoves.push_back(potentialMoves.at(i));
      }
    }

    return validMoves;
  }

  void printPuzzleState() {

    for (unsigned int i = 0; i < this->board.size(); i++) {
      for (unsigned int j = 0; j < this->board.size(); j++) {

        cout << this->board.at(i).at(j) << " ";
      }
      cout << endl;
    }
    cout << endl;
    cout << endl;
  }

  void printSolution(BoardState *final) {
    while (final->child) { // final->parent
      cout << "The best state to expand with a g(n) = " << final->gnCost
           << " and h(n) of " << final->hnCost << " is?" << endl;
      final->printPuzzleState();
      final = final->child; // final = final->parent
    }
  }
};

BoardState* firstBoard;
struct Comparison {

  bool operator()(BoardState *lhs, BoardState *rhs) {

    return lhs->fnCost > rhs->fnCost;
  }
};

priority_queue<BoardState *, vector<BoardState *>, Comparison> expandStates;

int maxQueueSize = expandStates.size();

void BoardState::expandBoardState(int heuristic) {

  vector<vector<int>> validMoves = this->legalMoves();

  for (int i = 0; i < validMoves.size(); i++) {

    bool flag = false;

    BoardState *addChild = new BoardState(this->board); // deep copy

    int temp =
        addChild->board.at((validMoves.at(i).at(0))).at(validMoves.at(i).at(1));

    addChild->board.at((validMoves.at(i).at(0))).at(validMoves.at(i).at(1)) =
        addChild->board.at(this->xcoordinate).at(this->ycoordinate);

    addChild->board.at(xcoordinate).at(ycoordinate) = temp;

    if (heuristic == 0) {
      fnCost = this->gnCost + 1;
      hnCost = 0;
    } else if (heuristic == 1) {
      hnCost = MisplacedTile();
      fnCost = this->gnCost + hnCost;
    }

    else if (heuristic == 2) {
      hnCost = ManhattanDistance();
      fnCost = this->gnCost + hnCost;
    }

    BoardState *newBoard = new BoardState(addChild->board);
    newBoard->gnCost = this->gnCost + 1;
    newBoard->fnCost = fnCost;
    newBoard->hnCost = hnCost;

    newBoard->parent = this;
    this->child =
        newBoard; // ADDED THISSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS

    priority_queue<BoardState *> tempQ = visitedStates;

    while (!(tempQ.empty())) {

      if (newBoard->board == tempQ.top()->board) {
        flag = true;
        break;
      } else {
        flag = false;
        tempQ.pop();
      }
    }

    if (!flag) {

      expandStates.push(newBoard);

      if(expandStates.size() > maxQueueSize){
      maxQueueSize = expandStates.size();
      }
    }
  }
}

int BoardState::solvePuzzle(int heuristic) {

  while (expandStates.size() != 0) {

    BoardState *topNode = expandStates.top();
    expandStates.pop();
    numNodesExpanded++;

    visitedStates.push(topNode);

    if (topNode->board == goalState) {

      this->printSolution(firstBoard);
      return topNode->fnCost;
    }
    topNode->expandBoardState(heuristic);
  }
}

vector<vector<int>> level1 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}}; // depth: 0
vector<vector<int>> level2 = {{1, 2, 3}, {4, 5, 6}, {0, 7, 8}}; // depth: 2
vector<vector<int>> level3 = {{1, 2, 3}, {5, 0, 6}, {4, 7, 8}}; // depth: 4
vector<vector<int>> level4 = {{1, 3, 6}, {5, 0, 2}, {4, 7, 8}}; // depth: 8
vector<vector<int>> level5 = {{1, 3, 6}, {5, 0, 7}, {4, 8, 2}}; // depth: 12
vector<vector<int>> level6 = {{1, 6, 7}, {5, 0, 3}, {4, 8, 2}}; // depth: 16
vector<vector<int>> level7 = {{7, 1, 2}, {4, 8, 5}, {6, 3, 0}}; // depth: 20
vector<vector<int>> level8 = {{0, 7, 2}, {4, 6, 1}, {3, 5, 8}}; // depth: 24

int main() {

  int userInput;
  int counter = 0;
  int heuristic;
  vector<vector<int>> userPuzzle(3, vector<int>(3));
  cout << "Welcome to the 8-Puzzle solver. To create your own puzzle, type "
          "'1'. To use a predefined puzzle, type '2'."
       << endl;

  cin >> userInput;

  if (userInput == 1) {

    int numbers;
    cout << endl;
    cout << "Type in three numbers for the first row. Valid numbers are 1 "
            "through 8 inclusive. Type '0' to indicate the blank space. Make "
            "sure to leave a space between each number. Press the \"Enter\" "
            "button when finished."
         << endl;

    while (counter < 3) {

      cin >> numbers;
      userPuzzle.at(0).at(counter) = numbers;
      counter++;
    }

    numbers = 0;
    counter = 0;

    cout << endl;
    cout << "Type in three numbers for the second row. Valid numbers are 1 "
            "through 8 inclusive. Type '0' to indicate the blank space. Make "
            "sure to leave a space between each number. Press the \"Enter\" "
            "button when finished."
         << endl;

    while (counter < 3) {

      cin >> numbers;
      userPuzzle.at(1).at(counter) = numbers;
      counter++;
    }

    numbers = 0;
    counter = 0;

    cout << endl;
    cout << "Type in three numbers for the third row. Valid numbers are 1 "
            "through 8 inclusive. Type '0' to indicate the blank space. Make "
            "sure to leave a space between each number. Press the \"Enter\" "
            "button when finished."
         << endl;

    while (counter < 3) {

      cin >> numbers;
      userPuzzle.at(2).at(counter) = numbers;
      counter++;
    }

    cout << endl;
    cout << "Here is your puzzle." << endl;

    for (unsigned int i = 0; i < userPuzzle.size(); i++) {
      for (unsigned int j = 0; j < userPuzzle.size(); j++) {

        cout << userPuzzle.at(i).at(j) << " ";
      }
      cout << endl;
    }
    cout << endl;
    cout << endl;

    cout << "Please select the algorithm you would like to use to solve your "
            "puzzle. Type '1' for Uniform Cost Search. Type '2' for A* with "
            "the Misplaced Tile heuristic. Type '3' for A* with the Manhattan "
            "Distance heuristic."
         << endl;

    cout << endl;

    int choiceAlg;
    cin >> choiceAlg;

    if (choiceAlg == 1) {
      heuristic = 0;
    } else if (choiceAlg == 2) {
      heuristic = 1;
    } else if (choiceAlg == 3) {
      heuristic = 2;
    }
  }

  else if (userInput == 2) {

    int levelDifficulty;

    cout << "Please indicate the level of difficulty of the puzzle you would "
            "like to solve. Valid levels are 1 through 8 inclusive."
         << endl;
    cin >> levelDifficulty;
    cout << endl;

    switch (levelDifficulty) {

    case 1:
      userPuzzle = level1;
      break;

    case 2:
      userPuzzle = level2;
      break;

    case 3:
      userPuzzle = level3;
      break;

    case 4:
      userPuzzle = level4;
      break;

    case 5:
      userPuzzle = level5;
      break;

    case 6:
      userPuzzle = level6;
      break;

    case 7:
      userPuzzle = level7;
      break;

    case 8:
      userPuzzle = level8;
      break;

    default:
      userPuzzle = level1;
      break;
    }

    cout << "Please select the algorithm you would like to use to solve your "
            "puzzle. Type '1' for Uniform Cost Search. Type '2' for A* with "
            "the Misplaced Tile heuristic. Type '3' for A* with the Manhattan "
            "Distance heuristic."
         << endl;

    cout << endl;

    int choiceAlg;
    cin >> choiceAlg;

    if (choiceAlg == 1) {
      heuristic = 0;
    } else if (choiceAlg == 2) {
      heuristic = 1;
    } else if (choiceAlg == 3) {
      heuristic = 2;
    }
  }

  // int heuristic = 2;
  // ActualPuzzle puzzle;
  BoardState *theInitial = new BoardState(userPuzzle);
  firstBoard = theInitial;
  expandStates.push(theInitial);

  clock_t startTime = clock();
  int solvedDepth = theInitial->solvePuzzle(heuristic); // puzzle.solvePuzzle

  cout << "Goal State!" << endl;
  cout << "Solution depth was " << solvedDepth << endl;
  cout << "Number of nodes expanded: " << numNodesExpanded << endl;
  cout << "Max queue size: " << maxQueueSize << endl;

  cout << "Total time: " << ((clock() - startTime) / (double)CLOCKS_PER_SEC)
       << " seconds" << endl;
  delete theInitial;

  return 0;
}
