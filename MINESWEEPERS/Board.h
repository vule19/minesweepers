#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include <vector>

struct Board{
    vector<vector<int>> mapvector;
    vector<vector<Tile>> spriteBoard;

    //cereate random number for random position of mines
    int random(int min, int max)
    {
        return min + rand() % ((max + 1) - min);
    }

    //generate a board with mines or non-mines - 2d vector
    void generate_board(int row, int col, int mines){
        for(int i = 0; i< row; i ++){
            mapvector.emplace_back(vector<int> (col));
        }
        for(int i = 0; i < row; i++) {
            for(int j = 0; j < col; j++){
                mapvector[i][j] = 0;
            }
        }
        for(int i = 0; i < mines; i++){
            int x = random(0, row - 1);
            int y = random(0, col - 1);
            if (mapvector[x][y] == 1) {
                i--; //skip if there is already an existing mine
            }
            else
                mapvector[x][y] = 1;
        }
    }

    void generate_grid(int row, int col) {
        spriteBoard.clear();
        for(int i = 0; i< row; i ++){
            spriteBoard.emplace_back(vector<Tile> (col));
        }
        for(int i = 0; i < row; i++) {
            for(int j = 0; j < col; j++){
                Tile sprite;
                spriteBoard[i][j] = sprite;
            }
        }
    }

    bool check_winner(int row, int col, int mines){
        int count = 0;
        for(int i = 0; i < row; i++) {
            for(int j = 0; j < col; j++){
                if(spriteBoard[i][j].revealed){
                    count++;
                }
            }
        }
        if(count == (row*col - mines)){  //open all non-mine tiles
            return true;
        }
        return false;
    }






};

