#include <SFML/Graphics.hpp>
#include "Game.h"
#include <iostream>

int main(int argc, char * argv[]) 
{
	Game g("config.txt");
	g.run();
	return 0;
}

