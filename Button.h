#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

using namespace std;

class Button {
    const string name;
    sf::Sprite sprite;
    const sf::Texture* texture;
    float x, y;

public:
    Button(const sf::Texture& tex, float x, float y, const string& name) : texture(&tex), x(x), y(y), name(name)
    {
        sprite.setTexture(*texture);
        sprite.setPosition(x, y);
    }

    sf::Sprite& getSprite() {
        return sprite;
    }

    string getName() const {
        return name;
    }

    void setTexture(const sf::Texture& tex) {
        texture = &tex;
        sprite.setTexture(*texture);
    }

    bool isClicked(float mX, float mY) const {
        return sprite.getGlobalBounds().contains(mX, mY);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
};