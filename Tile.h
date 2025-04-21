#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;

// Using an enum to track state.
// Enums rename integers, so "Up" will be replaced by '0' when the code compiles, and "Down" will be replaced with '1'
enum TileState {
    Up,
    Down
};

class Tile {
    // Store pointers to texture to avoid storing a copy (it can get very expensive very quickly!)
    const sf::Texture* up_texture;
    const sf::Texture* down_texture;
    sf::Sprite sprite;
    TileState state;

public:
    // Pass textures by reference and store by pointer to avoid copying the textures.
    // Pass x and y to indicate where on the screen the tile should appear
    Tile(const sf::Texture& up_texture, const sf::Texture& down_texture, int x, int y) {
        // Set the texture pointers
        this->up_texture = &up_texture;
        this->down_texture = &down_texture;
        // Set sprite to 'up' state and texture
        this->sprite = sf::Sprite(up_texture);
        this->state = Up;
        // Set the sprite's position on the screen
        sprite.setPosition(x, y);
    }
    // Getter for the sprite
    sf::Sprite& get_sprite() {
        return this->sprite;
    }
    // Function which outlines the logic of a tile being clicked
    void click() {
        // Check the current state and flip to the opposite state
        if (state == Up) {
            sprite.setTexture(*down_texture);
            state = Down;
        }
        else { // State is down
            sprite.setTexture(*up_texture);
            state = Up;
        }
    }
};

int main() {
    // Create the window
    sf::RenderWindow window;
    int window_width = 640;
    int window_height = 480;
    window.create(sf::VideoMode(window_width, window_height), "demo window");

    // Load the texture files
    sf::Texture up_texture;
    sf::Texture down_texture;


    up_texture.loadFromFile("../tile_hidden.png");
    down_texture.loadFromFile("../tile_revealed.png");

    // Create the tiles
    vector<Tile> tiles;
    for (int x = 0; x < window_width; x += 32) { // Iterating by 32 because the tile PNGs are 32 pixels wide
        for (int y = 0; y < window_height; y += 32) { // Iterating by 32 because the tile PNGs are 32 pixels tall
            // x and y correspond to where on the screen the top-left corner of the tile sprite should be
            tiles.emplace_back(up_texture, down_texture, x, y);
        }
    }

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
            // Click event
            if (event.type == sf::Event::EventType::MouseButtonPressed) {
                // Get click data
                auto click = event.mouseButton;
                // See if the click intersects any tiles. If so, run their click() function
                for (Tile& tile: tiles) {
                    // getGlobalBounds().contains(x, y) tests if (x, y) touches the sprite
                    if (tile.get_sprite().getGlobalBounds().contains(click.x, click.y)) {
                        tile.click(); // Click function as defined in the tile class
                    }
                }
            }
        }
        // Render loop
        window.clear(sf::Color::Blue);
        for (Tile& tile: tiles) {
            window.draw(tile.get_sprite());
        }
        window.display();
    }
}
