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

#include "Randomizer.hpp"
#include <ctime>

static unsigned long s1 = 0xFFFFFF;
static unsigned long s2 = 0xCCCCCC;
static unsigned long s3 = 0xFF00FF;

static int FastFloor(double x)
{
    return x > 0 ? (int)x : (int)x - 1;
}

void Randomizer::Seed()
{
    s1 = (69069 * time(NULL)) & 0xFFFFFFFF;
    if(s1 < 2) s1 += 2;
    s2 = (69069 * s1) & 0xFFFFFFFF;
    if(s2 < 8) s2 += 8;
    s3 = (69069 * s2) & 0xFFFFFFFF;
    if(s3 < 16) s3 += 16;
}

double Randomizer::RandomUnit()
{
    s1 = ((s1 & 4294967294) << 12) ^ (((s1 << 13) ^ s1) >> 19);
    s2 = ((s2 & 4294967288) << 4) ^ (((s2 << 2) ^ s2) >> 25);
    s3 = ((s3 & 4294967280) << 17) ^ (((s3 << 3) ^ s3) >> 11);
    //double r = (s1 ^ s2 ^ s3) * 2.3283064365e-10;
    //return r < 0.0 ? r += 1 : r;
    return (s1 ^ s2 ^ s3) * 2.3283064365e-10;
}

double Randomizer::RandomSymmetric()
{
    return 2.0 * RandomUnit() - 1.0;
}

double Randomizer::RandomRange(double rMin, double rMax)
{
    return (rMax - rMin) * RandomUnit() + rMin;
}

int Randomizer::RandomRange(int rMin, int rMax)
{
    return static_cast<int>(FastFloor(RandomUnit() * (rMax - rMin + 1)) + rMin);
}
