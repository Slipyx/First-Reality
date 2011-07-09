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
#include <emmintrin.h>

static uint64_t gen[4] = {0xFFFFFFFF,0xCCCCCCCC,0xFF0000FF,0xCC0000CC};

typedef union { uint64_t u64; double d; } U64double;

#define U64x(hi, lo) (((uint64_t)0x##hi << 32) + (uint64_t)0x##lo)

#define TW223_GEN(i, k, q, s) \
    z = gen[i]; \
    z = (((z << q) ^ z) >> (k - s)) ^ ((z & ((uint64_t)(int64_t) - 1 << (64 - k))) << s); \
    r ^= z; gen[i] = z;

static inline int32_t FastFloor(const double& d)
{
    int32_t di = _mm_cvttsd_si32(_mm_load_sd(&d));
    return d > 0.0 || d == di ? di : di - 1;
}

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
    uint32_t r = 0x11090601;
    for(uint8_t i = 0; i < 4; ++i) {
        U64double u;
        uint32_t m = 1u << (r & 255);
        r >>= 8;
        u.d = d = d * 3.14159265358979323846 + 2.7182818284590452354;
        if(u.u64 < m) u.u64 += m;
        gen[i] = u.u64;
    }
    for(uint8_t i = 0; i < 10; ++i) RandomStep();
}

double Randomizer::Random()
{
    U64double u;
    u.u64 = RandomStep();
    return u.d - 1.0;
}

int32_t Randomizer::Random(int32_t rMin, int32_t rMax)
{
    return FastFloor(Random() * (rMax - rMin + 1)) + rMin;
}
