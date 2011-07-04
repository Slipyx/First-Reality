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
#include <stdint.h>
#include <cmath>

static struct RandomState {
    uint64_t gen[4];
    int valid;
} rs;

typedef union { uint64_t u64; double d; } U64double;

#define U64x(hi, lo) (((uint64_t)0x##hi << 32) + (uint64_t)0x##lo)

#define TW223_GEN(i, k, q, s) \
    z = rs.gen[i]; \
    z = (((z << q) ^ z) >> (k - s)) ^ ((z & ((uint64_t)(int64_t) - 1 << (64 - k))) << s); \
    r ^= z; rs.gen[i] = z;

/*static int FastFloor(double x)
{
    return x > 0 ? (int)x : (int)x - 1;
}*/

// Returns double in range 1.0 <= d < 2.0
static uint64_t RandomStep()
{
    uint64_t z, r = 0;
    TW223_GEN(0, 63, 31, 18)
    TW223_GEN(1, 58, 19, 28)
    TW223_GEN(2, 55, 24, 7)
    TW223_GEN(3, 47, 21, 8)
    return (r & U64x(000FFFFF, FFFFFFFF)) | U64x(3FF00000, 00000000);
}

void Randomizer::Seed(double d)
{
    rs.valid = 0;

    uint32_t r = 0x11090601;
    for(int i = 0; i < 4; ++i) {
        U64double u;
        uint32_t m = 1u << (r & 255);
        r >>= 8;
        u.d = d = d * 3.14159265358979323846 + 2.7182818284590452354;
        if(u.u64 < m) u.u64 += m;
        rs.gen[i] = u.u64;
    }
    rs.valid = 1;
    for(int i = 0; i < 10; ++i) RandomStep();
}

double Randomizer::Random()
{
    U64double u;
    u.u64 = RandomStep();
    return u.d - 1.0;
}

int Randomizer::Random(int rMin, int rMax)
{
    return static_cast<int>(floor(Random() * (rMax - rMin + 1)) + rMin);
}

double Randomizer::RandomSymmetric()
{
    return 2.0 * Random() - 1.0;
}
