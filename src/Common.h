#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Vec2.h"

template <class T> using ptr = std::shared_ptr<T>;

static int RandomRange(int min, int max)
{
	return min + (rand() % (1 + max - min));
}