#include "Tank.h"
#include <chrono>
#include <ctime> 


unsigned int Tank::idInit = 0;
const string Tank::pathToResource = "Resources/Tank/Tank1/Tank1";

Tank::Tank()
{
	objType = TankObj;
}

Tank::Tank(int width, int height, float x, float y, FACING direction, int spriteElemNumber)
{
	id = idInit;
	idInit = (idInit + 1) % TANK_MAX_RANGE;
	spriteSheet = Sprite(pathToResource + ".png");
	spriteSheetInfo = Tiles(pathToResource + ".xml", spriteElemNumber);

	objInfo.botLeftPosition = D3DXVECTOR2(x, y);
	objInfo.direction = D3DXVECTOR2(1, 1);
	objInfo.center = D3DXVECTOR2(width / 2.0f, height / 2.0f);
	objInfo.width = width;
	objInfo.height = height;
	objType = TankObj;

	startingFrame = direction * 2;
	curFacing = direction;
}


Tank::~Tank()
{
}


void Tank::UpdateVelocity()
{
	objInfo.velocity = D3DXVECTOR2(0, 0);
	FACING prevFace = curFacing;
	if (Key_Down(DIK_UP))
	{
		//objInfo.botLeftPosition.y += speed * collisionTime;
		objInfo.velocity.y = speed;
		objInfo.direction.y = 1;
		objInfo.direction.x = 0;
		curFacing = UP;
	}
	else if (Key_Down(DIK_DOWN))
	{
		//objInfo.botLeftPosition.y -= speed * collisionTime;
		objInfo.velocity.y = -speed;
		objInfo.direction.y = -1;
		objInfo.direction.x = 0;
		curFacing = DOWN;
	}
	else if (Key_Down(DIK_LEFT))
	{
		//objInfo.botLeftPosition.x -= speed * collisionTime;
		objInfo.velocity.x = -speed;
		objInfo.direction.x = -1;
		objInfo.direction.y = 0;
		curFacing = LEFT;
	}
	else if (Key_Down(DIK_RIGHT))
	{
		//objInfo.botLeftPosition.x += speed * collisionTime;
		objInfo.velocity.x = speed;
		objInfo.direction.x = 1;
		objInfo.direction.y = 0;
		curFacing = RIGHT;
	}

	if (Key_Down(DIK_SPACE) && bullet == NULL)
	{
		D3DXVECTOR2 firingPos = objInfo.GetCenterPos();
		//firingPos += objInfo.direction * (objInfo.width / 2 - 7);
		bullet = new Bullet(firingPos.x, firingPos.y, curFacing);
	}
	if (prevFace != curFacing)
		countDownFrameDelay = 0;


	if (objInfo.velocity != D3DXVECTOR2(0, 0))
		UpdateAnimation();

}

void Tank::Update(Map* mapInfo)
{
	mapInfo->CollisionDetect(this, collisionDetect, 3);

	if (abs(normalX) > 0.0001f)
		objInfo.velocity.x = 0;
	if (abs(normalY) > 0.0001f)
		objInfo.velocity.y = 0;
	objInfo.botLeftPosition += objInfo.velocity *collisionTime;
	if (objInfo.velocity.x != 0)
	{
		auto err = "a";
	}

	if (bullet != NULL)
	{
		bullet->UpdateBullet(mapInfo);
		if (bullet->isDestroy)
		{
			delete bullet;
			bullet = NULL;
		}
	}

	collisionTime = 1;
	normalX = normalY = 0;
}

void Tank::Render(Camera camera)
{
	RECT rect;
	rect.bottom = spriteSheet.GetHeight();
	rect.right = spriteSheet.GetWidth();
	rect.left = rect.top = 0;
	spriteSheet.Render(camera, spriteSheetInfo.getRectLocation(curSprite), objInfo, 1);

	if (bullet != NULL)
		bullet->Render(camera);

}

package* Tank::GetPackage()
{
	package* out = new package(objInfo.botLeftPosition.x, objInfo.botLeftPosition.y);
	return out;
}

void Tank::UsePackage(package * pak)
{
	objInfo.botLeftPosition.x = pak->x;
	objInfo.botLeftPosition.y = pak->y;
}

__int32 Tank::GetX()
{
	return objInfo.botLeftPosition.x;
}

__int32 Tank::GetY()
{
	return objInfo.botLeftPosition.y;
}

D3DXVECTOR2 Tank::GetVelocity()
{
	return objInfo.velocity;
}

void Tank::GoUp()
{
	//objInfo.botLeftPosition.y += speed * collisionTime;
	objInfo.velocity.y = speed;
	objInfo.direction.y = 1;
	curFacing = UP;

	if (objInfo.velocity != D3DXVECTOR2(0, 0))
		UpdateAnimation();
}
void Tank::GoDown()
{
	objInfo.velocity.y = -speed;
	objInfo.direction.y = -1;
	curFacing = DOWN;

	if (objInfo.velocity != D3DXVECTOR2(0, 0))
		UpdateAnimation();
}
void Tank::GoLeft()
{
	//objInfo.botLeftPosition.x -= speed * collisionTime;
	objInfo.velocity.x = -speed;
	objInfo.direction.x = -1;
	curFacing = LEFT;

	if (objInfo.velocity != D3DXVECTOR2(0, 0))
		UpdateAnimation();
}
void Tank::GoRight()
{
	objInfo.velocity.x = speed;
	objInfo.direction.x = 1;
	curFacing = RIGHT;

	if (objInfo.velocity != D3DXVECTOR2(0, 0))
		UpdateAnimation();
}

void Tank::Shoot()
{
	D3DXVECTOR2 firingPos = objInfo.GetCenterPos();
	//firingPos += objInfo.direction * (objInfo.width / 2 - 7);
	bullet = new Bullet(firingPos.x, firingPos.y, curFacing);
}

void Tank::CalculateSnapshot(char input, int timestamp, int position)
{
	auto end = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	auto timenow = static_cast<int>(end_time);
	auto delay = timenow- timestamp;
	float lagY = history[position].y;
	float lagX = history[position].x;
	switch (input)
	{
	case 'w':
		lagY += GetVelocity().y * delay;
		break;

	case 'a':
		lagX -= GetVelocity().x * delay;
		break;

	case 's':
		lagY -= GetVelocity().y * delay;
		break;
	case 'd':
		lagX += GetVelocity().x * delay;
		break;
	case 'q':
		switch (history[position].face)
		{
		case UP:
			bullet = new Bullet(lagX, lagY+delay*3.0f, history[position].face);
			break;
		case DOWN:
			bullet = new Bullet(lagX, lagY - delay*3.0f, history[position].face);
			break;
		case LEFT:
			bullet = new Bullet(lagX - 3.0f*delay, lagY, history[position].face);
			break;
		case RIGHT:
			bullet = new Bullet(lagX + 3.0f*delay, lagY, history[position].face);
			break;
		}
		break;

	}
	objInfo.botLeftPosition.x = lagX;
	objInfo.botLeftPosition.y = lagY;
}

void Tank::SaveSnapShot(char input, int timestamp)
{
	snapshot snap;
	snap.input = input;
	snap.x = objInfo.botLeftPosition.x;
	snap.y = objInfo.botLeftPosition.y;
	snap.timestamp = timestamp;
	snap.face = curFacing;
	if (history.size() < 1000)
	{
		history.push_back(snap);
	}
	else
	{
		for (int i = 0; i < history.size()-1; i++)
		{
			history[i] = history[i + 1];
			history.pop_back();
			history.push_back(snap);
		}
	}
}

void Tank::UpdateAnimation()
{
	this;
	startingFrame = curFacing * 2;
	countDownFrameDelay--;
	if (countDownFrameDelay <= 0)
	{
		countDownFrameDelay = frameDelay;
		curSprite = curSprite % 2 + startingFrame;
	}
}
