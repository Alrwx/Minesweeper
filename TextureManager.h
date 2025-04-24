#pragma once
#include <SFML/Graphics.hpp>
#include <map>

using namespace std;

class TextureManager {
private:
    map<string, sf::Texture> textures;
public:
    TextureManager() {
        sf::Texture debug;
        debug.loadFromFile("files/images/debug.png");
        textures["debug"] = debug;
        sf::Texture digits;
        digits.loadFromFile("files/images/digits.png");
        textures["digits"] = digits;
        sf::Texture face_happy;
        face_happy.loadFromFile("files/images/face_happy.png");
        textures["happy"] = face_happy;
        sf::Texture face_lose;
        face_lose.loadFromFile("files/images/face_lose.png");
        textures["lose"] = face_lose;
        sf::Texture face_win;
        face_win.loadFromFile("files/images/face_win.png");
        textures["win"] = face_win;
        sf::Texture flag;
        flag.loadFromFile("files/images/flag.png");
        textures["flag"] = flag;
        sf::Texture leaderboard;
        leaderboard.loadFromFile("files/images/leaderboard.png");
        textures["leaderboard"] = leaderboard;
        sf::Texture mine;
        mine.loadFromFile("files/images/mine.png");
        textures["mine"] = mine;
        sf::Texture number_1;
        number_1.loadFromFile("files/images/number_1.png");
        textures["1"] = number_1;
        sf::Texture number_2;
        number_2.loadFromFile("files/images/number_2.png");
        textures["2"] = number_2;
        sf::Texture number_3;
        number_3.loadFromFile("files/images/number_3.png");
        textures["3"] = number_3;
        sf::Texture number_4;
        number_4.loadFromFile("files/images/number_4.png");
        textures["4"] = number_4;
        sf::Texture number_5;
        number_5.loadFromFile("files/images/number_5.png");
        textures["5"] = number_5;
        sf::Texture number_6;
        number_6.loadFromFile("files/images/number_6.png");
        textures["6"] = number_6;
        sf::Texture number_7;
        number_7.loadFromFile("files/images/number_7.png");
        textures["7"] = number_7;
        sf::Texture number_8;
        number_8.loadFromFile("files/images/number_8.png");
        textures["8"] = number_8;
        sf::Texture pause;
        pause.loadFromFile("files/images/pause.png");
        textures["pause"] = pause;
        sf::Texture play;
        play.loadFromFile("files/images/play.png");
        textures["play"] = play;
        sf::Texture tile_hidden;
        tile_hidden.loadFromFile("files/images/tile_hidden.png");
        textures["hidden"] = tile_hidden;
        sf::Texture tile_revealed;
        tile_revealed.loadFromFile("files/images/tile_revealed.png");
        textures["revealed"] = tile_revealed;
    }
    const sf::Texture& text(const string& name) const {
        return textures.at(name);
    }
};