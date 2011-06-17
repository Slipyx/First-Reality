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

#include "Player.hpp"
#include "irrxml/irrXML.h"
#include <iostream>

Player::Player(sf::RenderWindow& app, Map* map, float startX, float startY) : Actor(app, map)
{
    imgSheet.LoadFromFile("images/playerSheet.png");
    sprPlayer.SetImage(imgSheet);
    tWidth = 16;
    tHeight = 24;
    rectMap = new sf::IntRect[(imgSheet.GetWidth() / tWidth) * (imgSheet.GetHeight() / tHeight)];
    for(unsigned int ry = 0; ry < imgSheet.GetHeight(); ry += tHeight) {
        for(unsigned int rx = 0; rx < imgSheet.GetWidth(); rx += tWidth) {
            rectMap[(ry / tHeight) * (imgSheet.GetHeight() / tHeight) + (rx / tWidth)] = sf::IntRect(rx, ry, tWidth, tHeight);
        }
    }

    mFacingDir = FACING_DOWN;
    mSpeed.x = 64.0f, mSpeed.y = 64.0f;
    mPos.x = startX, mPos.y = startY;
    mTargetPos = mPos;
	mSteps = 0;
    sprPlayer.SetPosition(mPos + sf::Vector2f(-(tWidth / 2.0f), -float(tHeight)));

    // ********************************
    // Load animation set
    std::string animFile = "char";
    // XML parser object for animation file
    irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(("anims/" + animFile + ".xml").c_str());
    // ********************************
    // Parse the animation file!
    while(xml && xml->read()) {
        switch(xml->getNodeType()) {
        case irr::io::EXN_ELEMENT:
            if(!strcmp("animation", xml->getNodeName())) {
                std::string aName = xml->getAttributeValue("name");
                int aStart = xml->getAttributeValueAsInt("start");
                int aEnd = xml->getAttributeValueAsInt("end");
                int aRate = xml->getAttributeValueAsInt("rate");
                animSet.push_back(new Animation(aName, aStart, aEnd, aRate));
            }
            break;
        }
    }
    // Delete XML parser object
    delete xml;
    // Done parsing!

    SwitchIdleAnimation();
}

void Player::Update(const float& dt)
{
    curAnim->Update(dt);
    // Capture input system
    const sf::Input& mInput = mApp->GetInput();
    // Check for input and adjust target pos
    if(mPos == mTargetPos) {
        // Input
        if(mInput.IsKeyDown(sf::Key::Left)) {
            mTargetPos.x = mPos.x - Map::TILE_SIZE;
            mFacingDir = FACING_LEFT;
            sprPlayer.FlipX(false);
        }
        else if(mInput.IsKeyDown(sf::Key::Right)) {
            mTargetPos.x = mPos.x + Map::TILE_SIZE;
            mFacingDir = FACING_RIGHT;
            sprPlayer.FlipX(true);
        }
        else if(mInput.IsKeyDown(sf::Key::Up)) {
            mTargetPos.y = mPos.y - Map::TILE_SIZE;
            mFacingDir = FACING_UP;
        }
        else if(mInput.IsKeyDown(sf::Key::Down)) {
            mTargetPos.y = mPos.y + Map::TILE_SIZE;
            mFacingDir = FACING_DOWN;
        }
        else {
            SwitchIdleAnimation();
        }
    }
    // Move towards target position if not going to collide
    else if(!CheckCollisions()) {
        // Horizontal
        if(mPos.x > mTargetPos.x) { // Moving left
            mPos.x -= mSpeed.x * dt;
            if(mPos.x < mTargetPos.x) { mPos.x = mTargetPos.x; mSteps++; }
        }
        else if(mPos.x < mTargetPos.x) { // Moving right
            mPos.x += mSpeed.x * dt;
            if(mPos.x > mTargetPos.x) { mPos.x = mTargetPos.x; mSteps++; }
        }
        // Vertical
        if(mPos.y > mTargetPos.y) { // Moving up
            mPos.y -= mSpeed.y * dt;
            if(mPos.y < mTargetPos.y) { mPos.y = mTargetPos.y; mSteps++; }
        }
        else if(mPos.y < mTargetPos.y) { // Moving down
            mPos.y += mSpeed.y * dt;
            if(mPos.y > mTargetPos.y) { mPos.y = mTargetPos.y; mSteps++; }
        }
        // Switch animation
        switch(mFacingDir) {
        case FACING_DOWN:
            curAnim = animSet[3];
            break;
        case FACING_UP:
            curAnim = animSet[4];
            break;
        case FACING_LEFT:
        case FACING_RIGHT:
            curAnim = animSet[5];
            break;
        }
        //std::cout << "Player pos: " << mPos.x << ", " << mPos.y << '\n';
        sprPlayer.SetPosition(mPos + sf::Vector2f(-(tWidth / 2.0f), -float(tHeight)));
		//std::cout << "Steps: " << mSteps << '\n';
    }
}

void Player::Draw()
{
    sprPlayer.SetSubRect(rectMap[curAnim->GetCur()]);
    //sprPlayer.SetSubRect(rectMap[(mFacingDir > 2 ? 2 : mFacingDir) * (imgSheet.GetWidth() / tWidth)]);
    mApp->Draw(sprPlayer);
}

void Player::SwitchIdleAnimation()
{
    // Switch to idle animation depending on facing direction
    switch(mFacingDir) {
    case FACING_DOWN:
        curAnim = animSet[0];
        break;
    case FACING_UP:
        curAnim = animSet[1];
        break;
    case FACING_LEFT:
    case FACING_RIGHT:
        curAnim = animSet[2];
        break;
    }
}

bool Player::CheckCollisions()
{
    unsigned int tPosIndex = ((unsigned int(mTargetPos.y) * mMap->GetWidth() / Map::TILE_SIZE) + (unsigned int(mTargetPos.x) / Map::TILE_SIZE)) - mMap->GetWidth() / 2;
    unsigned int cPosIndex = 0;
    // Determine current pos index based on target pos index and facing direction
    switch(mFacingDir) {
    case FACING_DOWN:
        cPosIndex = tPosIndex - mMap->GetWidth();
        break;
    case FACING_UP:
        cPosIndex = tPosIndex + mMap->GetWidth();
        break;
    case FACING_LEFT:
        cPosIndex = tPosIndex + 1;
        break;
    case FACING_RIGHT:
        cPosIndex = tPosIndex - 1;
        break;
    }
    const unsigned char* mapColLayer = mMap->GetCollisionLayer();
    bool bCollide = false;

    // Check against map outter boundries
    if(mTargetPos.x < 0.0f || mTargetPos.x > mMap->GetWidth() * Map::TILE_SIZE ||
       mTargetPos.y < 0.0f || mTargetPos.y > mMap->GetHeight() * Map::TILE_SIZE) {
        bCollide = true;
	}
    // Check target position against map's collision layer
    // Full collision tile
    else if(mapColLayer[tPosIndex] == 1) {
        bCollide = true;
	}
    // Bottom collision tile
	else if(mapColLayer[tPosIndex] == 2 && mFacingDir == FACING_UP ||
         mapColLayer[cPosIndex] == 2 && mFacingDir == FACING_DOWN) {
        bCollide = true;
	}
	// Top collision tile
	else if(mapColLayer[tPosIndex] == 3 && mFacingDir == FACING_DOWN ||
         mapColLayer[cPosIndex] == 3 && mFacingDir == FACING_UP) {
	    bCollide = true;
	}
	// Right collision tile
	else if(mapColLayer[tPosIndex] == 4 && mFacingDir == FACING_LEFT ||
         mapColLayer[cPosIndex] == 4 && mFacingDir == FACING_RIGHT) {
        bCollide = true;
	}
	// Left collision tile
	else if(mapColLayer[tPosIndex] == 5 && mFacingDir == FACING_RIGHT ||
         mapColLayer[cPosIndex] == 5 && mFacingDir == FACING_LEFT) {
	    bCollide = true;
	}

    if(bCollide) {
        mTargetPos = mPos;
        SwitchIdleAnimation();
    }

    return bCollide;
}

Player::~Player()
{
    delete curAnim;
    for(unsigned int i = 0; i < animSet.size(); ++i)
        delete animSet[i];
    delete[] rectMap;
}
