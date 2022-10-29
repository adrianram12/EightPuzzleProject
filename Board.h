#ifndef __BOARD_H__
#define __BOARD_H__

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class BoardState;

priority_queue<BoardState*> visitedStates;
priority_queue<BoardState*> expandStates;

vector<vector<int> > goalState = {{1,2,3}, {4,5,6}, {7,8,0}};

//create the node for each generated board state
class BoardState{

public:
    vector<vector<int> > board;
    int xcoordinate;
    int ycoordinate;

    int gnCost = 0;
    int fnCost = 0;

    BoardState* parent = nullptr;

    BoardState(){
      //this->findBlankSpace();
    }

    BoardState(vector<vector<int> > x){

        // cout << "sbout to find blank spac" << endl;
        this->board = x;
        this->findBlankSpace();

    }

    BoardState(BoardState& copyConstr){

      
      xcoordinate = copyConstr.xcoordinate;
      ycoordinate = copyConstr.ycoordinate;

      gnCost = copyConstr.gnCost;
      fnCost = copyConstr.fnCost;
      board = copyConstr.board;

      parent = new BoardState();
      *parent = copyConstr.parent;

      //cout << "printing copyconstr board" << endl;
      //copyConstr.printBoardState();
      //cout << "printing parent board " << endl;
      //parent->printBoardState();

    }

    BoardState operator=(BoardState* overload){
      //cout << "Printing overload statement" << endl;
      //overload->printBoardState();

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
          // cout << board.at(i).at(j) << endl;

          if(this->board.at(i).at(j) == 0){
            this->xcoordinate = i;
            this->ycoordinate = j;
          }
        }
      }
    }

    vector<vector<int> > legalMoves(){

      cout << xcoordinate << endl;
      cout << ycoordinate << endl;

      vector<vector<int> > potentialMoves = {{xcoordinate - 1, ycoordinate}, {xcoordinate + 1, ycoordinate}, {xcoordinate, ycoordinate - 1}, {xcoordinate, ycoordinate + 1}};

      vector<vector<int> > validMoves;

      for(unsigned int i = 0; i < potentialMoves.size(); i++){

          if((potentialMoves.at(i).at(0) >= 0 && potentialMoves.at(i).at(0) <= 2) && (potentialMoves.at(i).at(1) >= 0 && potentialMoves.at(i).at(1) <= 2)){

            validMoves.push_back(potentialMoves.at(i));
            cout << "potential moves " << potentialMoves.at(i).at(0) << " " << potentialMoves.at(i).at(1) << endl;
          }
      }

      return validMoves;

    }

    void expandBoardState(int h = 0){

      vector<vector<int> > validMoves = legalMoves();


      for(int i = 0; i < validMoves.size(); i++){
        //this->printBoardState();
        cout << "this xcoordinate: " << this->xcoordinate << endl;
        cout << "this ycoordinate: " << this->ycoordinate << endl;
        bool flag = false;

        BoardState* addChild = new BoardState(this->board); //deep copy

        //cout << "Printing the deep copy" << endl;
        //addChild->printBoardState();
        

        int temp = addChild->board.at((validMoves.at(i).at(0))).at(validMoves.at(i).at(1));

        //cout << temp << endl;

        addChild->board.at((validMoves.at(i).at(0))).at(validMoves.at(i).at(1)) = addChild->board.at(this->xcoordinate).at(this->ycoordinate);

        //cout << addChild->board.at((validMoves.at(i).at(0))).at(validMoves.at(i).at(1)) << endl;

        addChild->board.at(xcoordinate).at(ycoordinate) = temp;
        //this->printBoardState();
        //addChild->printBoardState();
        //cout << "-----------------------------------" << endl;
        //cout << addChild->board.at(xcoordinate).at(ycoordinate) << endl;



        BoardState* newBoard = new BoardState(addChild->board);
        newBoard->gnCost = this->gnCost + 1;

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
  cout << "Printing newboard board state" << endl;
        newBoard->printBoardState();

        
      }

    }

    int solvePuzzle(){

      while(expandStates.size() != 0){

        BoardState* topNode = expandStates.top();
        expandStates.pop();

        visitedStates.push(topNode);

        if(topNode->board == goalState){

          return topNode->gnCost;
        }

        topNode->expandBoardState();



      }
    }

    void printBoardState(){

        for(unsigned int i = 0; i < this->board.size(); i++){
            for(unsigned int j = 0; j < this->board.size(); j++){

                cout << this->board.at(i).at(j) << " ";
                
            }
            cout << endl;
        }
cout << endl;
cout << endl;
    }
};

#endif