lonely: main.cpp dsq.cpp geom.cpp planet.cpp biome.hpp dsq.hpp config.h geom.hpp planet.hpp
	c++ --std=c++11 main.cpp dsq.cpp geom.cpp planet.cpp -lSDL -o lonely

lonely-debug:
	c++ --std=c++11 main.cpp dsq.cpp geom.cpp planet.cpp -lSDL -o lonely-debug -g

lonely-sdl2: main.cpp dsq.cpp geom.cpp planet.cpp biome.hpp dsq.hpp config.h geom.hpp planet.hpp
	c++ -DUSE_SDL2 --std=c++11 main.cpp dsq.cpp geom.cpp planet.cpp -lSDL2 -o lonely-sdl2

lonely-sdl2-debug: main.cpp dsq.cpp geom.cpp planet.cpp biome.hpp dsq.hpp config.h geom.hpp planet.hpp
	c++ -DUSE_SDL2 --std=c++11 main.cpp dsq.cpp geom.cpp planet.cpp -lSDL2 -o lonely-sdl2-debug -g
