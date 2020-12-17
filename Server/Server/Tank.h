#pragma once

class Tank;

#include <d3dx9.h>
#include "Sprite.h"
#include "Object.h"
#include "Tiles.h"
#include "Map.h"
#include <vector>
#include "Bullet.h"

struct snapshot
{
	int timestamp;
	char input;
	float x;
	float y;
	FACING face;
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
	static unsigned int idInit;
	static const string pathToResource;

	FACING curFacing = UP;
	float speed = 1.2f;
	int curSprite = 0;
	int frameDelay = 4;
	int startingFrame = UP * 2;
	int countDownFrameDelay = frameDelay;
	Sprite spriteSheet;
	Tiles spriteSheetInfo;
	MapElement collisionDetect[3] = { BRICK, STONE, WATER };
	Bullet* bullet = NULL;
public:
	Tank();
	Tank(int width, int height, float x, float y, FACING direction, int spriteElemNumber);
	~Tank();

	void UpdateInput();
	void Update(Map* mapInfo, Tank* tanks, int numberOfTanks);
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
	void Shoot();
	int GetID()
	{
		return id;
	}
	float GetSpeed()
	{
		return speed;
	}
	vector<snapshot> history;
	void CalculateSnapshot(char input, int timestamp, int position);
	void SaveSnapShot(char input, int timestamp);
	void UpdateVelocity();

private:
	void UpdateAnimation();
	void TankCollideDetect(Tank* tanks, int numberOfTanks);
};	