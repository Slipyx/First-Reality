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

#include "Animation.hpp"

Animation::Animation(std::string name, char start, char end, char rate, std::string mode)
{
    mName = name;
    mStart = start;
    mEnd = end;
    mRate = rate;
    cur = start;
    mMode = mode;
    bForward = true;
}

void Animation::Update(const float& dt)
{
    if(mMode == "loop") {
        cur += (mRate * dt);
        if(cur >= mEnd + 1) {
            cur = mStart;
        }
    }
    else if(mMode == "pong") {
        if(bForward) {
            cur += (mRate * dt);
            if(cur >= mEnd + 1) {
                cur = mEnd;
                bForward = false;
            }
        }
        else {
            cur -= (mRate * dt);
            if(cur <= mStart) {
                cur = float(mStart) + 1.0f;
                bForward = true;
            }
        }
    }
    else if(mMode == "once") {
        cur += (mRate * dt);
        if(cur >= mEnd + 1) {
            cur = mEnd;
        }
    }
}

int Animation::GetCur()
{
    return int(cur);
}

Animation::~Animation()
{
}
