#include "planet.hpp"
#include "dsq.hpp"
#include "config.h"
#include <cstdlib>
#include <cmath>

using namespace std;

double diamond_size(double a, double b)
{
    b = fabs(2 * b - 1);
    //return 1;
    return hypot(1, cos(b * M_PI / 2));
}

void planet::generate(int seed)
{
    srand(seed);
    diamond_square dsq;
    dsq.data_exists[0] = dsq.data_exists[WIDTH - 1] = dsq.data_exists[(WIDTH - 1LL) << 32] = dsq.data_exists[((WIDTH - 1LL) << 32) + HEIGHT - 1] = true;
    hlines.clear();
    hlines.resize(WIDTH, vector<int>(HEIGHT));
    max_height = 0;
    min_height = 0;
    for(int i = 0; i < WIDTH; i++)
        for(int j = 0; j < HEIGHT; j++)
        {
            hlines[i][j] = min(255, dsq.recursion(i, j, 1, diamond_size));
            max_height = max(max_height, hlines[i][j]);
            min_height = min(min_height, hlines[i][j]);
        }
}
