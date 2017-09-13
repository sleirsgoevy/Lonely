#include "geom.hpp"
#include "planet.hpp"
#include <cmath>

void render3d(char* ptr, vector<planet>& planets)
{
    for(int j = 0; j < 480; j++)
        for(int i = 0; i < 640; i++)
        {
            coord ray = {(i - 320) / 200., (j - 240) / 200., 1};
            for(planet i : planets)
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
                    double latitude = asin(cross.y);
                    cross.x /= cos(latitude);
                    double longitude = asin(cross.x);
                    longitude += i.curr_rot;
                    int la = min(HEIGHT - 1., latitude * HEIGHT);
                    int lo = min(WIDTH - 1., longitude * WIDTH);
                    latitude = 1 - fabs(2 * latitude / M_PI);
                    double h = pow((i.hlines[lo][la] - i.min_height) / (double)(i.max_height - i.min_height), 1.2);
                    biome b = i.get(latitude, h);
                    *(ptr++) = b.b;
                    *(ptr++) = b.g;
                    *(ptr++) = b.r;
                    goto next_pixel;
                }
            }
            for(int i = 0; i < 3; i++)
                *(ptr++) = 0;
        next_pixel:
#ifdef USE_SDL2
            ptr++;
#endif
        }
}
