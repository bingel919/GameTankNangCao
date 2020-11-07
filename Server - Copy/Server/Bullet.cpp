#include "Bullet.h"

const string Bullet::pathToResource = "Resources/Bullet/Bullet";
unsigned int Bullet::idInit = 0;

void Bullet::UpdateBullet(Map * mapInfo)
{
	mapInfo->CollisionDetect(this, collisionDetect, 2);
	objInfo.botLeftPosition += objInfo.velocity *collisionTime;
}

Bullet::Bullet()
{
	objType = BulletObj;
}

Bullet::Bullet(float x, float y, FACING direction)
{
	id = idInit;
	idInit = (idInit + 1) % BULLET_MAX_RANGE;
	objType = BulletObj;
	float width = 6;
	float height = 6;
	objInfo.width = width;
	objInfo.height = height;
	objInfo.center = D3DXVECTOR2(width / 2.0f, height / 2.0f);
	spriteSheet = Sprite(pathToResource + ".png");
	spriteSheetInfo = Tiles(pathToResource + ".xml", 4);
	objInfo.center = D3DXVECTOR2(width / 2.0f, height / 2.0f);
	curSprite = (int)direction;

	D3DXVECTOR2 offset;
	switch (direction)
	{
	case LEFT:
		offset = D3DXVECTOR2(-width, -height / 2.0f);
		objInfo.direction = D3DXVECTOR2(-1, 0);
		break;
	case RIGHT:
		offset = D3DXVECTOR2(0, -height / 2.0f);
		objInfo.direction = D3DXVECTOR2(1, 0);
		break;
	case UP:
		offset = D3DXVECTOR2(-width/2, 0);
		objInfo.direction = D3DXVECTOR2(0, 1);
		break;
	case DOWN:
		offset = D3DXVECTOR2(-width / 2, -height);
		objInfo.direction = D3DXVECTOR2(0, -1);
		break;
	default:
		break;
	}
	objInfo.botLeftPosition = D3DXVECTOR2(x + offset.x, y + offset.y);
	objInfo.velocity = objInfo.direction * speed;
}

void Bullet::Render(Camera camera)
{
	spriteSheet.Render(camera, spriteSheetInfo.getRectLocation(curSprite), objInfo, 1);
}


Bullet::~Bullet()
{
}
