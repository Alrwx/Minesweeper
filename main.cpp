#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "Tile.h"
#include <stdlib.h>
#include "TextureManager.h"
#include <vector>

#include "Button.h"

using namespace std;

void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

void WriteText(sf::RenderWindow& window, std::string text, bool bold, bool underline, sf::Color color, int size, float x, float y) {
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

    setText(word, x, y);

    window.draw(word);
}

void Window(sf::RenderWindow &window, int type, int row, int col, float width, float height, std::string& username) {
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
}

void generateRandom(vector<Tile>& tiles, int row, int col, int mines, vector<sf::Texture>& texters, vector<sf::Texture>& numbers, int startIndex = -1) {
    tiles.clear();

    for (int x = 0; x < col; x++) {
        for (int y = 0; y < row; y ++) {
            tiles.emplace_back(texters[0], texters[1], texters[2], texters[3], numbers, x * 32, y * 32);
        }
    }

    if (startIndex == -1) {
        return;
    }

    int totalTiles = col * row;
    int placedMines = 0;

    while (placedMines < mines) {
        int randIndex = std::rand() % totalTiles;

        if (randIndex != startIndex || !tiles[randIndex].IsMine()) {
            tiles[randIndex].toggleMine();
            placedMines++;
        }
    }
}

void GameScreen(sf::RenderWindow &window, TextureManager &text, int col, int row, int mines) {

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

    // Create the tiles
    vector<Tile> tiles;

    generateRandom(tiles, row, col, mines, tilesforfunction, numbers);

    float tileSize = 32;
    float butY = (row + 0.5) * 32;
    float screenWidth = col * tileSize;

    float faceX = (screenWidth / 2.0f) - tileSize;
    float debugX = screenWidth - (tileSize * 9.5f);
    float pauseX = screenWidth - (tileSize * 7.5f);
    float leaderboardX = screenWidth - (tileSize * 5.5f);

    Button face(text.text("happy"), faceX, butY, "face");
    Button debug(text.text("debug"), debugX, butY, "debug");
    Button pause(text.text("pause"), pauseX, butY, "pause");
    Button leaderboard(text.text("leaderboard"), leaderboardX, butY, "leaderboard");

    //useless for now
    // Button lose(text.text("lose"), faceX, butY, "lose");
    // Button winner(text.text("win"), faceX, butY, "win");

    vector<Button> buttons;
    buttons.emplace_back(face);
    buttons.emplace_back(debug);
    buttons.emplace_back(pause);
    buttons.emplace_back(leaderboard);

    bool blockClick = false;
    bool blockButton = false;
    bool debugMode = false;
    bool firstClick = true;

    // Mainloop
    while (window.isOpen()) {
        // Event loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close event
            if (event.type == sf::Event::EventType::Closed) {
                window.close();
                break;
            }
            if (event.type == sf::Event::EventType::MouseButtonPressed) {
                auto click = event.mouseButton;
                cout << click.x << " , " << click.y << endl;
                if (event.mouseButton.button == sf::Mouse::Right) {
                    for (Tile& tile: tiles) {
                        if (tile.get_sprite().getGlobalBounds().contains(click.x, click.y) && !blockClick) {
                            tile.toggleFlag();
                            cout << "Right click, flag placed" << endl;
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
                                    int firstClickedTile = firstClickedRow * col + firstClickedCol;
                                    generateRandom(tiles, row, col, mines, tilesforfunction, numbers, firstClickedTile);
                                    firstClick = false;
                                }
                                tile.click();
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
                                // cout << button.getName() << " was clicked" << endl;
                                if (button.getName() == "face") {
                                    generateRandom(tiles, row, col, mines, tilesforfunction, numbers);
                                    buttons[0].setTexture(text.text("happy"));
                                    debugMode = false;
                                    blockClick = false;
                                    blockButton = false;
                                    firstClick = true;
                                }
                                if (button.getName() == "debug" && !blockButton) {
                                    debugMode = !debugMode;
                                }
                                if (button.getName() == "pause" && !blockButton) {
                                    cout << button.getName() << " is working bub" << endl;
                                }
                                if (button.getName() == "leaderboard") {
                                    int b = 3;
                                    cout << button.getName() << " is working bub" << endl;
                                }
                            }
                        }
                    }
                }
            }
        }
        // Render loop
        window.clear(sf::Color::White);
        for (Tile& tile: tiles) {
            tile.draw(window, debugMode);
        }
        for (Button& button: buttons) {
            button.draw(window);
        }
        window.display();
    }
}

int main() {
    std::string path = "files/config.cfg";
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }
    int col;
    int row;
    int mines;
    file >> col >> row >> mines;
    std::cout << col << " " << row << " " << mines << std::endl;

    int width;
    int height;

    std::string input;
    std::string username;
    bool welcome = true;
    bool game = false;

    if (welcome) {
        width = col * 32;
        height = (row * 32) + 100;
        sf::RenderWindow window(sf::VideoMode(width, height), "Minesweeper");
        int usernamelength = 0;

        while(window.isOpen()) {
            sf::Event event;
            while(window.pollEvent(event)) {
                if(event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::TextEntered) {
                    std::cout << event.text.unicode << std::endl;
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
                            std::cout << username << std::endl;
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
        sf::RenderWindow gameWindow(sf::VideoMode(width, height), "Game Screen");
        Window(gameWindow, 2, row, col, width, height, username);
        TextureManager texture;
        GameScreen(gameWindow,texture, col, row, mines);
    }
    return 0;
}


