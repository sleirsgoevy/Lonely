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
    bool done = false;
    bool do_rotate = true;
    int vert_rot_mode = 0;
    int horz_rot_mode = 0;
    while(!done)
    {
        if(earth.curr_rot > 2 * M_PI)
            earth.curr_rot -= 2 * M_PI;
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
        render3d(ptr, planets);
#ifdef USE_SDL2
        SDL_UpdateTexture(texture, NULL, buffer, 640 * 4);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
#else
        SDL_Flip(screen);
#endif
        if(do_rotate)
            earth.curr_rot += earth.rot_speed;
        matrix n;
        if(vert_rot_mode > 0)
            n = n * matrix(
                1, 0, 0,
                0, cos(0.1), -sin(0.1),
                0, sin(0.1), cos(0.1)
            );
        else if(vert_rot_mode < 0)
            n = n * matrix(
                1, 0, 0,
                0, cos(0.1), sin(0.1),
                0, -sin(0.1), cos(0.1)
            );
        if(horz_rot_mode > 0)
            n = n * matrix(
                cos(0.1), -sin(0.1), 0,
                sin(0.1), cos(0.1), 0,
                0, 0, 1
            );
        else if(horz_rot_mode < 0)
            n = n * matrix(
                cos(0.1), sin(0.1), 0,
                -sin(0.1), cos(0.1), 0,
                0, 0, 1
            );
        earth.m = earth.m * n;
    }
    return 0;
}
