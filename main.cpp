#include <iostream>
#include <vector>
#include <queue>


using namespace std;


vector<vector<int> > level1 = {{1,2,3}, {4,5,6}, {7,8,0}};
vector<vector<int> > level2 = {{1,2,3}, {4,5,6}, {0,7,8}};

priority_queue<BoardState*> visitedStates;
priority_queue<BoardState*> expandStates;




//create the node for each generated board state
class BoardState{

public:
    vector<vector<int> > board;
    int xcoordinate;
    int ycoordinate;

    int gnCost = 0;
    int fnCost = 0;

    BoardState* parent = nullptr;

    BoardState(){}

    BoardState(vector<vector<int> > x){

        this->board = x;

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

    void findBlankSpace(){

      for(unsigned int i = 0; i < 3; i++){

        for(unsigned int j = 0; j < 3; j++){

          if(board.at(i).at(j) == 0){
            xcoordinate = i;
            ycoordinate = j;
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

    void expandBoardState(int h = 0){

      vector<vector<int> > validMoves = legalMoves();


      for(int i = 0; i < validMoves.size(); i++){
        bool flag = false;

        BoardState* addChild = this; //deep copy

        int temp = addChild->board.at((validMoves.at(i).at(0))).at(validMoves.at(i).at(1));

        addChild->board.at((validMoves.at(i).at(0))).at(validMoves.at(i).at(1)) = addChild->board.at(xcoordinate).at(ycoordinate);

        addChild->board.at(xcoordinate).at(ycoordinate) = temp;

        BoardState* newBoard = new BoardState(addChild->board);

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

          visitedStates.push(newBoard);



        }

        

      }



    



      


      


    }

    void printBoardState(){

        for(unsigned int i = 0; i < board.size(); i++){
            for(unsigned int j = 0; j < board.size(); j++){

                cout << board.at(i).at(j) << " ";
                
            }
            cout << endl;
        }

        cout << endl;
        cout << endl;

    }
};

int main(){

  BoardState theInitial(level1);

  theInitial.printBoardState();
  theInitial.findBlankSpace();
  theInitial.legalMoves();
  theInitial.expandBoardState();

  return 0;
}

  