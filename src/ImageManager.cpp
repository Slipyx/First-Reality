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

#include "ImageManager.hpp"
#include <iostream>

std::map<std::string, sf::Image> mImages;

const sf::Image& ImageManager::GetImage(const std::string& file)
{
    for(std::map<std::string, sf::Image>::const_iterator it = mImages.begin(); it != mImages.end(); ++it) {
        if(file == it->first) {
            std::cout << "Using existing image...\n";
            return it->second;
        }
    }

    sf::Image image;
    if(image.LoadFromFile("images/" + file)) {
        mImages[file] = image;
        std::cout << "Loaded new image...\n";
        return mImages[file];
    }

    std::cout << "ERROR: Image not found!\n";
    mImages[file] = image;
    return mImages[file];
}
