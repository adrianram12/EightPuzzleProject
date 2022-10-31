#include <iostream>
#include <vector>
#include <queue>
#include <ctime>

//#include "Board.h"


using namespace std;
class BoardState;

priority_queue<BoardState*> visitedStates;
//priority_queue<BoardState*, vector<BoardState*>, Comparison > expandStates;


vector<vector<int> > goalState = {{1,2,3}, {4,5,6}, {7,8,0}};

//create the node for each generated board state
class BoardState{

public:
    vector<vector<int> > board;
    int xcoordinate;
    int ycoordinate;

    int gnCost;
    int fnCost = 0;
    int hnCost = 0;

    BoardState* parent = nullptr;

   

  //   bool operator()(BoardState* rhs)
  // {
  //       return this->gnCost < rhs->gnCost;
  // }

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

          if(this->board.at(i).at(j) != goalState.at(i).at(j)){

            misplacedTiles = misplacedTiles + 1;
          }
        }
      }

      return misplacedTiles;

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

    void expandBoardState(int heuristic, priority_queue<BoardState*, vector<BoardState*>, Comparison > expandStates){
      vector<vector<int> > validMoves = this->legalMoves();

      for(int i = 0; i < validMoves.size(); i++){
        
        bool flag = false;

        BoardState* addChild = new BoardState(this->board); //deep copy
        

        int temp = addChild->board.at((validMoves.at(i).at(0))).at(validMoves.at(i).at(1));

        addChild->board.at((validMoves.at(i).at(0))).at(validMoves.at(i).at(1)) = addChild->board.at(this->xcoordinate).at(this->ycoordinate);

        addChild->board.at(xcoordinate).at(ycoordinate) = temp;

        if(heuristic == 1){
          fnCost = this->gnCost + MisplacedTile();
        }

        else if(heuristic == 2){

          //fnCost = this->gnCost + ManhattanDistance();
        }

        BoardState* newBoard = new BoardState(addChild->board);
        newBoard->gnCost = this->gnCost + 1;
        
        //cout << "GNCOST!!!!!!!!!!!!!!!!!!!!!!!: " << newBoard->gnCost << endl;
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

    int solvePuzzle(int heuristic, priority_queue<BoardState*, vector<BoardState*>, Comparison > expandStates){

      while(expandStates.size() != 0){

        BoardState* topNode = expandStates.top();
        expandStates.pop();


        visitedStates.push(topNode);

        if(topNode->board == goalState){
          this->printSolution(topNode);
          return topNode->gnCost;
        }

        topNode->expandBoardState(heuristic, expandStates);



      }
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

    return lhs->gnCost > rhs->gnCost;

  }
};



//priority_queue<BoardState*> visitedStates;
//priority_queue<BoardState*, vector<BoardState*>, Comparison > expandStates;


vector<vector<int> > level1 = {{1,2,3}, {4,5,6}, {7,8,0}};  //depth: 0
vector<vector<int> > level2 = {{1,2,3}, {4,5,6}, {0,7,8}};  //depth: 2
vector<vector<int> > level3 = {{1,2,3}, {5,0,6}, {4,7,8}};  //depth: 4
vector<vector<int> > level4 = {{1,3,6}, {5,0,2}, {4,7,8}};  //depth: 8
vector<vector<int> > level5 = {{1,3,6}, {5,0,7}, {4,8,2}};  //depth: 12
vector<vector<int> > level6 = {{1,6,7}, {5,0,3}, {4,8,2}};  //depth: 16
vector<vector<int> > level7 = {{7,1,2}, {4,8,5}, {6,3,0}};  //depth: 20
vector<vector<int> > level8 = {{0,7,2}, {4,6,1}, {3,5,8}};  //depth: 24





int main(){

  priority_queue<BoardState*, vector<BoardState*>, Comparison > expandStates;

  int heuristic = 1;
  //ActualPuzzle puzzle;
  BoardState* theInitial = new BoardState(level1);
  expandStates.push(theInitial);

  clock_t startTime = clock();
  int solvedDepth = theInitial->solvePuzzle(heuristic, expandStates); //puzzle.solvePuzzle

  cout << "Depth is: " << solvedDepth << endl; 

  cout << "Total time: " << (clock() - startTime) / (CLOCKS_PER_SEC / 1000) << endl;
  delete theInitial;

  return 0;
}
