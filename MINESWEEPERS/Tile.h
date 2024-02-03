#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "TextureManager.h"
#include "Board.h"

struct Tile{
    sf::Sprite sprite;
    bool flag_on = false;
    bool revealed = false;
    vector<Tile*> neighbor_vector;
    bool is_mine = false;
    int click = 0;
    int x;
    int y;
    int count_mine = 0;

    Tile() {
        sprite.setTexture(TextureManager::getTexture("tile_hidden")); //set as default
    }

    void this_is_mine(){
        is_mine = true; //if this tile has mine
    }

    void right_clicked(int& mines_left){
        flag_on = !flag_on;
        if(!revealed){
            if(flag_on){
                sprite.setTexture(TextureManager::getTexture("flag"));
                mines_left--;
            }
            else{
                sprite.setTexture(TextureManager::getTexture("tile_hidden"));
                revealed = false;
                mines_left++;
            }
        }
    }

    void left_clicked(vector<vector<Tile>> &srpiteVector, int row, int col){
        if(!flag_on){
            if(click != 0){return;} //cannot do anything after being revealed/ left-clicked
            click++;
            revealed = true;
            checking_neighbor(srpiteVector, row, col);
            if(!is_mine){
                if(count_mine == 0){
                    sprite.setTexture(TextureManager::getTexture("tile_revealed"));
                    for(auto & i : neighbor_vector){
                        i->left_clicked(srpiteVector, row, col); //recursive to open neighbor tiles
                    }
                }
                else{
                    std::string s = std::to_string(count_mine);
                    sprite.setTexture(TextureManager::getTexture("number_"+s));
                    return;
                }
            }
            else{
                return;
            }
        }
    }

    void checking_neighbor(vector<vector<Tile>> &srpiteVector, int row, int col) {
        if(x == 0 && y == 0){
            neighbor_vector.push_back(&srpiteVector[y][x+1]);
            neighbor_vector.push_back(&srpiteVector[y+1][x]);
            neighbor_vector.push_back(&srpiteVector[y+1][x+1]);
        }
        else if(x == (col -1) && y == (row - 1)){
            neighbor_vector.push_back(&srpiteVector[y][x-1]);
            neighbor_vector.push_back(&srpiteVector[y-1][x]);
            neighbor_vector.push_back(&srpiteVector[y-1][x-1]);
        }
        else if(x == (col -1) && y == 0){
            neighbor_vector.push_back(&srpiteVector[y][x-1]);
            neighbor_vector.push_back(&srpiteVector[y+1][x]);
            neighbor_vector.push_back(&srpiteVector[y+1][x-1]);
        }
        else if(x == 0 && y == (row - 1)){
            neighbor_vector.push_back(&srpiteVector[y][x+1]);
            neighbor_vector.push_back(&srpiteVector[y-1][x]);
            neighbor_vector.push_back(&srpiteVector[y-1][x+1]);
        }
        else if(x == 0){
            neighbor_vector.push_back(&srpiteVector[y-1][x]);
            neighbor_vector.push_back(&srpiteVector[y-1][x+1]);
            neighbor_vector.push_back(&srpiteVector[y][x+1]);
            neighbor_vector.push_back(&srpiteVector[y+1][x+1]);
            neighbor_vector.push_back(&srpiteVector[y+1][x]);
        }
        else if(x == col - 1){
            neighbor_vector.push_back(&srpiteVector[y-1][x]);
            neighbor_vector.push_back(&srpiteVector[y-1][x-1]);
            neighbor_vector.push_back(&srpiteVector[y][x-1]);
            neighbor_vector.push_back(&srpiteVector[y+1][x-1]);
            neighbor_vector.push_back(&srpiteVector[y+1][x]);
        }
        else if(y == 0){
            neighbor_vector.push_back(&srpiteVector[y][x-1]);
            neighbor_vector.push_back(&srpiteVector[y+1][x-1]);
            neighbor_vector.push_back(&srpiteVector[y+1][x]);
            neighbor_vector.push_back(&srpiteVector[y+1][x+1]);
            neighbor_vector.push_back(&srpiteVector[y][x+1]);
        }
        else if(y == row -1){
            neighbor_vector.push_back(&srpiteVector[y][x-1]);
            neighbor_vector.push_back(&srpiteVector[y-1][x-1]);
            neighbor_vector.push_back(&srpiteVector[y-1][x]);
            neighbor_vector.push_back(&srpiteVector[y-1][x+1]);
            neighbor_vector.push_back(&srpiteVector[y][x+1]);
        }
        else{
            neighbor_vector.push_back(&srpiteVector[y][x-1]);
            neighbor_vector.push_back(&srpiteVector[y-1][x-1]);
            neighbor_vector.push_back(&srpiteVector[y-1][x]);
            neighbor_vector.push_back(&srpiteVector[y-1][x+1]);
            neighbor_vector.push_back(&srpiteVector[y][x+1]);
            neighbor_vector.push_back(&srpiteVector[y+1][x+1]);
            neighbor_vector.push_back(&srpiteVector[y+1][x]);
            neighbor_vector.push_back(&srpiteVector[y+1][x-1]);
        }
        //count the number of mines around tile
        for (auto & i : neighbor_vector){
            if(i->is_mine){
                count_mine++;
            }
        }
    }

    void draw(sf::RenderWindow& window){
        sprite.setPosition(x * 32, y * 32);
        window.draw(sprite);
    }

};