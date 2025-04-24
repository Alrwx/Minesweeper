#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>


using namespace std;

enum TileState {
    Up,
    Down,
    Bomb,
    Flag
};

class Tile {
    const sf::Texture* up_texture;
    const sf::Texture* down_texture;
    const sf::Texture* mine_texture;
    const sf::Texture* flag_texture;
    vector<sf::Texture>* number_textures;

    sf::Sprite baseSprite;
    sf::Sprite flagSprite;
    sf::Sprite mineSprite;
    sf::Sprite numberSprite;
    TileState state;

    bool isMine = false;
    bool hasFlag = false;
    int adjacentMines;
    bool lose = false;

    int x, y;

public:
    Tile(const sf::Texture& up, const sf::Texture& down, const sf::Texture& mine, const sf::Texture& flag, vector<sf::Texture>& numbers, int x, int y)
        : up_texture(&up), down_texture(&down), mine_texture(&mine), flag_texture(&flag), number_textures(&numbers), x(x), y(y)
    // ,isMine(isMine), hasFlag(hasFlag), state(Up), adjacentMines(0)
    {
        this->state = Up;
        baseSprite.setTexture(*up_texture);
        baseSprite.setPosition(x, y);

        flagSprite.setTexture(*flag_texture);
        flagSprite.setPosition(x, y);

        mineSprite.setTexture(*mine_texture);
        mineSprite.setPosition(x, y);
    }
    sf::Sprite& get_sprite() {
        return baseSprite;
    }
    sf::Sprite& get_flagSprite() {
        return flagSprite;
    }
    sf::Sprite& get_MineSprite() {
        return mineSprite;
    }
    sf::Sprite& get_numSprite() {
        return numberSprite;
    }

    TileState getState() {
        return state;
    }

    bool IsMine() {
        return isMine;
    }

    bool getHasFlag() {
        return hasFlag;
    }

    void click() {
        if (state == Up && !hasFlag) {
            baseSprite.setTexture(*down_texture);
            if (isMine) {
                mineSprite.setTexture(*mine_texture);
            }
            state = Down;
        }
        if (isMine && !hasFlag) {
            baseSprite.setTexture(*mine_texture);
            state = Down;
            lose = true;
        }
    }

    bool didIlose() {
        return lose;
    }

    void toggleFlag() {
        if (state == Up) {
            hasFlag = !hasFlag;
        }
    }

    void toggleMine() {
        isMine = !isMine;
    }

    void setAdjMines(int count) {
        adjacentMines = count;
    }

    void draw(sf::RenderWindow& window, bool debugMode) {
        window.draw(baseSprite);

        if (state == Up && hasFlag) {
            window.draw(flagSprite);
        }

        if (state == Down) {
            if (isMine) {
                window.draw(mineSprite);
            } else if (adjacentMines > 0) {
                window.draw(numberSprite);
            }
            // Nothing extra to draw for adjacentMines == 0
        }

        if (debugMode && isMine) {
            window.draw(mineSprite);
        }

    }
};