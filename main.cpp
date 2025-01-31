#include <ctime>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

//keeps track of the total number of nodes expanded by the search algorithm
int numNodesExpanded = 0;


class BoardState;

//priority queue that keeps track of visited states in order to eliminate repeated expansions
priority_queue<BoardState *> visitedStates;


//2D vector that is hardcoded to be the expected Goal State used for comparison of each board configuration
vector<vector<int>> goalState = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};

//BaordState class creates the node for each generated board configuration
class BoardState {

public:

  //keeps track of the current board configuration
  vector<vector<int>> board;

  //both hold the coordinates of the location of the blank space
  int xcoordinate;
  int ycoordinate;

  //variables used to keep track of f(n), g(n), and h(n)
  int gnCost = 0;
  int fnCost = 0;
  int hnCost = 0;

  //keeps track of the parent of each board configuration
  BoardState *parent = nullptr;

  void expandBoardState(int);
  int solvePuzzle(int);

  //Default Constructor for Board Configuration
  BoardState() {
    this->findBlankSpace();
    gnCost = 0;
    fnCost = 0;
    hnCost = 0;
  }

  //Parameterized Constructor for Board Configuration
  BoardState(vector<vector<int>> x) {
    this->board = x;

    this->findBlankSpace();

    gnCost = 0;
    fnCost = 0;
    hnCost = 0;
  }


  //Copy Constructor used to create a Deep Copy of a node/BaordState
  BoardState(BoardState &copyConstr) {

    xcoordinate = copyConstr.xcoordinate;
    ycoordinate = copyConstr.ycoordinate;

    gnCost = copyConstr.gnCost;
    fnCost = copyConstr.fnCost;
    board = copyConstr.board;

    parent = new BoardState();
    *parent = copyConstr.parent;
  }

  //Overload operator that copies the contents of one BoardState into another
  BoardState operator=(BoardState *overload) {

    BoardState newBoard;

    newBoard.xcoordinate = overload->xcoordinate;
    newBoard.ycoordinate = overload->ycoordinate;
    newBoard.gnCost = overload->gnCost;
    newBoard.fnCost = overload->fnCost;
    newBoard.board = overload->board;
    newBoard.parent = overload->parent;

    return *this;
  }

  //function that calculates the number of misplaced tiles in a board configuration
  int MisplacedTile() {

    int misplacedTiles = 0;

    for (int i = 0; i < this->board.size(); i++) {
      for (int j = 0; j < this->board.size(); j++) {

        if ((this->board.at(i).at(j) != 0) && (this->board.at(i).at(j) != goalState.at(i).at(j))) {

          misplacedTiles = misplacedTiles + 1;
        }
      }
    }

    return misplacedTiles;
  }

  //function that calculates the Manhattan Distance of a board configuration
  int ManhattanDistance() {
    int manhattanD = 0;
    int x1Coordinate = 0;
    int x2Coordinate = 0;
    int y1Coordinate = 0;
    int y2Coordinate = 0;

    for (int i = 0; i < this->board.size(); i++) {
      for (int j = 0; j < this->board.size(); j++) {

        if ((this->board.at(i).at(j) != 0) && (this->board.at(i).at(j) != goalState.at(i).at(j))) {

          x1Coordinate = i;
          y1Coordinate = j;

          for (int k = 0; k < goalState.size(); k++) {
            for (int l = 0; l < goalState.size(); l++) {

              if (this->board.at(x1Coordinate).at(y1Coordinate) == goalState.at(k).at(l)) {
                x2Coordinate = k;
                y2Coordinate = l;
                break;
              }
            }
          }

          manhattanD = abs(x1Coordinate - x2Coordinate) + abs(y1Coordinate - y2Coordinate) + manhattanD;
        }
      }
    }

    return manhattanD;
  }


  //function that finds the location of the blank space and holds its coordinates
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

  //function that calculates all the possible moves in a specific board configuration
  vector<vector<int>> legalMoves() {

    vector<vector<int>> potentialMoves = {{xcoordinate - 1, ycoordinate},
                                          {xcoordinate + 1, ycoordinate},
                                          {xcoordinate, ycoordinate - 1},
                                          {xcoordinate, ycoordinate + 1}};

    vector<vector<int>> validMoves;

    for (unsigned int i = 0; i < potentialMoves.size(); i++) {

      //checks to see if the coordinates generated are within bounds of the 3x3 board
      if ((potentialMoves.at(i).at(0) >= 0 &&
           potentialMoves.at(i).at(0) <= 2) &&
          (potentialMoves.at(i).at(1) >= 0 &&
           potentialMoves.at(i).at(1) <= 2)) {

        validMoves.push_back(potentialMoves.at(i));
      }
    }

    return validMoves;
  }

  //function that prints the current board configuration
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

  //function that prints the traceback of the solution to the solved puzzle
  //function also prints out the current state's g(n) and h(n)
  void printSolution(BoardState *final){

    vector<BoardState*> listNodes;

    while(final->parent){

      listNodes.push_back(final);
      final = final->parent;
    }

    for(int i = listNodes.size() - 1; i >= 0; i--){
      cout << "The best state to expand with a g(n) = " << listNodes.at(i)->parent->gnCost
            << " and h(n) of " << listNodes.at(i)->parent->hnCost << " is?" << endl;

        listNodes.at(i)->parent->printPuzzleState();

    }

    //prints out the expected goal state after traversing through the traceback
    cout << "1 2 3" << endl;
    cout << "4 5 6" << endl;
    cout << "7 8 0" << endl;

    cout << endl;

  }
};


//struct used to compare each BoardState object's total cost, f(n), in order to put it
//in the correct location in the priority queue
struct Comparison {

  bool operator()(BoardState *lhs, BoardState *rhs) {

    return lhs->fnCost > rhs->fnCost;
  }
};

//priority queue used for holding the nodes that are waiting to be expanded
priority_queue<BoardState *, vector<BoardState *>, Comparison> expandStates;

//holds the maximum queue size of the expandStates priority queue
int maxQueueSize = expandStates.size();

//function that expands the current BoardState object's children
void BoardState::expandBoardState(int heuristic) {

  //first calls legalMoves function in order to determine all possbile moves
  vector<vector<int>> validMoves = this->legalMoves();

  for (int i = 0; i < validMoves.size(); i++) {

    bool flag = false;

    //creates a Deep Copy of the current BoardState object
    BoardState *addChild = new BoardState(this->board);

    //this section of code swaps location of one tile with the blank space
    int temp =
        addChild->board.at((validMoves.at(i).at(0))).at(validMoves.at(i).at(1));

    addChild->board.at((validMoves.at(i).at(0))).at(validMoves.at(i).at(1)) =
        addChild->board.at(this->xcoordinate).at(this->ycoordinate);

    addChild->board.at(xcoordinate).at(ycoordinate) = temp;


    //if heuristic is 0, then perform Uniform Cost Search
    if (heuristic == 0) {
      fnCost = this->gnCost + 1;
      hnCost = 0;
    } 
    
    //if heurisitc is 1, then perform A* with Misplaced Tile Heuristic
    else if (heuristic == 1) {
      hnCost = MisplacedTile();
      fnCost = this->gnCost + hnCost;
    }

    //if heuristic is 2, then perform A* with Manhattan Distance Heuristic
    else if (heuristic == 2) {
      hnCost = ManhattanDistance();
      fnCost = this->gnCost + hnCost;
    }

    //copy f(n), g(n), and h(n) of parent into child's costs
    BoardState *newBoard = new BoardState(addChild->board);
    newBoard->gnCost = this->gnCost + 1;
    newBoard->fnCost = fnCost;
    newBoard->hnCost = hnCost;

    //assigns child's parent
    newBoard->parent = this;
  
    priority_queue<BoardState *> tempQ = visitedStates;

    //while loop ensures that the child node currently being
    //examined is not already a visited state
    while (!(tempQ.empty())) {

      //if already a visited state, move on to the next child
      if (newBoard->board == tempQ.top()->board) {
        flag = true;
        break;
      } 
      
      //if top node is not the same as child, move on to the next 
      //node in visitedStates to compare it
      else {
        flag = false;
        tempQ.pop();
      }
    }

    //if child was not found in visitedStates, add it to the
    //expandStates priority queue
    if (!flag) {

      expandStates.push(newBoard);

      //update the maximum queue size only when necessary
      if(expandStates.size() > maxQueueSize){
        maxQueueSize = expandStates.size();
      }
    }
  }
}

//function that begins the General Search Algorithm
int BoardState::solvePuzzle(int heuristic) {

  while (expandStates.size() != 0) {

    BoardState *topNode = expandStates.top();
    expandStates.pop();

    //updates the number of nodes that have been expanded 
    numNodesExpanded++; 

    visitedStates.push(topNode);

    //if we found our goal state, print the traceback as well as
    //the total cost f(n)
    if (topNode->board == goalState) {

      this->printSolution(topNode);
      return topNode->fnCost;
    }

    //if node is not our goal state, push it into
    //expandedStates in order to expand it
    topNode->expandBoardState(heuristic);
  }

  //if no solution is possible, print error message
  // and return a f(n) equal to 0
  if(expandStates.size() == 0){

    cout << "Error. No solution found." << endl;
    return 0;
  }
}


//2D vectors that are hardcoded in order to test the functionality of program
vector<vector<int>> level1 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}}; // depth: 0
vector<vector<int>> level2 = {{1, 2, 3}, {4, 5, 6}, {0, 7, 8}}; // depth: 2
vector<vector<int>> level3 = {{1, 2, 3}, {5, 0, 6}, {4, 7, 8}}; // depth: 4
vector<vector<int>> level4 = {{1, 3, 6}, {5, 0, 2}, {4, 7, 8}}; // depth: 8
vector<vector<int>> level5 = {{1, 3, 6}, {5, 0, 7}, {4, 8, 2}}; // depth: 12
vector<vector<int>> level6 = {{1, 6, 7}, {5, 0, 3}, {4, 8, 2}}; // depth: 16
vector<vector<int>> level7 = {{7, 1, 2}, {4, 8, 5}, {6, 3, 0}}; // depth: 20
vector<vector<int>> level8 = {{0, 7, 2}, {4, 6, 1}, {3, 5, 8}}; // depth: 24


//main() prints out the user interface
int main() {

  int userInput;
  int counter = 0;
  int heuristic;
  vector<vector<int>> userPuzzle(3, vector<int>(3));
  cout << "Welcome to the 8-Puzzle solver. To create your own puzzle, type "
          "'1'. To use a predefined puzzle, type '2'."
       << endl;

  cin >> userInput;


  //if input is 1, take in user's desired initial board configuration
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

    //prints out the user's desired board configuration
    for (unsigned int i = 0; i < userPuzzle.size(); i++) {
      for (unsigned int j = 0; j < userPuzzle.size(); j++) {

        cout << userPuzzle.at(i).at(j) << " ";
      }
      cout << endl;
    }
    cout << endl;
    cout << endl;

    cout << "Please select the algorithm you would like to use to solve your puzzle." << endl;
    cout << "Type '1' for Uniform Cost Search." << endl;
    cout << "Type '2' for A* with the Misplaced Tile heuristic." << endl;
    cout << "Type '3' for A* with the Manhattan Distance heuristic." << endl;


    cout << endl;

    int choiceAlg;
    cin >> choiceAlg;

    //if input is 1, perform Uniform Cost Search
    if (choiceAlg == 1) {
      heuristic = 0;
    } 

    //if input is 2, perform A* with Misplaced Tile Heuristic
    else if (choiceAlg == 2) {
      heuristic = 1;
    } 
    
    //if input is 3, perform A* with Manhattan Distance Heuristic
    else if (choiceAlg == 3) {
      heuristic = 2;
    }
  }


  //if input is 2, use a default board configuration that has already been hardcoded
  else if (userInput == 2) {

    int levelDifficulty;

    cout << "Please indicate the level of difficulty of the puzzle you would "
            "like to solve. Valid levels are 1 through 8 inclusive."
         << endl;
    cin >> levelDifficulty;
    cout << endl;

    //chooses a board based on the level of difficulty chosen by the user
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

    cout << "Here is your puzzle." << endl;

    //prints out the default board configuration
    for (unsigned int i = 0; i < userPuzzle.size(); i++) {
      for (unsigned int j = 0; j < userPuzzle.size(); j++) {

        cout << userPuzzle.at(i).at(j) << " ";
      }
      cout << endl;
    }
    cout << endl;
    cout << endl;

    cout << "Please select the algorithm you would like to use to solve your puzzle." << endl;
    cout << "Type '1' for Uniform Cost Search." << endl;
    cout << "Type '2' for A* with the Misplaced Tile heuristic." << endl;
    cout << "Type '3' for A* with the Manhattan Distance heuristic." << endl;

    cout << endl;

    int choiceAlg;
    cin >> choiceAlg;


    //if input is 1, perform Uniform Cost Search
    if (choiceAlg == 1) {
      heuristic = 0;
    } 
    
    //if input is 2, perform A* with Misplaced Tile Heuristic
    else if (choiceAlg == 2) {
      heuristic = 1;
    } 
    
    //if input is 3, perform A* with Manhattan Distance Heuristic
    else if (choiceAlg == 3) {
      heuristic = 2;
    }
  }

  
  //creates the initial BoardState object
  BoardState *theInitial = new BoardState(userPuzzle);
 

 //pushes the initial BoardState object into the expandStates priority queue
  expandStates.push(theInitial);

  //begins timer to calculate the total time to solve the puzzle given
  //the initial board configuration
  clock_t startTime = clock();
  int solvedDepth = theInitial->solvePuzzle(heuristic); 

  cout << "Goal State!" << endl;
  cout << "Solution depth was " << solvedDepth << endl;
  cout << "Number of nodes expanded: " << numNodesExpanded << endl;
  cout << "Max queue size: " << maxQueueSize << endl;

  cout << "Total time: " << ((clock() - startTime) / (double)CLOCKS_PER_SEC)
       << " seconds" << endl;

  delete theInitial;

  return 0;
}
