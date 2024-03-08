#pragma once
#include "Vec2.h"
#include <SFML/Graphics.hpp>

class CTransform
{
public:
	Vec2 pos = { 0.0, 0.0 };
	Vec2 velocity = { 0.0, 0.0 };
	float angle = 0;

	CTransform(const Vec2& position, const Vec2& velocity, float angle)
		: pos(position),velocity(velocity), angle(angle)
	{
	}

};