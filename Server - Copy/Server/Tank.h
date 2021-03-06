#pragma once

class Tank;

#include <d3dx9.h>
#include "Sprite.h"
#include "Object.h"
#include "Tiles.h"
#include "Map.h"
#include "Bullet.h"

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

	SOCKET sock;
	SOCKADDR_IN server_address;
	void UpdateInput(int player);
	void Update(Map* mapInfo, Tank* tanks, int numberOfTanks);
	void Render(Camera camera);
	package *GetPackage();
	void UsePackage(package *pak);
	int GetX()
	{
		return objInfo.botLeftPosition.x;
	}
	int SendPack(char command);
	int ReceivPack();
	void UpdateVelocity();
	void UsePack(int player_x, int player_y, int player, bool shoot);
private:
	void UpdateAnimation();
	void TankCollideDetect(Tank* tanks, int numberOfTanks);
	int previousX ;
	int previousY;
};	