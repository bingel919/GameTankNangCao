#pragma once
#include <d3dx9math.h>

struct Object
{
	Box objInfo;
	float collisionTime = 1;
	float normalX = 0;
	float normalY = 0;
};