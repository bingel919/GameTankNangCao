#pragma once

#include <d3dx9.h>
#include "Sprite.h"
#include "Object.h"
#include "Tiles.h"
#include "Map.h"
#include <vector>

struct snapshot
{
	int timestamp;
	char input;
	float x;
	float y;
};

enum FACING
{
	UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3
};
class package
{
public:
	package(int m, int n)
	{
		x = m;
		y = n;
	};
	int x;
	int y;
};

class Tank : public Object
{
protected:
	FACING curFacing = UP;
	float speed = 1.2f;
	int curSprite = 0;
	int frameDelay = 4;
	int startingFrame = UP * 2;
	int countDownFrameDelay = frameDelay;
	Sprite spriteSheet;
	Tiles spriteSheetInfo;
	MapElement collisionDetect[3] = { BRICK, STONE, WATER };
public:
	Tank();
	Tank(string path, int width, int height, float x, float y, FACING direction, int spriteElemNumber);
	~Tank();

	void UpdateVelocity();
	void Update(Map mapInfo);
	void Render(Camera camera);
	package *GetPackage();
	void UsePackage(package *pak);
	__int32 GetX();
	__int32 GetY();
	D3DXVECTOR2 GetVelocity();
	void GoUp();
	void GoDown();
	void GoLeft();
	void GoRight();
	vector<snapshot> history;
	void CalculateSnapshot(char input, int timestamp, int position);
	void SaveSnapShot(char input, int timestamp);


private:
	void UpdateAnimation();
};	