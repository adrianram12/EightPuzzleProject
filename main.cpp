#include <iostream>
#include <vector>
#include <queue>
#include <ctime>


using namespace std;
class BoardState;

priority_queue<BoardState*> visitedStates;


vector<vector<int> > goalState = {{1,2,3}, {4,5,6}, {7,8,0}};

//create the node for each generated board state
class BoardState{

public:
    vector<vector<int> > board;
    int xcoordinate;
    int ycoordinate;

    int gnCost = 0;
    int fnCost = 0;
    int hnCost = 0;

    BoardState* parent = nullptr;

void expandBoardState(int);
int solvePuzzle(int);


    BoardState(){
      this->findBlankSpace();
      gnCost = 0;
      fnCost = 0;

    }

    BoardState(vector<vector<int> > x){
        this->board = x;

        this->findBlankSpace();

        gnCost = 0;
        fnCost = 0;

    }

    BoardState(BoardState& copyConstr){

      
      xcoordinate = copyConstr.xcoordinate;
      ycoordinate = copyConstr.ycoordinate;

      gnCost = copyConstr.gnCost;
      fnCost = copyConstr.fnCost;
      board = copyConstr.board;

      parent = new BoardState();
      *parent = copyConstr.parent;

    }

    BoardState operator=(BoardState* overload){

      BoardState newBoard;

      newBoard.xcoordinate = overload->xcoordinate;
      newBoard.ycoordinate = overload->ycoordinate;
      newBoard.gnCost = overload->gnCost;
      newBoard.fnCost = overload->fnCost;
      newBoard.board = overload->board;
      newBoard.parent = overload->parent;

      return *this;

    }

    int MisplacedTile(){

      int misplacedTiles = 0;
      
      for(int i = 0; i < this->board.size(); i++){
        for(int j = 0; j < this->board.size(); j++){

          if((this->board.at(i).at(j) != 0) && (this->board.at(i).at(j) != goalState.at(i).at(j))){

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

    void findBlankSpace(){

      for(unsigned int i = 0; i < 3; i++){

        for(unsigned int j = 0; j < 3; j++){

          if(this->board.at(i).at(j) == 0){
            this->xcoordinate = i;
            this->ycoordinate = j;
          }
        }
      }
    }

    vector<vector<int> > legalMoves(){

      vector<vector<int> > potentialMoves = {{xcoordinate - 1, ycoordinate}, {xcoordinate + 1, ycoordinate}, {xcoordinate, ycoordinate - 1}, {xcoordinate, ycoordinate + 1}};

      vector<vector<int> > validMoves;

      for(unsigned int i = 0; i < potentialMoves.size(); i++){

          if((potentialMoves.at(i).at(0) >= 0 && potentialMoves.at(i).at(0) <= 2) && (potentialMoves.at(i).at(1) >= 0 && potentialMoves.at(i).at(1) <= 2)){

            validMoves.push_back(potentialMoves.at(i));
             
          }
      }

      return validMoves;

    }

    void printPuzzleState(){

        for(unsigned int i = 0; i < this->board.size(); i++){
            for(unsigned int j = 0; j < this->board.size(); j++){

                cout << this->board.at(i).at(j) << " ";
                
            }
            cout << endl;
        }
cout << endl;
cout << endl;
    }

    

    void printSolution(BoardState* final) {
      while (final->parent) {
        final->printPuzzleState();
        final = final->parent;
      }
    }
};

struct Comparison{

  bool operator()(BoardState* lhs, BoardState* rhs){

    return lhs->fnCost > rhs->fnCost;

  }
};

priority_queue<BoardState*, vector<BoardState*>, Comparison > expandStates;

void BoardState::expandBoardState(int heuristic){

      vector<vector<int> > validMoves = this->legalMoves();

      for(int i = 0; i < validMoves.size(); i++){
        
        bool flag = false;

        BoardState* addChild = new BoardState(this->board); //deep copy
        

        int temp = addChild->board.at((validMoves.at(i).at(0))).at(validMoves.at(i).at(1));

        addChild->board.at((validMoves.at(i).at(0))).at(validMoves.at(i).at(1)) = addChild->board.at(this->xcoordinate).at(this->ycoordinate);

        addChild->board.at(xcoordinate).at(ycoordinate) = temp;

        if(heuristic == 0){
          fnCost = this->gnCost + 1;
        }
        else if(heuristic == 1){
         
          fnCost = this->gnCost + MisplacedTile();
          cout << "FNCOST!!!!!!!!!!!!!!!!!!!!!: " << this->fnCost << endl;
        }

        else if(heuristic == 2){

          fnCost = this->gnCost + ManhattanDistance();
        }

        BoardState* newBoard = new BoardState(addChild->board);
        newBoard->gnCost = this->gnCost + 1;
        newBoard->fnCost = fnCost;
    
        
        
        newBoard->parent = this;
        

        priority_queue<BoardState*> tempQ = visitedStates;

        while(!(tempQ.empty())){

          if(newBoard->board == tempQ.top()->board) {
            flag = true;
            break;
          }
          else {
            flag = false;
            tempQ.pop();
          }
        }

        if (!flag) {

          
          expandStates.push(newBoard);
    

        }
  
      }



    }

int BoardState::solvePuzzle(int heuristic){

      while(expandStates.size() != 0){

        BoardState* topNode = expandStates.top();
        expandStates.pop();


        visitedStates.push(topNode);

        if(topNode->board == goalState){
          this->printSolution(topNode);
          return topNode->fnCost;
        }

        topNode->expandBoardState(heuristic);



      }
    }


vector<vector<int> > level1 = {{1,2,3}, {4,5,6}, {7,8,0}};  //depth: 0
vector<vector<int> > level2 = {{1,2,3}, {4,5,6}, {0,7,8}};  //depth: 2
vector<vector<int> > level3 = {{1,2,3}, {5,0,6}, {4,7,8}};  //depth: 4
vector<vector<int> > level4 = {{1,3,6}, {5,0,2}, {4,7,8}};  //depth: 8
vector<vector<int> > level5 = {{1,3,6}, {5,0,7}, {4,8,2}};  //depth: 12
vector<vector<int> > level6 = {{1,6,7}, {5,0,3}, {4,8,2}};  //depth: 16
vector<vector<int> > level7 = {{7,1,2}, {4,8,5}, {6,3,0}};  //depth: 20
vector<vector<int> > level8 = {{0,7,2}, {4,6,1}, {3,5,8}};  //depth: 24





int main(){

  int heuristic = 2;
  //ActualPuzzle puzzle;
  BoardState* theInitial = new BoardState(level6);
  expandStates.push(theInitial);

  clock_t startTime = clock();
  int solvedDepth = theInitial->solvePuzzle(heuristic); //puzzle.solvePuzzle

  cout << "Total Cost f(n): " << solvedDepth << endl; 

  cout << "Total time: " << (float)(clock() - startTime) / (CLOCKS_PER_SEC / 1000) << endl;
  delete theInitial;

  return 0;
}
