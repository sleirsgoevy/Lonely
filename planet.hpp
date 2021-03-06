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
    double curr_rot = 0;
    double ocean_level;
    matrix m;// = matrix(1, 0, 0, 0, 1, 0, 0, 0, 1);
    void generate(int seed);
    biome get(double xd, double yd);
};

#endif
