#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "Tile.h"
#include <sstream>
#include "TextureManager.h"
#include <vector>
#include "Button.h"
#include <chrono>

using namespace std;

void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

void WriteText(sf::RenderWindow& window, const std::string& text, bool bold, bool underline, sf::Color color, int size, float x, float y) {
    //simple stuff, writes the text
    sf::Font font;
    font.loadFromFile("files/font.ttf");
    sf::Text word(text, font, size);
    word.setFillColor(color);
    word.setPosition(x, y);
    if (bold) {
        word.setStyle(sf::Text::Bold);
    }
    if (underline) {
        word.setStyle(sf::Text::Underlined);
    }

    //centering
    setText(word, x, y);

    window.draw(word);
}

void Window(sf::RenderWindow &window, int type, int row, int col, float width, float height, const string& username) {
    //this function lays the base for the window
    if (type == 1) {
        window.clear(sf::Color::Blue);
        WriteText(window, "WELCOME TO MINESWEEPER!", true, true, sf::Color::White, 24, (width/2), ((height/2)-150));
        WriteText(window, "Enter your name:", true, false, sf::Color::White, 20, (width/2), ((height/2)-75));
        WriteText(window, username + "|", true, false, sf::Color::Yellow, 18, (width/2), ((height/2)-45));
    }
    if (type == 2) {
        window.clear(sf::Color::White);
        // WriteText(window, "Game Screen...", true, true, sf::Color::Black, 28, (width/2), ((height/2)-250));
        // WriteText(window, ("Hello " + username), true, false, sf::Color::Black, 30, (width/2), ((height/2)-150));
    }
    if (type == 3) {
        window.clear(sf::Color::Blue);
        WriteText(window, "LEADERBOARD", true, true, sf::Color::White, 20, width/2, ((height/2) + 20));
    }
}

void generateRandom(vector<Tile>& tiles, int row, int col, int mines, vector<sf::Texture>& texters, vector<sf::Texture>& numbers, int startIndex = -1) {
    tiles.clear();

    //makes all the tiles
    for (int x = 0; x < col; x++) {
        for (int y = 0; y < row; y++) {
            tiles.emplace_back(texters[0], texters[1], texters[2], texters[3], numbers, x * 32, y * 32);

        }
    }
    //doesn't genertate bombs if startIndex is -1, should only happen for first click
    if (startIndex == -1) {
        return;
    }

    int totalTiles = col * row;
    int placedMines = 0;

    //randomly makes the tiles into bombs
    while (placedMines < mines) {
        int randIndex = rand() % totalTiles;

        if (randIndex != startIndex && !tiles[randIndex].IsMine()) {
            tiles[randIndex].toggleMine();
            placedMines++;
        }
    }

    for (int x = 0; x < col; x++) {
        for (int y = 0; y < row; y++) {
            int count = 0;
            int place = x * row + y;

            //adjacent areas to the tile, like bottom left, bottom mid, bottom right, etc.
            int aroundXes[] = {-1,0,1,-1,1,-1,0,1};
            int aroundYes[] = {-1,-1,-1,0,0,1,1,1};

            for (int i = 0; i < 8; i++) {
                int adjX = x + aroundXes[i];
                int adjY = y + aroundYes[i];

                if (adjX >= 0 && adjX < col && adjY >= 0 && adjY < row) {
                    int adjPlace = adjX * row + adjY;
                    if (tiles[adjPlace].IsMine()) {
                        count++;
                    }
                }
            }
            //gives the current tile the adj count that we found previously
            tiles[place].setAdjMines(count);
        }
    }
}

string LeaderboardInfo(string& username, bool win = false, bool first = false, int endTime = 0) {
    string longtext;
    string path = "files/leaderboard.txt";
    vector<int> times;
    //if the player won, puts their time into the file
    if (first) {
        if (win) {
            ofstream outp(path, ios::app);
            if (!outp.is_open()) {
                std::cerr << "Error opening file" << std::endl;
                return longtext;
            }
            int minutes = endTime / 60;
            int seconds = endTime % 60;

            outp << "\n" << minutes << ":" << seconds << ", " << username;
            outp.close();
        }
    }
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        return longtext;
    }

    vector<int> hours;
    vector<int> minutes;
    vector<string> names;

    int specialIndex = -1;
    bool special = false;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string hStr, mStr, name;

        if (getline(ss, hStr, ':') && getline(ss, mStr, ',') && getline(ss, name)) {
            hours.push_back(stoi(hStr));
            minutes.push_back(stoi(mStr));
            name = name.substr(1);
            names.push_back(name);
        }
    }
    //make sure the special index doesn't get lost
    if (first && win) {
        specialIndex = hours.size()-1;
        special = true;
    }
    //reads the file, seperates them into hours : minutes, compares the hours to hours, then if they are the same compare the minutes, if its smaller than they swap essentially
    for (int i = 0; i < hours.size(); ++i) {
        for (int j = i + 1; j < hours.size(); ++j) {
            // Compare times
            if ((hours[j] < hours[i]) || (hours[j] == hours[i] && minutes[j] < minutes[i])) {
                // Swap hours
                int tempH = hours[i];
                hours[i] = hours[j];
                hours[j] = tempH;

                // Swap minutes
                int tempM = minutes[i];
                minutes[i] = minutes[j];
                minutes[j] = tempM;

                // Swap names
                string tempN = names[i];
                names[i] = names[j];
                names[j] = tempN;

                if (i == specialIndex)
                    specialIndex = j;
                else if (j == specialIndex)
                    specialIndex = i;
            }
        }
    }
    //since its only top 5, it takes either the first 5, or if the leaderboard txt has less than 5
    int five;
    if (hours.size() > 5) {
        five = 5;
    } else {
        five = hours.size();
    }
    // makes one really big string
    for (int i = 0; i < five; i++) {
        string shour = (hours[i] < 10 ? "0" : "") + to_string(hours[i]);
        string smins = (minutes[i] < 10 ? "0" : "") + to_string(minutes[i]);

        longtext += to_string(i + 1) + ".\t" + shour + ":" + smins + "\t" + names[i];

        // Optional: add asterisk for a new record (example)
        if (special && i == specialIndex) {
            longtext += "*";
        }

        longtext += "\n\n";
    }
    return longtext;
}

void LeaderboardScreen(sf::RenderWindow& window, const string& longtext, float width, float height) {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::EventType::Closed) {
                window.close();
                break;
            }
        }
        window.clear(sf::Color::Blue);
        WriteText(window, "LEADERBOARD", true, true, sf::Color::White, 20, width/2, ((height/2) - 120));
        WriteText(window, longtext, true, false, sf::Color::White, 18, width/2, height/2 + 20);
        window.display();
    }
}

void RecReveal(vector<Tile>& tiles, int x, int y, int col, int row, int& rcount) {
    int place = x * row + y;

    //base case
    if (tiles[place].isRevealed() || tiles[place].IsMine() || tiles[place].getHasFlag()) {
        return;
    }

    tiles[place].reveal();
    rcount++;

    if (tiles[place].getAdjMines() > 0) {
        return;
    }

    //adjacent areas to the tile, like bottom left, bottom mid, bottom right, etc.
    int aroundXes[] = {-1,0,1,-1,1,-1,0,1};
    int aroundYes[] = {-1,-1,-1,0,0,1,1,1};

    //neighboring tiles
    for (int i = 0; i < 8; i++) {
        int adjX = x + aroundXes[i];
        int adjY = y + aroundYes[i];

        //checks bounds of grid
        if (adjX >= 0 && adjX < col && adjY >= 0 && adjY < row) {
            RecReveal(tiles, adjX, adjY, col, row, rcount);
        }
    }
}

bool checkWin(vector<Tile>& tiles, int col, int row, int mines, int rCount, bool& win, bool& blockClick, bool& blockButton) {
    //sees if the revealed count = the number of non-mine spaces
    if (rCount == (col * row - mines)) {
        win = true;
        blockButton = true;
        blockClick = true;

        for (Tile& tile : tiles) {
            if (tile.IsMine() && !tile.getHasFlag()) {
                tile.toggleFlag();
            }
        }
        return true;
    }

    return false;
}

vector<int> counterDigits(int mines) {
    vector<int> counterDigits;
    bool neg = false;;
    if (mines < 0) {
        neg = true;
    }
    int tmines = abs(mines);

    //annoying minus sign
    if (neg) {
        counterDigits.push_back(10);
    }
    counterDigits.push_back(tmines / 100);
    counterDigits.push_back((tmines / 10) % 10);
    counterDigits.push_back(tmines % 10);

    return counterDigits;
}

vector<int> timerDigits(int ttime) {
    int minutes = ttime / 60;
    int seconds = ttime % 60;

    vector<int> digits;

    digits.push_back(minutes / 10);
    digits.push_back(minutes % 10);
    digits.push_back(seconds / 10);
    digits.push_back(seconds % 10);

    return digits;
}

void drawMineCount(sf::RenderWindow& window, const TextureManager& tex, int mines, int row) {
    vector<int> digits = counterDigits(mines);
    bool neg = false;
    if (digits[0] == 10) {
        neg = true;
    }
    int negative = 0;
    float y = 32 * (row + 0.5f) + 16;
    if (neg) {
        sf::Sprite minus;
        minus.setTexture(tex.dig(10));
        minus.setPosition(12,y);
        window.draw(minus);
        negative = 1;
    }

    for (int i = negative; i < digits.size(); i++) {
        sf::Sprite digit;
        digit.setTexture(tex.dig(digits[i]));
        digit.setPosition((33 + 21 * (i - negative)), y);
        window.draw(digit);
    }
}

void drawTimer(sf::RenderWindow& window, const TextureManager& tex, const int time, const int row, const int col) {
    vector<int> digits = timerDigits(time);
    float y = 32 * (row + 0.5f) + 16;

    float minX = (32 * col) - 97;
    float secX = (32 * col) - 54;

    for (int i = 0; i < 2; i++) {
        sf::Sprite digit;
        digit.setTexture(tex.dig(digits[i]));
        digit.setPosition(minX + (i * 21), y);
        window.draw(digit);
    }

    for (int i = 2; i < 4; i++) {
        sf::Sprite digit;
        digit.setTexture(tex.dig(digits[i]));
        digit.setPosition(secX + ((i-2) * 21), y);
        window.draw(digit);
    }
}

void GameScreen(sf::RenderWindow &window, TextureManager &text, int col, int row, int mines, int width, int height, string& username) {
    vector<sf::Texture> tilesforfunction;
    sf::Texture up_texture = text.text("hidden");
    sf::Texture down_texture = text.text("revealed");
    sf::Texture mine = text.text("mine");
    sf::Texture flag = text.text("flag");

    tilesforfunction.push_back(up_texture);
    tilesforfunction.push_back(down_texture);
    tilesforfunction.push_back(mine);
    tilesforfunction.push_back(flag);

    vector<sf::Texture> numbers;
    sf::Texture num1 = text.text("1");
    sf::Texture num2 = text.text("2");
    sf::Texture num3 = text.text("3");
    sf::Texture num4 = text.text("4");
    sf::Texture num5 = text.text("5");
    sf::Texture num6 = text.text("6");
    sf::Texture num7 = text.text("7");
    sf::Texture num8 = text.text("8");


    numbers.push_back(num1);
    numbers.push_back(num2);
    numbers.push_back(num3);
    numbers.push_back(num4);
    numbers.push_back(num5);
    numbers.push_back(num6);
    numbers.push_back(num7);
    numbers.push_back(num8);

    //time
    auto startTime = chrono::high_resolution_clock::now();
    int totalTime = 0;
    int showTime = 0;
    int totalPauseTime = 0;
    chrono::high_resolution_clock::time_point pauseStartTime;

    vector<Tile> tiles;

    generateRandom(tiles, row, col, mines, tilesforfunction, numbers);

    float tileSize = 32;
    float butY = (row + 0.5) * 32;
    float screenWidth = col * tileSize;

    float faceX = ((col / 2.0f) * tileSize) - tileSize;
    float debugX = screenWidth - (tileSize * 9.5f); //just changed it so it would work dynamically instead of through fixed numbers
    float pauseX = screenWidth - (tileSize * 7.5f);
    float leaderboardX = screenWidth - (tileSize * 5.5f);

    Button face(text.text("happy"), faceX, butY, "face");
    Button debug(text.text("debug"), debugX, butY, "debug");
    Button pause(text.text("pause"), pauseX, butY, "pause");
    Button leaderboard(text.text("leaderboard"), leaderboardX, butY, "leaderboard");

    vector<Button> buttons;
    buttons.emplace_back(face);
    buttons.emplace_back(debug);
    buttons.emplace_back(pause);
    buttons.emplace_back(leaderboard);

    bool blockClick = false;
    bool blockButton = false;
    bool debugMode = false;
    bool firstClick = true;
    bool win = false;
    bool paused = false;
    bool pauseButton = false;
    bool leaderscreen = false;
    int rCount = 0;
    bool fleader = true;
    int flagsPlaced = 0;
    bool actuallyPaused = false;


    // Mainloop
    while (window.isOpen()) {
        // Event loop
        if (leaderscreen) {
            height = ((row*16) + 50);
            width = (col * 16);

            sf::RenderWindow leader(sf::VideoMode(width, height), "Leaderboard Window");
            Window(leader, 3, row, col, (float)width, (float)height, username);
            string bigtext = LeaderboardInfo(username, win, fleader, showTime);

            auto now = chrono::high_resolution_clock::now();
            totalTime += chrono::duration_cast<chrono::seconds>(now - startTime).count(); // freeze timer

            LeaderboardScreen(leader, bigtext, width, height);
            fleader = false;

            startTime = chrono::high_resolution_clock::now();

            leaderscreen = false;
            paused = !paused;

            blockClick = false;
            blockButton = false;

        }
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close event
            if (event.type == sf::Event::EventType::Closed) {
                window.close();
                break;
            }
            if (event.type == sf::Event::EventType::MouseButtonPressed) {
                auto click = event.mouseButton;
                if (event.mouseButton.button == sf::Mouse::Right) {
                    for (Tile& tile: tiles) {
                        if (tile.get_sprite().getGlobalBounds().contains(click.x, click.y) && !blockClick) {
                            if (!tile.isRevealed()) {
                                bool Flag = tile.getHasFlag();
                                tile.toggleFlag();
                                if (!Flag) {
                                    flagsPlaced++;
                                } else {
                                    flagsPlaced--;
                                }
                            }
                        }
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (click.y < row * 32) {
                        for (Tile& tile: tiles) {
                            if (tile.get_sprite().getGlobalBounds().contains(click.x, click.y) && !blockClick) {
                                if (firstClick) {
                                    int firstClickedCol = click.x / 32;
                                    int firstClickedRow = click.y / 32;
                                    int firstClickedTile = firstClickedCol * row + firstClickedRow;
                                    generateRandom(tiles, row, col, mines, tilesforfunction, numbers, firstClickedTile);
                                    firstClick = false;
                                }
                                if (tile.IsMine()) {
                                    tile.click();
                                } else if (tile.getAdjMines() == 0) {
                                    int tileclickcol = click.x / 32;
                                    int tileclickrow = click.y / 32;
                                    RecReveal(tiles, tileclickcol, tileclickrow, col, row, rCount);
                                } else {
                                    if (!tile.isRevealed()) {
                                        tile.click();
                                        rCount++;
                                    }

                                }

                                if (checkWin(tiles, col, row, mines, rCount, win, blockClick, blockButton)) {
                                    buttons[0].setTexture(text.text("win"));
                                    debugMode = false;

                                    // LeaderboardInfo(username, true, true, showTime);
                                }
                                if (tile.didIlose()) {
                                    buttons[0].setTexture(text.text("lose"));
                                    debugMode = true;
                                    blockClick = true;
                                    blockButton = true;
                                }
                            }
                        }
                    } else {
                        for (Button& button: buttons) {
                            if (button.isClicked(click.x, click.y)) {
                                if (button.getName() == "face") {
                                    generateRandom(tiles, row, col, mines, tilesforfunction, numbers);
                                    buttons[0].setTexture(text.text("happy"));
                                    debugMode = false;
                                    blockClick = false;
                                    blockButton = false;
                                    actuallyPaused = false;
                                    firstClick = true;
                                    paused = false;
                                    buttons[2].setTexture(text.text("pause"));
                                    pauseButton = false;
                                    win = false;
                                    rCount = 0;
                                    fleader = true;
                                    flagsPlaced = 0;
                                    totalTime = 0;
                                    showTime = 0;
                                    startTime = chrono::high_resolution_clock::now();
                                }
                                if (button.getName() == "debug" && !blockButton && !pauseButton) {
                                    debugMode = !debugMode;
                                    if (debugMode) {
                                        cout << "debugMode turned on" << endl;
                                    } else {
                                        cout << "debugMode turned off" << endl;
                                    }
                                }
                                if (button.getName() == "pause" && !blockButton) {
                                    paused = !paused;
                                    if (paused) {
                                        actuallyPaused = true;
                                        buttons[2].setTexture(text.text("play"));

                                        auto now = chrono::high_resolution_clock::now();
                                        totalTime += chrono::duration_cast<chrono::seconds>(now - startTime).count();

                                        pauseStartTime = chrono::high_resolution_clock::now();
                                        cout << "..." << endl;
                                    } else {
                                        actuallyPaused = false;
                                        buttons[2].setTexture(text.text("pause"));

                                        auto now = chrono::high_resolution_clock::now();
                                        int pauseDuration = chrono::duration_cast<chrono::seconds>(now - pauseStartTime).count();
                                        totalPauseTime += pauseDuration;

                                        cout << "Paused for " << pauseDuration << " seconds. Total pause time: " << totalPauseTime << " seconds."<< endl;
                                        startTime = chrono::high_resolution_clock::now();

                                    }
                                    blockClick = !blockClick;
                                    pauseButton = !pauseButton;

                                }
                                if (button.getName() == "leaderboard") {
                                    paused = true;
                                    // if (!pauseButton) {
                                    blockClick = true;
                                    blockButton = true;
                                    // }
                                    leaderscreen = true;
                                }
                            }
                        }
                    }
                }
            }
        }
        if (!paused && !pauseButton && !blockClick && !win) {
            auto now = chrono::high_resolution_clock::now();
            showTime = totalTime + chrono::duration_cast<chrono::seconds>((now - startTime)).count();
        }
        // Render loop
        window.clear(sf::Color::White);
        for (Tile& tile: tiles) {
            tile.draw(window, debugMode, (actuallyPaused || paused));
        }
        drawMineCount(window, text, mines - flagsPlaced, row);
        drawTimer(window, text, showTime, row, col);
        for (Button& button: buttons) {
            button.draw(window);
        }
        window.display();
    }
}

int main() {
    string path = "files/config.cfg";
    ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }
    int col;
    int row;
    int mines;
    file >> col >> row >> mines;

    int width;
    int height;

    std::string input;
    std::string username;
    bool welcome = true;
    bool game = false;

    if (welcome) {
        width = col * 32;
        height = (row * 32) + 100;
        sf::RenderWindow window(sf::VideoMode(width, height), "Welcome Window");
        int usernamelength = 0;

        while(window.isOpen()) {
            sf::Event event;
            while(window.pollEvent(event)) {
                if(event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::TextEntered) {
                    if ((event.text.unicode >= 65 && event.text.unicode <= 122) && usernamelength < 10) {
                        username += event.text.unicode;
                        usernamelength++;
                    } else if (event.text.unicode == 8) {
                        if (usernamelength > 0) {
                            username.pop_back();
                            usernamelength--;
                        }
                    } else if (event.text.unicode == 13) {
                        if (usernamelength > 0) {
                            welcome = false;
                            game = true;
                            window.close();
                        }
                    }

                    username[0] = std::toupper(username[0]);
                    for (int i = 1; i < usernamelength; i++) {
                        username[i] = std::tolower(username[i]);
                    }
                }
            }

            Window(window, 1, row, col, width, height, username);

            window.display();
        }
    }
    if (game) {
        width = col * 32;
        height = (row * 32) + 100;
        sf::RenderWindow gameWindow(sf::VideoMode(width, height), "Game Window");
        Window(gameWindow, 2, row, col, width, height, username);
        TextureManager texture;
        GameScreen(gameWindow,texture, col, row, mines, width, height, username);
    }
    return 0;
}
