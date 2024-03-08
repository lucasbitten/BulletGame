#pragma once
#include "Vec2.h"

class CBoundingBox
{
public:
	Vec2 size;
	Vec2 halfSize;

	CBoundingBox(const Vec2 & s)
		: size(s), halfSize(s.x * 0.5f, s.y * 0.5f)
	{
	}

}; 

