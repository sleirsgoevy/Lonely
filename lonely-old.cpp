#include <SDL/SDL.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <ctime>
#include <cmath>

using namespace std;

/*int resolution = 480;
vector<vector<int> > map(resolution, vector<int>(resolution));
int unknown_value = 0;
int random_coaf = 1;

int uV(int x, int y)
{
    if (x < 0 || x >= resolution || y < 0 || y >= resolution)
        return unknown_value;
    else
        return map[x][y];
}

void diamond(int x, int y, int radius, int iter)
{
    map[x][y] = (uV(x - radius, y) + uV(x + radius, y) + uV(x, y + radius) + uV(x, y - radius)) / 4 + (rand() % (10 - iter)*random_coaf);
}

void square(int x, int y, int lenght, int iter)
{
    map[x][y] = (uV(x - lenght, y - lenght) + uV(x - lenght, y + lenght) + uV(x + lenght, y - lenght) + uV(x + lenght, y + lenght)) / 4 + (rand() % (10 - iter)*random_coaf);
}*/

int WIDTH = 1024;
int HEIGHT = 1024;

map<pair<int, int>, bool> data_exists;
map<pair<int, int>, int> data;

int recursion(int a, int b, int mul)
{
/*    if(a < 0 || a*mul >= WIDTH || b < 0 || b*mul >= WIDTH)
        return 0;*/
    pair<int, int> key = make_pair((WIDTH - (a * mul) % WIDTH) % WIDTH, (HEIGHT - (b * mul) % HEIGHT) % HEIGHT);
    if(data_exists[key])
        return data[key];
    int ans;
    if(a % 2 == 0 && b % 2 == 0)
        ans = recursion(a / 2, b / 2, mul * 2);
    else if(a % 2 && b % 2)
        ans = (recursion(a - 1, b - 1, mul)+recursion(a - 1, b + 1, mul)+recursion(a + 1, b - 1, mul)+recursion(a + 1, b + 1, mul)) / 4 + rand() % mul - mul / 2;
    else
        ans = (recursion(a - 1, b, mul)+recursion(a + 1, b, mul)+recursion(a, b - 1, mul)+recursion(a, b + 1, mul)) / 4 + rand() % mul - mul / 2;
    ans = abs(ans);
    data_exists[key] = true;
    data[key] = ans;
    return ans;
}

int main(int argc, char* argv[])
{
    srand(time(NULL));
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    SDL_Surface* screen = SDL_SetVideoMode(640, 480, 24,
                                           SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }

/*    // load an image
    SDL_Surface* bmp = SDL_LoadBMP("cb.bmp");
    if (!bmp)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return 1;
    }*/

    // centre the bitmap on screen
/*    SDL_Rect dstrect;
    dstrect.x = (screen->w - bmp->w) / 2;
    dstrect.y = (screen->h - bmp->h) / 2;*/

    // program main loop



    /*int iteration = 7;
    for (int i = 0; i < iteration; ++i)
    {
        int lenght = resolution / (1 << (i + 1));
        unknown_value = rand() % (10 - i);
        for (int x = lenght; x < resolution; x += 2*lenght)
            for (int y = lenght; y < resolution; y += 2*lenght)
                {
                    square(x, y, lenght, i);

                    diamond(x - lenght, y, lenght, i);
                    diamond(x, y + lenght - 1, lenght, i);
                    diamond(x + lenght - 1, y, lenght, i);
                    diamond(x, y - lenght, lenght, i);
                }
    }*/





   /* cout << RAND_MAX << endl;

    for(int i = 0; i < 640; i++)
        for(int j = 0; j < 480; j++)
        {
            data[i][j] = rand() % 256;
        }

    vector<vector<int> > data2(640, vector<int>(480));
    int radius = 5;
    int divisor = 100;
    for(int i = 0; i < 640; i++)
        for(int j = 0; j < 480; j++)
        {
            data2[i][j] = 0;
            for(int x = max(0, i - radius); x < min(640, i + radius + 1); x++)
                for(int y = max(0, j - radius); y < min(480, j + radius + 1); y++)
                    data2[i][j] += data[x][y];
            data2[i][j] = min(data2[i][j] / divisor, 255);
        }
*/


    data_exists[make_pair(0, 0)] = data_exists[make_pair(0, WIDTH - 1)] = data_exists[make_pair(WIDTH - 1, 0)] = data_exists[make_pair(WIDTH - 1, HEIGHT - 1)] = true;
    vector<vector</*bool*/int> > hlines(WIDTH, vector</*bool*/int>(HEIGHT));
/*    for(int k = 50; k < 400; k += 50)
    {
        cout << k << endl;
        for(int i = 0; i < 640; i++)
            for(int j = 0; j < 480; j++)
            {
                bool have_less = false, have_more = false;
                for(int x = max(0, i - 1); x < min(640, i + 2); x++)
                    for(int y = max(0, j - 1); y < min(480, j + 2); y++)
                    {
                        int val = recursion(x, y, 1);
                        if(val <= k)
                            have_less = true;
                        if(val >= k)
                            have_more = true;
                    }
                if(have_less && have_more)
                    hlines[i][j] = true;
            }
    }*/
    for(int i = 0; i < WIDTH; i++)
        for(int j = 0; j < HEIGHT; j++)
            hlines[i][j] = min(255, 25 * (recursion(i, j, 1) / 25));
    double rot_angle = 0;
    bool rot_mode = argc - 1;
    bool done = false;
    while (!done)
    {
        if(rot_angle > 2 * M_PI)
            rot_angle -= 2 * M_PI;
        cout << "PUTIN" << endl;
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                done = true;
                break;
            case SDL_KEYDOWN:
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    else if(event.key.keysym.sym == SDLK_SPACE)
                        rot_mode = !rot_mode;
                    break;
                }
            }
        }
//        SDL_LockSurface(screen);
        char* ptr = (char*)screen->pixels;
        int step = screen->pitch / screen->w;
        for(int j = 0; j < 480; j++)
            for(int i = 0; i < 640; i++)
            { 
                int value;
                int hyp = hypot(i - 320, j - 240);
                if(hyp == 200)
                    value = 255;
                else if(hyp > 200)
                    value = 0;
                else
                {
                    double x = (i - 320) / 200.0;
                    double y = (j - 240) / 200.0;
                    double latitude, longitude;
                    if(rot_mode)
                    {
                        latitude = acos(hypot(x, y));
                        longitude = atan(y / x);
                        if(x < 0)
                            longitude += M_PI;
                    }
                    else
                    {
                        latitude = asin(y);
                        x /= cos(latitude);
                        latitude += M_PI / 2;
                        longitude = asin(x);
                    }
                    longitude += rot_angle;
                    int la = min(HEIGHT - 1., latitude * HEIGHT / M_PI);
                    int lo = longitude * WIDTH / 2 / M_PI;
                    lo = (WIDTH + lo % WIDTH) % WIDTH;
                    value = hlines[lo][la];
                }
                for(int k = 0; k < 3; k++)
                    *(ptr++) = /*255 **/ value;//hlines[i][j];//min(recursion(i, j, 1), 255);//uV(i, j);//*((i - 300)*(i - 300) + (j - 300)*(j - 300) <= 2500);
//        SDL_UnlockSurface(screen);
            }
        SDL_Flip(screen);
        rot_angle += 0.03;
    }
    return 0;
}
