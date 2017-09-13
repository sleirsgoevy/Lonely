#include "geom.hpp"
#include "planet.hpp"
#include "config.h"
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

void render3d(char* ptr, vector<planet>& planets)
{
    char* origptr = ptr;
    for(int j = 0; j < 480; j++)
        for(int i = 0; i < 640; i++)
        {
            if((ptr - origptr)%3)
                asm volatile("int $3");
            coord ray = {(i - 320) / 200., (j - 240) / 200., 1};
            for(planet& i : planets)
            {
                coord cross = ray_x_sphere(ray, i.center, i.r);
                if(cross.z > 0)
                {
                    cross.x -= i.center.x;
                    cross.y -= i.center.y;
                    cross.z -= i.center.z;
                    cross.x /= i.r;
                    cross.y /= i.r;
                    cross.z /= i.r;
                    cross = i.m * cross;
                    double latitude = asin(cross.y);
                    double longitude = atan2(cross.x, -cross.z);
                    longitude = (longitude + i.curr_rot) / 2 / M_PI;
                    longitude -= floor(longitude);
                    latitude = 0.5 - latitude / M_PI;
                    int la = min(HEIGHT - 1., latitude * HEIGHT);
                    int lo = min(WIDTH - 1., longitude * WIDTH);
                    latitude = 1 - fabs(2 * latitude - 1);
                    double h = pow((i.hlines[lo][la] - i.min_height) / (double)(i.max_height - i.min_height), 1.2);
                    biome b = i.get(latitude, h);
                    *(ptr++) = min(255., sqrt(b.b));
                    *(ptr++) = min(255., sqrt(b.g));
                    *(ptr++) = min(255., sqrt(b.r));
                    goto next_pixel;
                }
            }
            for(int i = 0; i < 3; i++)
                *(ptr++) = 0;
        next_pixel:
#ifdef USE_SDL2
            ptr++;
#endif
            ;
        }
}
