#pragma once

#include "dxgraphics.h"
#include "dxinput.h"

#define CAMERA_SPEED 0

class Camera
{
protected:
	int _Speed;
	int _gameWidth;
public:
	int _X, _Y;

	Camera();
	Camera(int gameWidth);
	~Camera();
	D3DXVECTOR3 WorldToView(D3DXVECTOR3 pos);
	void Update(int x);

};

