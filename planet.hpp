#ifndef _PLANET_HPP
#define _PLANET_HPP
#include "geom.hpp"
#include "biome.hpp"
#include <vector>

struct planet
{
    std::vector<std::vector<int> > hlines;
    coord center;
    double r;
    double rot_speed;
    const biome* biome_map;
    int biome_map_width;
    int biome_map_height;
    int min_height;
    int max_height;
    int ocean_level;
    void generate(int seed);
};

#endif
