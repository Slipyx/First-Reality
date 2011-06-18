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

#include "PlayerMenu.hpp"
#include "Player.hpp"
#include "FontManager.hpp"
#include <sstream>

PlayerMenu::PlayerMenu(sf::RenderWindow& app, Player* player)
{
    mApp = &app;
    mPlayer = player;

    txtSteps.SetFont(FontManager::GetFont("DroidSans.ttf"));
    txtSteps.SetString("Steps: 0");
    txtSteps.SetCharacterSize(10);
    txtSteps.SetColor(sf::Color(255, 255, 255));
}

void PlayerMenu::Update(const float& dt)
{
    sf::View uiView = mApp->GetView();
    // Update Steps string
    std::stringstream ssSteps;
    ssSteps << "Steps: " << mPlayer->GetSteps();
    txtSteps.SetString(ssSteps.str());
    txtSteps.SetPosition(uiView.GetSize().x - txtSteps.GetRect().Width - 2.0f, uiView.GetSize().y - txtSteps.GetRect().Height - 2.0f);
}

void PlayerMenu::Draw()
{
    sf::View uiView = mApp->GetView();
    mApp->Draw(sf::Shape::Rectangle(0.0f, 0.0f, uiView.GetSize().x, uiView.GetSize().y, sf::Color(0, 0, 0)));
    mApp->Draw(sf::Shape::Line(uiView.GetSize().x * 0.75f, 0.0f, uiView.GetSize().x * 0.75f, uiView.GetSize().y, 1.0f, sf::Color(255, 255, 255)));
    mApp->Draw(txtSteps);
}
