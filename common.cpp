#include "common.hpp"
#include "config.h"
#include <unordered_map>

using namespace std;

biome average(const biome& a, double A, const biome& b, double B)
{
    return {(a.r * A + b.r * B) / (A + B), (a.g * A + b.g * B) / (A + B), (a.b * A + b.b * B) / (A + B)};
}

biome get(const biome* biome_map, int width, int height, double xd, double yd)
{
    xd *= width - 1;
    yd *= height - 1;
    int x = xd;
    int y = yd;
    int x2 = x + 1;
    int y2 = y + 1;
    biome average_left = average(biome_map[y * width + x], y2 - yd, biome_map[min(height - 1, y2) * width + x], yd - y);
    biome average_right = average(biome_map[y * width + min(width - 1, x2)], y2 - yd, biome_map[min(height - 1, y2) * width + min(width - 1, x2)], yd - y);
    return average(average_left, x2 - xd, average_right, xd - x);
}

unordered_map<long long, bool> data_exists;
unordered_map<long long, int> data;

int recursion(int a, int b, int mul, double (*dsz)(double, double))
{
    pair<int, int> key = make_pair((WIDTH - (a * mul) % WIDTH) % WIDTH, (HEIGHT - (b * mul) % HEIGHT) % HEIGHT);
    long long lkey = (((long long)key.first) << 32) + (long long)key.second;
    double multiplier = dsz(key.first / (double)WIDTH, key.second / (double)HEIGHT);
    if(data_exists[lkey])
        return data[lkey];
    int ans;
    if(a % 2 == 0 && b % 2 == 0)
        ans = recursion(a / 2, b / 2, mul * 2, dsz);
    else if(a % 2 && b % 2)
        ans = (recursion(a - 1, b - 1, mul, dsz)+recursion(a - 1, b + 1, mul, dsz)+recursion(a + 1, b - 1, mul, dsz)+recursion(a + 1, b + 1, mul, dsz)) / 4 + multiplier * (rand() % mul - mul / 2);
    else
        ans = (recursion(a - 1, b, mul, dsz)+recursion(a + 1, b, mul, dsz)+recursion(a, b - 1, mul, dsz)+recursion(a, b + 1, mul, dsz)) / 4 + multiplier * (rand() % mul - mul / 2);
    data_exists[lkey] = true;
    data[lkey] = ans;
    return ans;
}


