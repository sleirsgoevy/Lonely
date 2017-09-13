#ifndef _COMMON_HPP
#define _COMMON_HPP
#include "biome.hpp"
int recursion(int a, int b, int mul, double (*dsz)(double, double));
biome get(const biome* biome_map, int width, int height, double xd, double yd);
#endif
