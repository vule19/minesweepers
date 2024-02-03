#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "TextureManager.h"
#include <fstream>
#include <string>
#include "Board.h"
#include <sstream>

//read config file to get number of columns, rows, and mines
void Constructor(int &col, int &row, int &mines) {
    std::ifstream cFile("config.cfg");
    string num_col;
    string num_row;
    string num_mines;
    if (cFile.is_open()) {
        while (std::getline(cFile, num_col)) {
            col = stoi(num_col);
            getline(cFile, num_row, '\n');
            row = stoi(num_row);
            getline(cFile, num_mines, '\n');
            mines = stoi(num_mines);
        }
    }
}

//parse the digits for digit image
map<int, sf::Sprite> parseDigits(sf::Sprite digits) {
    map<int, sf::Sprite> digitsMap;
    for (int i = 0; i < 11; i++) {
        sf::IntRect rect(i * 21, 0, 21, 32);
        digits.setTextureRect(rect);
        sf::Sprite sprite = digits;
        digitsMap.emplace(i, sprite);
    }
    return digitsMap;
}

//read leaderboard.txt
void readLeaderboard(string& rankingstring){
    std::ifstream cFile("leaderboard.txt");
    if (cFile.is_open()) {
        for(int i = 1; i < 6 ; i ++){
            string line;
            getline(cFile, line);
            istringstream  stream(line);
            string time;
            getline(stream, time, ',');
            string name;
            getline(stream, name, ',');
            rankingstring += to_string(i) + ".\t" + time +"\t" + name + "\n\n";
        }

    }
}

//write/ append to leaderboard.txt
void writeLeaderboard(string& rankingstring){
    std::ifstream cFile("leaderboard.txt");
    string content;
    bool done = false;
    if (cFile.is_open()) {
        for(int i = 0; i < 5 ; i ++){
            string line;
            getline(cFile, line);
            if(stoi(rankingstring.substr(0,2)) < stoi(line.substr(0,2)) && !done){
                if(i != 0)
                content+="\n";
                content+=rankingstring;
                done = true;
                i++;
            }
            if(stoi(rankingstring.substr(0,2)) == stoi(line.substr(0,2)) && !done){
                if(stoi(rankingstring.substr(3,2)) < stoi(line.substr(3,2)) && !done){
                    if(i != 0)
                        content+="\n";
                    content+=rankingstring;
                    done = true;
                    i++;
                }
            }
            if(i!=0)
            content+="\n";
            content+=line;
        }
    }
    std::ofstream File("leaderboard.txt");
    File<<content;
}

int main() {
    int col;
    int row;
    int mines;
    Constructor(col, row, mines);

    /* Welcome Window */
    sf::RenderWindow welcomeWindow(sf::VideoMode(col * 32, row * 32 + 100), "Minesweeper", sf::Style::Close);

    sf::Font font;
    font.loadFromFile("font.ttf");
    if (!font.loadFromFile("font.ttf")) {
        std::cout << "can't load :(" << std::endl;
        return 0;
    }

    sf::Text welcomeText;
    welcomeText.setString("WELCOME TO MINESWEEPER!");
    welcomeText.setFont(font);
    welcomeText.setCharacterSize(24);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    welcomeText.setFillColor((sf::Color::White));

    sf::FloatRect welcomeTextRect = welcomeText.getGlobalBounds();
    welcomeText.setOrigin(welcomeTextRect.left + welcomeTextRect.width / 2.0f, welcomeTextRect.top + welcomeTextRect.height / 2.0f);
    welcomeText.setPosition(sf::Vector2f(col * 32 /2.0f,(row * 32 + 100)/2.0f-150));

    sf::Text enterName;
    enterName.setString("Enter your name:");
    enterName.setFont(font);
    enterName.setCharacterSize(20);
    enterName.setStyle(sf::Text::Bold);
    enterName.setFillColor((sf::Color::White));

    sf::FloatRect enterNameRect = enterName.getLocalBounds();
    enterName.setOrigin(enterNameRect.left + enterNameRect.width / 2.0f, enterNameRect.top + enterNameRect.height / 2.0f);
    enterName.setPosition(sf::Vector2f(col * 32 /2.0f,(row * 32 + 100)/2.0f-75));

    sf::Text typedName;
    typedName.setFont(font);
    typedName.setCharacterSize(18);
    typedName.setStyle(sf::Text::Bold);
    typedName.setFillColor((sf::Color::White));
    typedName.setPosition(sf::Vector2f(col * 32 /2.0f,(row * 32 + 100)/2.0f-45));

    std::string inputText;

    sf::RectangleShape cursorIndicator;
    cursorIndicator.setSize(sf::Vector2f(2.0f, static_cast<float>(typedName.getCharacterSize())));
    cursorIndicator.setFillColor(sf::Color::White);

    int char_num = 0; //keep track of #characters user typing

    while(welcomeWindow.isOpen()) {
        sf::Event event;
        while(welcomeWindow.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                welcomeWindow.close();
                return 0;
            }

            if(event.type == sf::Event::TextEntered) {
                //only accept alphabetical characters
                if((64 < event.text.unicode && event.text.unicode < 91) || (96 < event.text.unicode && event.text.unicode < 123) && char_num < 10) {
                    if (char_num == 0){ //uppercase first character
                        inputText += std::toupper(static_cast<char>(event.text.unicode));
                    }
                    else { //lowercase the rest
                        inputText += std::tolower(static_cast<char>(event.text.unicode));
                    }
                    char_num++; //increment #characters
                }
                //backspace to remove last character
                else if (event.text.unicode == '\b' && !inputText.empty()) {
                    inputText.pop_back();
                    char_num--;
                }
                //Enter button (only works after entering username)
                else if( event.text.unicode == 13 && !inputText.empty())
                    welcomeWindow.close();
            }
        }

        typedName.setString(inputText);
        sf::FloatRect typedNameRect = typedName.getLocalBounds();
        typedName.setOrigin(typedNameRect.left + typedNameRect.width / 2.0f, typedNameRect.top + typedNameRect.height / 2.0f);
        welcomeWindow.clear(sf::Color::Blue);
        welcomeWindow.draw(welcomeText);
        welcomeWindow.draw(enterName);
        welcomeWindow.draw(typedName);
        cursorIndicator.setPosition(typedName.findCharacterPos(inputText.size()));
        welcomeWindow.draw(cursorIndicator);
        welcomeWindow.display();
    }
    string username = inputText; //this is username that user enters

    /* Game Window */

    bool show_mine = false;
    bool lost = false;
    bool win = false;
    bool leaderboard = false;

    sf::RenderWindow gameWindow(sf::VideoMode(col * 32, row * 32 + 100), "Minesweeper", sf::Style::Close);

    auto start_time = chrono::high_resolution_clock::now();
    auto pauseTime = chrono::high_resolution_clock::now();
    auto elapsed_paused_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - pauseTime).count();
    bool paused = false; //false when game in not paused, true when the game is paused
    sf::Texture& digitsText = TextureManager::getTexture("digits");
    sf::Sprite digits;
    digits.setTexture(digitsText);
    map<int, sf::Sprite> digitsMap = parseDigits(digits); //map for my digits
    sf::Texture& pauseText = TextureManager::getTexture("pause");
    sf::Sprite pauseBttn;
    pauseBttn.setTexture(pauseText);
    pauseBttn.setPosition(col * 32 - 240, 32 * (row + 0.5));
    sf::Texture& playText = TextureManager::getTexture("play");
    sf::Sprite playBttn;
    playBttn.setTexture(playText);
    playBttn.setPosition(col * 32 - 240, 32 * (row + 0.5));

    sf::Texture& face_happyTexture = TextureManager::getTexture("face_happy");
    sf::Sprite face_happySprite;
    face_happySprite.setTexture(face_happyTexture);
    face_happySprite.setPosition(col/ 2.0 * 32 -32, 32 * (row + 0.5));

    sf::Texture& face_loseTexture = TextureManager::getTexture("face_lose");
    sf::Sprite face_loseSprite;
    face_loseSprite.setTexture(face_loseTexture);
    face_loseSprite.setPosition(col/ 2.0 * 32 -32, 32 * (row + 0.5));

    sf::Texture& face_winTexture = TextureManager::getTexture("face_win");
    sf::Sprite face_winSprite;
    face_winSprite.setTexture(face_winTexture);
    face_winSprite.setPosition(col/ 2.0 * 32 -32, 32 * (row + 0.5));

    sf::Texture& debugTexture = TextureManager::getTexture("debug");
    sf::Sprite debugSprite;
    debugSprite.setTexture(debugTexture);
    debugSprite.setPosition(col * 32 - 304, 32 * (row + 0.5));

    sf::Texture& leaderboardTexture = TextureManager::getTexture("leaderboard");
    sf::Sprite leaderboardSprite;
    leaderboardSprite.setTexture(leaderboardTexture);
    leaderboardSprite.setPosition(col * 32 - 176, 32 * (row + 0.5));

    sf::Texture& mineTexture = TextureManager::getTexture("mine");
    sf::Sprite mineSprite;
    mineSprite.setTexture(mineTexture);

    sf::Texture& tile_hiddenTexture = TextureManager::getTexture("tile_hidden");
    sf::Sprite tile_hiddenSprite;
    tile_hiddenSprite.setTexture(tile_hiddenTexture);

    sf::Texture& tile_revealedTexture = TextureManager::getTexture("tile_revealed");
    sf::Sprite tile_revealedSprite;
    tile_revealedSprite.setTexture(tile_revealedTexture);

    //calling Board class for a new board with defaults
    Board board;
    board.generate_board(row, col, mines); //board with mine(1) and non-mine(0)
    board.generate_grid(row, col);         //board with sprite
    int mines_left = mines;
    int update_leaderboard = 0;
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++){
            board.spriteBoard[i][j].x = j;
            board.spriteBoard[i][j].y = i;
            if(board.mapvector[i][j] == 1){
                board.spriteBoard[i][j].this_is_mine(); //set mine
            }
        }
    }

    while(gameWindow.isOpen()) {
        sf::Event event;
        while(gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                gameWindow.close();
            }
            //right click to place or remove flags
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                sf::Vector2i  mouse;
                mouse = sf::Mouse::getPosition(gameWindow);
                if(mouse.y <= row * 32 && !paused && !win && !lost){
                    board.spriteBoard[mouse.y/32][mouse.x/32].right_clicked(mines_left);
                }
            }
            //left click
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mouse_left;
                mouse_left = sf::Mouse::getPosition(gameWindow);
                //within the grid and game is played - reveal the tile
                if (mouse_left.y <= row * 32 && !paused && !win && !lost) {
                    //when tile does not have mine
                    if (board.mapvector[mouse_left.y / 32][mouse_left.x / 32] == 0) {
                        board.spriteBoard[mouse_left.y / 32][mouse_left.x / 32].left_clicked(board.spriteBoard, row, col);
                    }
                    //tile has mine
                    else if (board.mapvector[mouse_left.y / 32][mouse_left.x / 32] == 1 && !board.spriteBoard[mouse_left.y / 32][mouse_left.x / 32].flag_on) {
                        show_mine = true; //display all mines
                        lost = true; //game over
                    }
                }
                //game is paused - pause/play button
                if (pauseBttn.getGlobalBounds().contains(mouse_left.x, mouse_left.y)) {
                    if (!win && !lost) { //cannot pause when there is win or loss
                        paused = !paused; //boolean changed everytime pause button is clicked
                        if (paused) {
                            pauseTime = chrono::high_resolution_clock::now();
                        }
                        else { //unpause
                            auto unPausedTime = chrono::high_resolution_clock::now();
                            elapsed_paused_time += chrono::duration_cast<chrono::seconds>(
                                    unPausedTime - pauseTime).count();
                        }
                    }
                }
                //debug button
                if (debugSprite.getGlobalBounds().contains(mouse_left.x, mouse_left.y)) {
                    if (!win && !lost && !paused)
                        show_mine = !show_mine;
                }
                //happy face button - restart new game
                if (face_happySprite.getGlobalBounds().contains(mouse_left.x, mouse_left.y)) {
                    //reset everything
                    board.generate_grid(row, col);
                    board.generate_board(row, col, mines);
                    for (int i = 0; i < row; i++) {
                        for (int j = 0; j < col; j++) {
                            board.spriteBoard[i][j].x = j;
                            board.spriteBoard[i][j].y = i;
                            if (board.mapvector[i][j] == 1) {
                                board.spriteBoard[i][j].this_is_mine();
                            }
                        }
                    }
                    mines_left = mines;
                    update_leaderboard = 0;
                    paused = false;
                    show_mine = false;
                    lost = false;
                    win = false;
                    start_time = chrono::high_resolution_clock::now();
                }
                //leaderboard button - open Leaderboard window
                if (leaderboardSprite.getGlobalBounds().contains(mouse_left.x, mouse_left.y)) {
                    leaderboard = true;
                    paused = true;
                }
            }
        }

        gameWindow.clear(sf::Color::White);
        auto game_duration = std::chrono::duration_cast<std::chrono::seconds>(chrono::high_resolution_clock::now() - start_time);
        int total_time = game_duration.count(); // necessary to subtract elapsed time later because "game_duration.count()" is const
        int minutes;
        int seconds;
        if(!paused) {
            total_time = total_time - elapsed_paused_time;
            minutes = total_time / 60;
            seconds = total_time % 60;
        }
        int minutes0 = minutes / 10 % 10; //minutes index 0
        int minutes1 = minutes % 10; // minutes index 1
        int seconds0 = seconds / 10 % 10; // seconds index 0
        int seconds1 = seconds % 10; // seconds index 1
        digitsMap[minutes0].setPosition(col*32 - 97, 32*(row + 0.5f) + 16);
        gameWindow.draw(digitsMap[minutes0]);
        digitsMap[minutes1].setPosition(col*32 - 76, 32*(row + 0.5f) + 16);
        gameWindow.draw(digitsMap[minutes1]);
        digitsMap[seconds0].setPosition(col*32 - 54, 32*(row + 0.5f) + 16);
        gameWindow.draw(digitsMap[seconds0]);
        digitsMap[seconds1].setPosition(col*32 - 33, 32*(row + 0.5f) + 16);
        gameWindow.draw(digitsMap[seconds1]);
        gameWindow.draw(pauseBttn);

        if(paused){
            gameWindow.draw(playBttn);
        }
        //display the number of mines - number of flags
        if(mines_left < 0){    //handle negative sign
            digitsMap[10].setPosition(12, 32*(row + 0.5f) + 16);
            gameWindow.draw(digitsMap[10]);
        }
        int temp = abs(mines_left);
        int mine1 = temp/100;
        int mine2 = (temp%100)/10;
        int mine3 = (temp%100)%10;
        digitsMap[mine1].setPosition(33, 32*(row + 0.5f) + 16);
        gameWindow.draw(digitsMap[mine1]);
        digitsMap[mine2].setPosition(54, 32*(row + 0.5f) + 16);
        gameWindow.draw(digitsMap[mine2]);
        digitsMap[mine3].setPosition(75, 32*(row + 0.5f) + 16);
        gameWindow.draw(digitsMap[mine3]);

        //checking if player wins
        if(board.check_winner(row, col, mines) & !lost){
            update_leaderboard++;
            win = true;
            paused =true;
            string time;
            if(update_leaderboard == 1){
                time = to_string(minutes/10) + to_string(minutes%10) + ":" + to_string(seconds/10) + to_string(seconds%10); //amount of time that user takes to win
                string new_rank = time + "," + username + "*";
                writeLeaderboard(new_rank);
                for(int i = 0; i < row; i++){
                    for(int j = 0; j < col; j++){
                        if(board.mapvector[i][j] == 1) {
                            board.spriteBoard[i][j].right_clicked(mines_left);
                        }
                    }
                }
            }

        }
        //tile_hidden as a background
        for(int i = 0; i < row; i++) {
            for(int j = 0; j < col; j++){
                tile_hiddenSprite.setPosition(j*32, i*32);
                gameWindow.draw(tile_hiddenSprite);
            }
        }
        //draw every tile
        for(int i = 0; i < row; i++) {
            for(int j = 0; j < col; j++){
                board.spriteBoard[i][j].draw(gameWindow);
            }
        }
        if(show_mine) {
            for(int i = 0; i < row; i++){
                for(int j = 0; j < col; j++){
                    if(board.mapvector[i][j] == 1) {
                        mineSprite.setPosition(j * 32, i * 32);
                        gameWindow.draw(mineSprite);
                    }
                }
            }
        }
        if(lost){
            gameWindow.draw(face_loseSprite);
        }
        else if(win){
            gameWindow.draw(face_winSprite);
        }
        else{
            gameWindow.draw(face_happySprite);
        }
        if(paused && !win){
            for(int i = 0; i < row; i++){
                for(int j = 0; j < col; j++){
                    tile_revealedSprite.setPosition(j*32, i*32);
                    gameWindow.draw(tile_revealedSprite); //tiles turn into tie_revealed when it is paused
                }
            }
        }
        gameWindow.draw(debugSprite);
        gameWindow.draw(leaderboardSprite);
        gameWindow.display();

        /* Leaderboard */
        if(leaderboard){
            string rankingstring; // content of leaderboard.txt after reading file
            readLeaderboard(rankingstring);
            sf::RenderWindow leaderboardWindow(sf::VideoMode(col * 16, row * 16 + 50), "Minesweeper", sf::Style::Close);

            sf::Text leaderboardText;
            leaderboardText.setString("LEADERBOARD");
            leaderboardText.setFont(font);
            leaderboardText.setCharacterSize(20);
            leaderboardText.setStyle(sf::Text::Bold | sf::Text::Underlined);
            leaderboardText.setFillColor((sf::Color::White));
            sf::FloatRect leaderboardRect = leaderboardText.getGlobalBounds();
            leaderboardText.setOrigin(leaderboardRect.left + leaderboardRect.width / 2.0f, leaderboardRect.top + leaderboardRect.height / 2.0f);
            leaderboardText.setPosition(sf::Vector2f(col * 16 /2.0f,(row * 16 + 50)/2.0f-120));

            sf::Text filecontent;
            filecontent.setFont(font);
            filecontent.setString(rankingstring);
            filecontent.setCharacterSize(18);
            filecontent.setStyle(sf::Text::Bold);
            filecontent.setFillColor((sf::Color::White));
            sf::FloatRect contentRect = filecontent.getGlobalBounds();
            filecontent.setOrigin(contentRect.left + contentRect.width / 2.0f, contentRect.top + contentRect.height / 2.0f);
            filecontent.setPosition(sf::Vector2f(col * 16 /2.0f,(row * 16 + 50)/2.0f+20));

            while(leaderboardWindow.isOpen()) {
                while(leaderboardWindow.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        leaderboard = false;
                        paused = false;
                        leaderboardWindow.close();
                    }
                }
                leaderboardWindow.clear(sf::Color::Blue);
                leaderboardWindow.draw(leaderboardText);
                leaderboardWindow.draw(filecontent);
                leaderboardWindow.display();
            }
        }
    }
    return 0;
}