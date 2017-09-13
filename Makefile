lonely: main.cpp common.cpp
	c++ --std=c++11 main.cpp common.cpp -lSDL -o lonely

lonely-sdl2: main.cpp common.cpp
	c++ -DUSE_SDL2 --std=c++11 main.cpp common.cpp -lSDL2 -o lonely-sdl2
