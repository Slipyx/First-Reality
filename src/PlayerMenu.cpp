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
#include "ImageManager.hpp"
#include "FontManager.hpp"
#include <sstream>
#include <iomanip>

PlayerMenu::PlayerMenu(sf::RenderWindow& app, Player* player)
{
    mApp = &app;
    mPlayer = player;

    txtMenuItms.SetFont(FontManager::GetFont("DroidSans.ttf"));
    txtMenuItms.SetString("Items\nSkills\nEquip\nStats\nQuit");
    txtMenuItms.SetCharacterSize(10);
    txtMenuItms.SetColor(sf::Color(255, 255, 255));

    txtSteps.SetFont(FontManager::GetFont("DroidSans.ttf"));
    //txtSteps.SetString("foo");
    txtSteps.SetCharacterSize(10);
    txtSteps.SetColor(sf::Color(255, 255, 255));

    sprCursor.SetImage(ImageManager::GetImage("gui/menuCursor.png"));
    curSelection = 1;
}

void PlayerMenu::Keypressed(sf::Key::Code key)
{
    if(key == sf::Key::Down) {
        curSelection += 1;
        if(curSelection > 5) {
            curSelection = 1;
        }
    }
    else if(key == sf::Key::Up) {
        curSelection -= 1;
        if(curSelection < 1) {
            curSelection = 5;
        }
    }

    if(key == sf::Key::X) {
        if(curSelection == 5) {
            mApp->Close();
        }
    }
}

void PlayerMenu::Update(const float& dt)
{
    sf::View uiView = mApp->GetView();
    txtMenuItms.SetPosition(uiView.GetSize().x * 0.6666f + 4.0f, 2.0f);
    // Update Steps string
    std::stringstream ssSteps;
    ssSteps << "Time\n  0:" << std::setfill('0') << std::setw(2) << mPlayer->GetTime() / 60000
    << "\nSteps\n  " << mPlayer->GetSteps() << "\nGP\n  " << mPlayer->GetGP();
    txtSteps.SetString(ssSteps.str());
    txtSteps.SetPosition(uiView.GetSize().x * 0.6666f + 4.0f, uiView.GetSize().y * 0.6666f + 2.0f);
    // Update cursor position
    sprCursor.SetPosition(uiView.GetSize().x * 0.6666f - 16.0f, (curSelection - 1) * 12 + 2.0f);
}

void PlayerMenu::Draw()
{
    sf::View uiView = mApp->GetView();
    // Menu BG
    mApp->Draw(sf::Shape::Rectangle(2.0f, 2.0f, uiView.GetSize().x - 4, uiView.GetSize().y - 4, sf::Color(41, 40, 132), 2.0f, sf::Color(192, 192, 192)));
    // Menu items BG
    mApp->Draw(sf::Shape::Rectangle(uiView.GetSize().x * 0.6666f, 2.0f, uiView.GetSize().x * 0.3333f - 2.0f, uiView.GetSize().y * 0.6666f - 4.0f, sf::Color(41, 40, 132), 2.0f, sf::Color(192, 192, 192)));
    // Steps and GP BG
    mApp->Draw(sf::Shape::Rectangle(uiView.GetSize().x * 0.6666f, uiView.GetSize().y * 0.6666f, uiView.GetSize().x * 0.3333f - 2.0f, uiView.GetSize().y * 0.3333f - 2.0f, sf::Color(41, 40, 132), 2.0f, sf::Color(192, 192, 192)));
    mApp->Draw(txtMenuItms);
    mApp->Draw(txtSteps);
    // Cursor
    mApp->Draw(sprCursor);
}
