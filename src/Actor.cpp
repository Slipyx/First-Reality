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

#include "Actor.hpp"

Actor::Actor(sf::RenderWindow& app, Map* map)
{
    mApp = &app;
    mMap = map;
    tWidth = 0;
    tHeight = 0;
    mPos.x = 0.0f;
    mPos.y = 0.0f;
}

void Actor::Update(const float& dt)
{
}

void Actor::Draw()
{
}

sf::Vector2f Actor::GetPos()
{
    return mPos;
}

const unsigned char Actor::GetWidth()
{
    return tWidth;
}

const unsigned char Actor::GetHeight()
{
    return tHeight;
}

Actor::~Actor()
{
}
