#pragma once

class Sprite;
typedef class Sprite* LPSprite;

enum SpecialCenter
{
	MiddleCenter = 0,
	BottomCenter = 1,
	TopCenter = 2,
	TopLeft = 3,
	TopRight = 4,
	BottomLeft = 5,
	BottomRight = 6,
	LeftCenter = 7,
	RightCenter = 8
};

#include <d3d9.h>
#include <d3dx9.h>
#include "dxgraphics.h"
#include "dxinput.h"
#include "Collision.h"
#include "Camera.h"
#include "debug.h"


class Sprite
{
protected:
	LPDIRECT3DTEXTURE9 _Image;
	D3DXMATRIX _renderMatrix;

public:
	int _Width, _Height;
	Sprite();
	~Sprite();
	Sprite(string Path);

	/*void Render(Camera camera);
	void Render(Camera camera, RECT renderRect);
	void Render(Camera camera, RECT renderRect, D3DXVECTOR3 center);*/

	void Render(Camera camera, RECT renderRect, D3DXVECTOR3 center, float X, float Y, float FlipX = 1.0f, float FlipY = 1.0f, float Scale = 1.0f);
	void Render(Camera camera, RECT renderRect, SpecialCenter sp, float X, float Y, float FlipX, float FlipY, float Scale);
	void Render(Camera camera, RECT renderRect, Box obj, float Scale, bool isFlip = false);

	virtual void KillSprite();
	int GetWidth();
	int GetHeight();
};

