#pragma once
#include <limits>
#include <d3dx9.h>

struct Box
{
	D3DXVECTOR2 botLeftPosition;
	float width, height;
	D3DXVECTOR2 center;
	D3DXVECTOR2 velocity;
	D3DXVECTOR2 direction;

	D3DXVECTOR2 GetCenterPos()
	{
		return botLeftPosition + center;
	}
};

float SweptAABB(Box box1, Box box2, float &normal_x, float &normal_y);
bool OverlappedBox(Box box1, Box box2);

