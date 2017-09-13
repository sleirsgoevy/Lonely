lonely: main.cpp dsq.cpp geom.cpp planet.cpp view3d.cpp biome.hpp dsq.hpp config.h geom.hpp planet.hpp
	c++ --std=c++11 main.cpp dsq.cpp geom.cpp planet.cpp view3d.cpp -lSDL -o lonely

lonely-debug: main.cpp dsq.cpp geom.cpp planet.cpp view3d.cpp biome.hpp dsq.hpp config.h geom.hpp planet.hpp
	c++ --std=c++11 main.cpp dsq.cpp geom.cpp planet.cpp view3d.cpp -lSDL -o lonely-debug -g

lonely-sdl2: main.cpp dsq.cpp geom.cpp planet.cpp view3d.cpp biome.hpp dsq.hpp config.h geom.hpp planet.hpp
	c++ -DUSE_SDL2 --std=c++11 main.cpp dsq.cpp geom.cpp planet.cpp view3d.cpp -lSDL2 -o lonely-sdl2

lonely-sdl2-debug: main.cpp dsq.cpp geom.cpp planet.cpp view3d.cpp biome.hpp dsq.hpp config.h geom.hpp planet.hpp
	c++ -DUSE_SDL2 --std=c++11 main.cpp dsq.cpp geom.cpp planet.cpp view3d.cpp -lSDL2 -o lonely-sdl2-debug -g
