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

#include "Map.hpp"
#include "Player.hpp"
//#include "Randomizer.hpp"
#include <iostream>
#include "irrxml/irrXML.h"

Map::Map(sf::RenderWindow& app, sf::View& gView, std::string fileName)
{
    sf::Clock mapLoadTimer;
    mApp = &app;
    mGView = &gView;

    // XML parser object for map file
    irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(("maps/" + fileName + ".xml").c_str());
    std::string BGData;
    std::string FGData;
    std::string ColData;
    int pStartX = 0, pStartY = 0;
    // ********************************
    // Parse the map file!
    while(xml && xml->read()) {
        switch(xml->getNodeType()) {
        case irr::io::EXN_ELEMENT:
            if(!strcmp("map", xml->getNodeName())) {
                width = xml->getAttributeValueAsInt("width");
                height = xml->getAttributeValueAsInt("height");
            }
            else if(!strcmp("tileset", xml->getNodeName())) {
                std::string tilesetFile = xml->getAttributeValue("file");
                imgTileset.LoadFromFile("images/" + tilesetFile);
                sprTile.SetImage(imgTileset);
            }
            else if(!strcmp("playerstart", xml->getNodeName())) {
                pStartX = xml->getAttributeValueAsInt("x");
                pStartY = xml->getAttributeValueAsInt("y");
            }
            else if(!strcmp("layer", xml->getNodeName())) {
                if(!strcmp("Background", xml->getAttributeValue("name"))) {
                    BGData = xml->getAttributeValue("data");
                }
                else if(!strcmp("Foreground", xml->getAttributeValue("name"))) {
                    FGData = xml->getAttributeValue("data");
                }
                else if(!strcmp("Collision", xml->getAttributeValue("name"))) {
                    ColData = xml->getAttributeValue("data");
                }
            }
            break;
        }
    }
    // Delete XML parser object
    delete xml;
    // Done parsing!
    // ********************************
    // Load in rectmap using tileset image size and map's tile size
    rectMap = new sf::IntRect[(imgTileset.GetWidth() / TILE_SIZE) * (imgTileset.GetHeight() / TILE_SIZE)];
    for(unsigned int ry = 0; ry < imgTileset.GetHeight(); ry += TILE_SIZE) {
        for(unsigned int rx = 0; rx < imgTileset.GetWidth(); rx += TILE_SIZE) {
            rectMap[(ry / TILE_SIZE) * (imgTileset.GetHeight() / TILE_SIZE) + (rx / TILE_SIZE)] = sf::IntRect(rx, ry, TILE_SIZE, TILE_SIZE);
        }
    }
    // ********************************
    // Load in layer data
    layerBG = new unsigned char[width * height];
    layerFG = new unsigned char[width * height];
    layerCol = new unsigned char[width * height];
    for(int i = 0; i < width * height; ++i) {
        // BG
        if(BGData.at(0) == '\n')
            BGData.erase(0, 1);
        layerBG[i] = atoi(BGData.substr(0, BGData.find(',')).c_str());
        BGData.erase(0, BGData.find(',') + 1);
        // FG
        if(FGData.at(0) == '\n')
            FGData.erase(0, 1);
        layerFG[i] = atoi(FGData.substr(0, FGData.find(',')).c_str());
        FGData.erase(0, FGData.find(',') + 1);
        // Collision
        if(ColData.at(0) == '\n')
            ColData.erase(0, 1);
        layerCol[i] = atoi(ColData.substr(0, ColData.find(',')).c_str());
        ColData.erase(0, ColData.find(',') + 1);
    }

    // ********************************
    // Load and start player
    player = new Player(app, this, float(pStartX), float(pStartY)); // (Exact grid points) Please don't use decimals here :D
    // Center view on player
    mGView->SetCenter(player->GetPos());

    std::cout << "Map loaded in " << mapLoadTimer.GetElapsedTime() << " ms!\n";
}

void Map::Update(const float& dt)
{
    player->Update(dt);
    // Constrain game view to map boundries
    sf::Vector2f boundedCenter = player->GetPos();
    float gViewHalfWidth = mGView->GetSize().x / 2.0f;
    float gViewHalfHeight = mGView->GetSize().y / 2.0f;

    // X left
    if(boundedCenter.x < gViewHalfWidth)
        boundedCenter.x = gViewHalfWidth;
    // X right
    else if(boundedCenter.x > width * TILE_SIZE - gViewHalfWidth)
        boundedCenter.x = width * TILE_SIZE - gViewHalfWidth;
    // Y top
    if(boundedCenter.y < gViewHalfHeight)
        boundedCenter.y = gViewHalfHeight;
    // Y bottom
    else if(boundedCenter.y > height * TILE_SIZE - gViewHalfHeight)
        boundedCenter.y = height * TILE_SIZE - gViewHalfHeight;

    mGView->SetCenter(boundedCenter);
}

void Map::Draw()
{
    DrawLayer(layerBG);
    DrawLayer(layerFG, 1);
    player->Draw();
    DrawLayer(layerFG, 2);
    //DrawCollisions();
    //DrawGrid();
}

bool Map::TileIsBehindActor(unsigned int tx, unsigned int ty, Actor* actor)
{
    sf::IntRect tileRect(tx, ty, TILE_SIZE, TILE_SIZE);
    sf::IntRect actorRect(int(actor->GetPos().x) - actor->GetWidth() / 2, int(actor->GetPos().y) - actor->GetHeight(), actor->GetWidth(), actor->GetHeight() - TILE_SIZE / 2);
    if(actorRect.Intersects(tileRect)) {
        return true;
    }
    else {
        return false;
    }
}

void Map::DrawLayer(unsigned char* layer, char mode)
{
    // Used for rendering tiles within game view
    sf::Vector2f gViewCenter = mGView->GetCenter();
    float tileViewHalfWidth = mGView->GetSize().x / 2.0f + TILE_SIZE;
    float tileViewHalfHeight = mGView->GetSize().y / 2.0f + TILE_SIZE;

    for(int i = 0; i < width * height; ++i) {
        if(layer[i]) {
            unsigned int txPos = i % width * TILE_SIZE;
            unsigned int tyPos = i / width * TILE_SIZE;
            // Only render if tile is within game view
            if(txPos > gViewCenter.x - tileViewHalfWidth &&
               txPos < gViewCenter.x + tileViewHalfWidth &&
               tyPos > gViewCenter.y - tileViewHalfHeight &&
               tyPos < gViewCenter.y + tileViewHalfHeight) {
                   bool bDrawIt = true;
                   // Draw all tiles
                   if(mode == 0) {}
                   // Draw only behind actors
                   else if(mode == 1) {
                       if(!TileIsBehindActor(txPos, tyPos, player)) {
                            bDrawIt = false;
                       }
                   }
                   // Draw all except behind actors
                   else if(mode == 2) {
                       if(TileIsBehindActor(txPos, tyPos, player)) {
                            bDrawIt = false;
                       }
                   }
                   // Draw it
                   if(bDrawIt) {
                       sprTile.SetSubRect(rectMap[layer[i] - 1]);
                       sprTile.SetPosition(float(txPos), float(tyPos));
                       mApp->Draw(sprTile);
                   }
            }
        }
    }
}

void Map::DrawGrid()
{
    // Vertical lines
    for(unsigned short x = 0; x < width; ++x) {
        mApp->Draw(sf::Shape::Line(float(x) * TILE_SIZE, 0.0f, float(x) * TILE_SIZE, float(height) * TILE_SIZE, 1.0f, sf::Color(255, 0, 0, 127)));
    }
    // Horizontal lines
    for(unsigned short y = 0; y < height; ++y) {
        mApp->Draw(sf::Shape::Line(0.0f, float(y) * TILE_SIZE, float(width) * TILE_SIZE, float(y) * TILE_SIZE, 1.0f, sf::Color(255, 0, 0, 127)));
    }
}

void Map::DrawCollisions()
{
    // Used for rendering tiles within game view
    sf::Vector2f gViewCenter = mGView->GetCenter();
    float tileViewHalfWidth = mGView->GetSize().x / 2.0f + TILE_SIZE;
    float tileViewHalfHeight = mGView->GetSize().y / 2.0f + TILE_SIZE;

    for(int i = 0; i < width * height; ++i) {
        if(layerCol[i]) {
            unsigned int txPos = i % width * TILE_SIZE;
            unsigned int tyPos = i / width * TILE_SIZE;
            // Only render if tile is within game view
            if(txPos > gViewCenter.x - tileViewHalfWidth &&
               txPos < gViewCenter.x + tileViewHalfWidth &&
               tyPos > gViewCenter.y - tileViewHalfHeight &&
               tyPos < gViewCenter.y + tileViewHalfHeight) {
                   // Full tile
                   if(layerCol[i] == 1)
                       mApp->Draw(sf::Shape::Rectangle(float(txPos) + 1.0f, float(tyPos) + 1.0f, TILE_SIZE - 2, TILE_SIZE - 2, sf::Color(0,0,0,0), 1.0f, sf::Color(173, 255, 47, 127)));
                   // Bottom collision tile
                   else if(layerCol[i] == 2)
                       mApp->Draw(sf::Shape::Line(float(txPos), float(tyPos + TILE_SIZE - 1), float(txPos + TILE_SIZE), float(tyPos + TILE_SIZE - 1), 1.0f, sf::Color(173, 255, 47, 127)));
                   // Top collision tile
                   else if(layerCol[i] == 3)
                       mApp->Draw(sf::Shape::Line(float(txPos), float(tyPos), float(txPos + TILE_SIZE), float(tyPos), 1.0f, sf::Color(173, 255, 47, 127)));
            }
        }
    }
}

unsigned char Map::GetWidth()
{
    return width;
}

unsigned char Map::GetHeight()
{
    return height;
}

const unsigned char* Map::GetCollisionLayer()
{
    return layerCol;
}

Map::~Map()
{
    delete player;
    delete[] layerCol;
    delete[] layerFG;
    delete[] layerBG;
    delete[] rectMap;
}
