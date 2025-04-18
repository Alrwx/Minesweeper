#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>

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
        window.clear(sf::Color::White
            );
        WriteText(window, "Game Screen...", true, true, sf::Color::Black, 28, (width/2), ((height/2)-250));
        WriteText(window, ("Hello " + username), true, false, sf::Color::Black, 30, (width/2), ((height/2)-150));

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
    bool leader = false;

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
        sf::RenderWindow gameWindow(sf::VideoMode(width, height), "Game");

        while (gameWindow.isOpen()) {
            sf::Event event;
            while (gameWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    gameWindow.close();
                }
            }
            Window(gameWindow, 2, row, col, width, height, username);

            gameWindow.display();
        }
    }
    return 0;
}