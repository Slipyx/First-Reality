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

#include "Perf.hpp"
#include <sstream>

Perf::Perf(sf::RenderWindow& app, sf::View& uiView)
{
    mApp = &app;
    mUiView = &uiView;
    font.LoadFromFile("fonts/DroidSans.ttf"); // TODO: Get from font manager
    txtFps.SetFont(font);
    txtFps.SetString("0 FPS");
    txtFps.SetCharacterSize(10);
    txtFps.SetPosition(mUiView->GetSize().x - txtFps.GetRect().Width - 2.0f, 0.0f); // Align on right hand of view
    txtFps.SetColor(sf::Color(255, 0, 0));
    // Reset fps timer when constructor is finished
    fpsUdateTimer.Reset();
}

void Perf::Update(const float& dt)
{
    if(fpsUdateTimer.GetElapsedTime() >= 1000)
    {
        float fps = 1.0f / dt;
        // Update fps string
        std::stringstream ssfps;
        ssfps << (int)fps << " FPS";
        txtFps.SetString(ssfps.str());
        txtFps.SetPosition(mUiView->GetSize().x - txtFps.GetRect().Width - 2.0f, 0.0f); // Re-align
        // Change color based on fps range
        if(fps >= 40)
            txtFps.SetColor(sf::Color(0, 255, 0));
        else if(fps >= 20)
            txtFps.SetColor(sf::Color(255, 255, 0));
        else
            txtFps.SetColor(sf::Color(255, 0, 0));
        fpsUdateTimer.Reset();
    }
}

void Perf::Draw()
{
    mApp->Draw(sf::Shape::Rectangle(mUiView->GetSize().x - txtFps.GetRect().Width - 5.0f, 0.0f,
                                    txtFps.GetRect().Width + 5.0f, txtFps.GetRect().Height + 2.0f, sf::Color(41, 41, 131, 127)));
    mApp->Draw(txtFps);
}

Perf::~Perf()
{
}
