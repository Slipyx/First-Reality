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

#include "Game.hpp"
#include "Randomizer.hpp"
#include <iostream>

// ****************************************************************************
// Initialize core Game members and load starting map
Game::Game(int appWidth, int appHeight)
{
    // Seed random
    Randomizer::Seed(double(time(NULL)));
    // Init window
    app.Create(sf::VideoMode(appWidth, appHeight, 32), "Project First Reality", sf::Style::Close);
    app.SetFramerateLimit(60);
    app.EnableVerticalSync(true);
    app.EnableKeyRepeat(false);

	// Initial game view
    gView.SetSize(320.0f, 240.0f);
    gView.SetCenter(160.0f, 120.0f);
    // UI view will never move
    uiView = gView;

    // Performace stats
    perf = new Perf(app, uiView);

    // Load initial map
    map = new Map(app, gView, "map01");
}

// ****************************************************************************
// Start main loop
void Game::Run()
{
    while(app.IsOpened())
    {
        dt = app.GetFrameTime() / 1000.0f;

        Update(dt);
        Draw();
    }
}

// ****************************************************************************
// Update logic and input
void Game::Update(const float& dt)
{
	// Poll one-shot events
    while(app.PollEvent(event))
    {
        if(event.Type == sf::Event::Closed)
            app.Close();
        if(event.Type == sf::Event::KeyPressed)
        {
            // Fire keypressed callbacks
            map->Keypressed(event.Key.Code);
            if(event.Key.Code == sf::Keyboard::A)
            {
                std::cout << Randomizer::Random(0, 255) << '\n';
            }
        }
    }

    // Update other objects
    map->Update(dt);
    perf->Update(dt);
}

// ****************************************************************************
// Clear screen and draw to display
void Game::Draw()
{
	// Clear color buffer
    //app.Clear(sf::Color(0, 127, 255)); // Don't need to clear when drawing over whole view

    // Set game view
    app.SetView(gView);

	// Draw game objects
	map->Draw();

	// Set UI view
    app.SetView(uiView);

	// Draw UI objects
    map->DrawUI();
    perf->Draw();

	// Swap buffers
    app.Display();
}

// ****************************************************************************
// Cleanup game objects
Game::~Game()
{
    delete map;
    delete perf;
}
