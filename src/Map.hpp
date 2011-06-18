/*
 * Copyright (c) 2011 Josh Koch
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 */

#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include "Actor.hpp"
#include "PlayerMenu.hpp"

class Actor;
class Player;
class PlayerMenu;

class Map
{
public:
    Map(sf::RenderWindow& app, sf::View& gView, std::string fileName);
    ~Map();

    static const unsigned char TILE_SIZE = 16;

    void Keypressed(sf::Key::Code key);
    void Update(const float& dt);
    void Draw();
    void DrawUI();
    unsigned char GetWidth();
    unsigned char GetHeight();
    const unsigned char* GetCollisionLayer();

private:
    sf::RenderWindow* mApp;
    sf::View* mGView;

    unsigned char width, height;

    sf::Image imgTileset;
    sf::Sprite sprTile;
    sf::IntRect* rectMap;
    unsigned char* layerBG;
    unsigned char* layerFG;
    unsigned char* layerCol;

    Player* player;
    PlayerMenu* playerMenu;
    bool bShowPlayerMenu;

    bool TileIsBehindActor(unsigned int tx, unsigned int ty, Actor* actor);
    void DrawLayer(unsigned char* layer, char mode = 0); // 0 = all tiles, 1 = only behind actors, 2 = all except behind actors
    void DrawGrid();
    void DrawCollisions();
};

#endif
