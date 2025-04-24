#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "TextureManager.h"
// #include <iostream> testing out w/ cout


using namespace std;

enum TileState {
    Up,
    Down,
};

class Tile {

    const sf::Texture* up_texture;
    const sf::Texture* down_texture;
    const sf::Texture* mine_texture;
    const sf::Texture* flag_texture;
    vector<sf::Texture>* number_textures;

    sf::Sprite revealedSprite;
    sf::Sprite baseSprite;
    sf::Sprite flagSprite;
    sf::Sprite mineSprite;
    sf::Sprite numberSprite;
    TileState state;

    bool isMine = false;
    bool hasFlag = false;
    int adjacentMines;
    bool lose = false;
    // bool revealed = false;
    int x, y;

public:
    Tile(const sf::Texture& up, const sf::Texture& down, const sf::Texture& mine, const sf::Texture& flag, vector<sf::Texture>& numbers, int x, int y)
        : up_texture(&up), down_texture(&down), mine_texture(&mine), flag_texture(&flag), number_textures(&numbers), x(x), y(y)
    // ,isMine(isMine), hasFlag(hasFlag), state(Up), adjacentMines(0)
    {
        this->state = Up;

        revealedSprite.setTexture(*down_texture);
        revealedSprite.setPosition(x,y);

        baseSprite.setTexture(*up_texture);
        baseSprite.setPosition(x, y);

        flagSprite.setTexture(*flag_texture);
        flagSprite.setPosition(x, y);

        mineSprite.setTexture(*mine_texture);
        mineSprite.setPosition(x, y);

        numberSprite.setPosition(x, y);
    }
    sf::Sprite& get_sprite() {
        return baseSprite;
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
            if (isMine && !hasFlag) {
                mineSprite.setTexture(*mine_texture);
                state = Down;
                lose = true;
            }
            state = Down;
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

    void draw(sf::RenderWindow& window, bool debugMode, bool paused) {
        window.draw(baseSprite);

        if (state == Up && hasFlag) {
            window.draw(flagSprite);
        }

        else if (state == Down) {
            if (isMine) {
                window.draw(mineSprite);
            } else if (adjacentMines > 0) {
                numberSprite.setTexture((*number_textures)[adjacentMines-1]);
                window.draw(numberSprite);
            }
            // Nothing extra to draw for adjacentMines == 0
        }

        if (debugMode && isMine) {
            window.draw(mineSprite);
        }

        if (paused) {
            window.draw(revealedSprite);
        }

    }
};