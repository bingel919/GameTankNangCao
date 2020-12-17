#pragma once

class Bullet;

#include "Object.h"
#include "Tiles.h"
#include "Map.h"

class Bullet : public Object
{
protected:
	static const string pathToResource;
	static unsigned int idInit;
	float speed = 3.0f;
	int curSprite = 0;
	int idTank;
public:
	Sprite spriteSheet;
	Tiles spriteSheetInfo;
	MapElement collisionDetect[3] = { BRICK, STONE };

	void UpdateBullet(Map* mapInfo);

	Bullet();
	Bullet(int inIdTank, float x, float y, FACING direction);
	void Render(Camera camera);
	~Bullet();
};

