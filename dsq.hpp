#ifndef _COMMON_HPP
#define _COMMON_HPP
#include "biome.hpp"
#include <unordered_map>

struct diamond_square
{
    std::unordered_map<long long, bool> data_exists;
    std::unordered_map<long long, int> data;
    int recursion(int a, int b, int mul, double (*dsz)(double, double));
};

biome get(const biome* biome_map, int width, int height, double xd, double yd);
#endif
