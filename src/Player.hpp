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

#ifndef PLAYER_H
#define PLAYER_H

#include "Actor.hpp"
#include "Animation.hpp"

enum eFacingDir {
    FACING_DOWN, FACING_UP, FACING_LEFT, FACING_RIGHT
};

class Player : public Actor
{
public:
    Player(sf::RenderWindow& app, Map* map, float startX, float startY);
    virtual ~Player();

    virtual void Update(const float& dt);
    void UpdateTimer(const float& dt);
    virtual void Draw();

    // Gets and Sets
    unsigned int GetTime() { return mTime; }
    unsigned short GetSteps() { return mSteps; }
    unsigned int GetGP() { return mGP; }

private:
    sf::Image imgSheet;
    sf::Sprite sprPlayer;
    sf::IntRect* rectMap;
    sf::Vector2f mSpeed;
    sf::Vector2f mTargetPos;
    eFacingDir mFacingDir;
    // Stats
    unsigned int mTime;
	unsigned short mSteps;
	unsigned int mGP;

    std::vector<Animation*> animSet;
    Animation* curAnim;

    void UpdateMovement(const float& dt);
    void SwitchIdleAnimation();
    bool CheckCollisions();
};

#endif
