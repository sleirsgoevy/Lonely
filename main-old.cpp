#ifdef USE_SDL2
#include <SDL2/SDL.h>
#else
#include <SDL/SDL.h>
#endif
#include "biome.hpp"
#include "dsq.hpp"
#include "planet.hpp"
#include "config.h"
#include "view3d.hpp"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <cmath>

using namespace std;

const biome arctic = {40000, 40000, 65536};
const biome tundra = {100, 0, 0};
const biome taiga = {0, 2500, 0};
const biome wasteland = {900, 900, 900};
const biome forest = {0, 10000, 0};
const biome desert = {40000, 40000, 0};
const biome savanna = {65536, 40000, 0};
const biome veld = {65536, 30000, 0};
const biome eq = {0, 10000, 1000};
const biome ocean = {0, 0, 65536};

const biome biome_map_earth[64] = {
    arctic, arctic, arctic, arctic, arctic, arctic, arctic, arctic,
    arctic, arctic, arctic, arctic, arctic, tundra, tundra, tundra,
    arctic, arctic, arctic, tundra, tundra, wasteland, taiga, taiga,
    arctic, arctic, tundra, taiga, wasteland, wasteland, forest, forest,
    arctic, tundra, taiga, taiga, wasteland, wasteland, veld, veld,
    arctic, tundra, taiga, forest, forest, veld, desert, desert,
    arctic, tundra, taiga, forest, veld, desert, savanna, savanna,
    arctic, tundra, taiga, forest, veld, desert, savanna, eq
};

double pow12(double x)
{
    return pow(x, 1.2);
}

int main(int argc, char* argv[])
{
    int seed;
    if(argc == 2)
        seed = atoi(argv[1]);
    else
    {
        seed = time(NULL);
        cout << "Seed: " << seed << endl;
    }
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Unable to init SDL: %s\n", SDL_GetError());
        return 1;
    }
    atexit(SDL_Quit);
#ifdef USE_SDL2
    SDL_Window* window = SDL_CreateWindow("Lonely", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    if(!window)
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, 640, 480);
    char* buffer = (char*)malloc(640*480*4);
    if(!buffer)
    {
        printf("Unable to allocate frame buffer\n");
        return 1;
    }
#else
    SDL_Surface* screen = SDL_SetVideoMode(640, 480, 24,
                                           SDL_HWSURFACE|SDL_DOUBLEBUF);
    if(!screen)
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }
#endif
    planet earth;
    earth.ocean_level = OCEAN_LEVEL;
    earth.biome_map = biome_map_earth;
    earth.biome_map_width = earth.biome_map_height = 8;
    earth.generate(seed);
    earth.center = {0, 0, 1.5};
    earth.r = 1;
    earth.rot_speed = 0.03;
    vector<planet> planets;
    planets.push_back(earth);
#define earth planets[0]
    while(true)
    {
#ifdef USE_SDL2
        char* ptr = buffer;
#else
        char* ptr = (char*)screen->pixels;
#endif
        render3d(ptr, planets);
#ifdef USE_SDL2
        SDL_UpdateTexture(texture, NULL, buffer, 640 * 4);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
#else
        SDL_Flip(screen);
#endif
        planets[0].curr_rot += planets[0].rot_speed;
        cout << "YUHU!" << endl;
    }
    return 0;
    double rot_angle = 0;
    double vert_rot = 0;
    double horz_rot_mode = 0;
    bool rot_mode = false;
    int vert_rot_mode = 0;
    bool color_mode = false;
    bool done = false;
    bool do_rotate = true;
    while(!done)
    {
        if(rot_angle > 2 * M_PI)
            rot_angle -= 2 * M_PI;
        cout << "I will draw a frame now!" << endl;
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                done = true;
                break;
            case SDL_KEYDOWN:
                {
                    if(event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    else if(event.key.keysym.sym == SDLK_SPACE)
                        rot_mode = !rot_mode;
                    else if(event.key.keysym.sym == SDLK_e)
                        color_mode = !color_mode;
                    else if(event.key.keysym.sym == SDLK_DOWN)
                        vert_rot_mode = 1;
                    else if(event.key.keysym.sym == SDLK_UP)
                        vert_rot_mode = -1;
                    else if(event.key.keysym.sym == SDLK_RIGHT)
                        horz_rot_mode = 1;
                    else if(event.key.keysym.sym == SDLK_LEFT)
                        horz_rot_mode = -1;
                    else if(event.key.keysym.sym == SDLK_r)
                        do_rotate = !do_rotate;
                    break;
                }
            case SDL_KEYUP:
                {
                    if(event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN)
                        vert_rot_mode = 0;
                    else if(event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT)
                        horz_rot_mode = 0;
                    break;
                }
            }
        }
#ifdef USE_SDL2
        char* ptr = buffer;
#else
        char* ptr = (char*)screen->pixels;
#endif
        for(int j = 0; j < 480; j++)
            for(int i = 0; i < 640; i++)
            { 
                int value;
                int hyp = hypot(i - 320, j - 240);
                if(hyp >= 200)
                {
                    int data = max(0, 255 - 20 * (hyp - 200));
                    *(ptr++) = data;
                    *(ptr++) = data * 2 / 3;
                    *(ptr++) = data * 2 / 3;
                }
                else
                {
                    double x = (i - 320) / 200.0;
                    double y = (j - 240) / 200.0;
                    double latitude, longitude;
                    if(rot_mode)
                    {
                        latitude = asin(hypot(x, y));
                        longitude = -atan(y / x);
                        if(x < 0)
                            longitude -= M_PI;
                    }
                    else
                    {
                        y /= sin(acos(x));
                        double a = asin(y);
                        a += vert_rot;
                        bool do_swap = false;
                        if(a > M_PI / 2)
                        {
                            a = M_PI - a;
                            do_swap = true;
                        }
                        if(a < -M_PI / 2)
                        {
                            a = -M_PI - a;
                            do_swap = true;
                        }
                        y = sin(a) * sin(acos(x));
                        latitude = asin(y);
                        x /= cos(latitude);
                        latitude += M_PI / 2;
                        longitude = asin(x);
                        if(do_swap)
                            longitude = M_PI - longitude;
                    }
                    longitude += rot_angle;
                    latitude /= M_PI;
                    longitude /= 2 * M_PI;
                    longitude -= floor(longitude);
                    int la = min(HEIGHT - 1., latitude * HEIGHT);
                    int lo = min(WIDTH - 1., longitude * WIDTH);
                    double h = pow12((earth.hlines[lo][la] - earth.min_height) / (double)(earth.max_height - earth.min_height));
                    biome b;
                    if(h >= earth.ocean_level)
                        b = get(earth.biome_map, earth.biome_map_width, earth.biome_map_height, 1 - abs(2 * latitude - 1), 1 - h);
                    else
                        b = ocean;
                    if(color_mode)
                    {
                        *(ptr++) = min(255., h * 256);
                        *(ptr++) = min(255., h * 256);
                        *(ptr++) = min(255., h * 256);
                    }
                    else
                    {
                        *(ptr++) = min(255, (int)sqrt(b.b));
                        *(ptr++) = min(255, (int)sqrt(b.g));
                        *(ptr++) = min(255, (int)sqrt(b.r));
                    }
                }
#ifdef USE_SDL2
                ptr++;
#endif
            }
#ifdef USE_SDL2
        SDL_UpdateTexture(texture, NULL, buffer, 640 * 4);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
#else
        SDL_Flip(screen);
#endif
        if(do_rotate)
            rot_angle += 0.03;
        if(vert_rot_mode > 0)
            vert_rot = min(M_PI / 2, vert_rot + 0.1);
        else if(vert_rot_mode < 0)
            vert_rot = max(-M_PI / 2, vert_rot - 0.1);
        if(horz_rot_mode > 0)
            rot_angle += 0.05;
        else if(horz_rot_mode < 0)
            rot_angle -= 0.05;
    }
    return 0;
}
