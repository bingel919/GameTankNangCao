#include "Tank.h"
#include <chrono>
#include <ctime> 


unsigned int Tank::idInit = 0;
const string Tank::pathToResource = "Resources/Tank";

Tank::Tank()
{
	objType = TankObj;
}

Tank::Tank(int width, int height, float x, float y, FACING direction, int spriteElemNumber)
{
	id = idInit;
	string tankName = "//Tank" + to_string(id + 1);
	idInit = (idInit + 1) % TANK_MAX_RANGE;
	string path = pathToResource + tankName + tankName;
	spriteSheet = Sprite(path + ".png");
	spriteSheetInfo = Tiles(path + ".xml", spriteElemNumber);

	respawnPos = D3DXVECTOR2(x, y);

	objInfo.botLeftPosition = respawnPos;
	objInfo.direction = D3DXVECTOR2(1, 1);
	objInfo.center = D3DXVECTOR2(width / 2.0f, height / 2.0f);
	objInfo.width = width;
	objInfo.height = height;
	objType = TankObj;
	objInfo.velocity = D3DXVECTOR2(0, 0);

	startingFrame = direction * 2;
	curFacing = direction;
}


Tank::~Tank()
{
}

Bullet* Tank::UpdateInput()
{

	FACING prevFace = curFacing;
	if (Key_Down(DIK_UP) && id == 0 ||
		Key_Down(DIK_W) && id == 1)
	{
		//objInfo.botLeftPosition.y += speed * collisionTime;
		objInfo.velocity.y = speed;
		objInfo.direction.y = 1;
		objInfo.direction.x = 0;
		curFacing = UP;
	}
	else if (Key_Down(DIK_DOWN) && id == 0 ||
		Key_Down(DIK_S) && id == 1)
	{
		//objInfo.botLeftPosition.y -= speed * collisionTime;
		objInfo.velocity.y = -speed;
		objInfo.direction.y = -1;
		objInfo.direction.x = 0;
		curFacing = DOWN;
	}
	else if (Key_Down(DIK_LEFT) && id == 0 ||
		Key_Down(DIK_A) && id == 1)
	{
		//objInfo.botLeftPosition.x -= speed * collisionTime;
		objInfo.velocity.x = -speed;
		objInfo.direction.x = -1;
		objInfo.direction.y = 0;
		curFacing = LEFT;
	}
	else if (Key_Down(DIK_RIGHT) && id == 0 ||
		Key_Down(DIK_D) && id == 1)
	{
		//objInfo.botLeftPosition.x += speed * collisionTime;
		objInfo.velocity.x = speed;
		objInfo.direction.x = 1;
		objInfo.direction.y = 0;
		curFacing = RIGHT;
	}

	bool isCreateBullet = false;
	if (bullet == NULL)
	{
		if (Key_Down(DIK_SPACE) && id == 0 ||
			Key_Down(DIK_LCONTROL) && id == 1 || bShoot)
		{
			bShoot = true;
			Shoot(isCreateBullet);
		}
		
	}

	if (isShoot && bullet == nullptr)
	{
		Shoot(false);
		isShoot = false;
		isCreateBullet = true;
	}

	if (prevFace != curFacing)
		countDownFrameDelay = 0;


	if (objInfo.velocity != D3DXVECTOR2(0, 0))
		UpdateAnimation();
	//bShoot = false;
	if (isCreateBullet)
		return bullet;
	return NULL;

}

void Tank::Update(Map* mapInfo, Tank* tanks, int numberOfTanks)
{
	switch (previousKey)
	{
	case 0:
		Stop();
		break;
	case 1:
		GoUp();
		break;
	case 2:
		GoDown();
		break;
	case 3:
		GoLeft();
		break;
	case 4:
		GoRight();
		break;

	default:
		break;
	}
	mapInfo->CollisionDetect(this, collisionDetect, 3);
	TankCollideDetect(tanks, numberOfTanks);
	

	if (bullet != NULL)
	{
		if (!bullet->isDestroy)
			bullet->UpdateBullet(mapInfo);
		//if (bullet->isDestroy)
		//{
		//	//delete bullet;
		//	bullet = nullptr;
		//}
	}
}

void Tank::Render(Camera camera)
{
	if (!isRespawn)
	{
		RECT rect;
		rect.bottom = spriteSheet.GetHeight();
		rect.right = spriteSheet.GetWidth();
		rect.left = rect.top = 0;
		spriteSheet.Render(camera, spriteSheetInfo.getRectLocation(curSprite), objInfo, 1);
	}
	/*if (bullet)
		bullet->Render(camera);*/

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

void Tank::SetMovingKey(int x)
{
	previousKey = x;
}



void Tank::GoUp()
{
	//previousKey = 1;
	//objInfo.botLeftPosition.y += speed * collisionTime;
	objInfo.velocity.y = speed;
	objInfo.direction.y = 1;
	curFacing = UP;
	objInfo.velocity.x = 0;

	if (objInfo.velocity != D3DXVECTOR2(0, 0))
		UpdateAnimation();
}
void Tank::GoDown()
{
	//previousKey = 2;
	objInfo.velocity.y = -speed;
	objInfo.direction.y = -1;
	curFacing = DOWN;
	objInfo.velocity.x = 0;

	if (objInfo.velocity != D3DXVECTOR2(0, 0))
		UpdateAnimation();
}
void Tank::GoLeft()
{
	//previousKey = 3;
	//objInfo.botLeftPosition.x -= speed * collisionTime;
	objInfo.velocity.x = -speed;
	objInfo.direction.x = -1;
	curFacing = LEFT;
	objInfo.velocity.y = 0;
	if (objInfo.velocity != D3DXVECTOR2(0, 0))
		UpdateAnimation();
}
void Tank::GoRight()
{
	//previousKey = 4;
	objInfo.velocity.x = speed;
	objInfo.direction.x = 1;
	curFacing = RIGHT;
	objInfo.velocity.y = 0;

	if (objInfo.velocity != D3DXVECTOR2(0, 0))
		UpdateAnimation();
}

void Tank::Stop()
{
	objInfo.velocity = D3DXVECTOR2(0, 0);
}

Bullet* Tank::Shoot(bool isCreateBullet)
{
	if (bullet != nullptr)
		return nullptr;
	isCreateBullet = true;
	D3DXVECTOR2 firingPos = objInfo.GetCenterPos();
	//firingPos += objInfo.direction * (objInfo.width / 2 - 7);
	bullet = new Bullet(id, firingPos.x, firingPos.y, curFacing);
	bShoot = true;
	return bullet;
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
			bullet = new Bullet(id, lagX, lagY+delay*3.0f, history[position].face);
			break;
		case DOWN:
			bullet = new Bullet(id, lagX, lagY - delay*3.0f, history[position].face);
			break;
		case LEFT:
			bullet = new Bullet(id, lagX - 3.0f*delay, lagY, history[position].face);
			break;
		case RIGHT:
			bullet = new Bullet(id, lagX + 3.0f*delay, lagY, history[position].face);
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
		}
		history.pop_back();
		history.push_back(snap);
	}
}

void Tank::UpdateAnimation()
{
	startingFrame = curFacing * 2;
	countDownFrameDelay--;
	if (countDownFrameDelay <= 0)
	{
		countDownFrameDelay = frameDelay;
		curSprite = curSprite % 2 + startingFrame;
	}
}

void Tank::TankCollideDetect(Tank * tanks, int numberOfTanks)
{
	for (int i = 0; i < numberOfTanks; i++)
	{
		/*if (tanks[i].id == id)
			continue;*/
		float normalX, normalY;
		float collisionTime = SweptAABB(this->objInfo, tanks[i].objInfo, normalX, normalY);
		if (collisionTime < 0.0f)
			collisionTime = 0;
		if (collisionTime < this->collisionTime && (normalX != 0 || normalY != 0))
		{
			this->collisionTime = collisionTime;
			this->normalX = normalX;
			this->normalY = normalY;
		}

	}
}

void Tank::TankCollideBullet(Bullet * bullet)
{
	if (isRespawn || bullet->isDestroy)
		return;
	float normalX, normalY;
	float collisionTime = SweptAABB(this->objInfo, bullet->objInfo, normalX, normalY);
	if (collisionTime < 0.0f)
		collisionTime = 0;
	if (collisionTime < this->collisionTime && (normalX != 0 || normalY != 0))
	{
		this->isRespawn = true;
		bullet->isDestroy = true;
	}
}

void Tank::UpdateVelocity()
{
	if (abs(normalX) > 0.0001f)
		objInfo.velocity.x = 0;
	if (abs(normalY) > 0.0001f)
		objInfo.velocity.y = 0;
	/*if (id == 1)
	_RPT1(0, "%d ; %d \n", objInfo.velocity.x, objInfo.velocity.y);*/
	objInfo.botLeftPosition += objInfo.velocity *collisionTime;

	collisionTime = 1;
	normalX = normalY = 0;
	UpdateAnimation();
}
void Tank::BulletReset()
{
	if (bullet && bullet->isDestroy)
		bullet = NULL;
}

void Tank::Respawn()
{
	if (!isRespawn)
		return;
	objInfo.botLeftPosition = respawnPos;
	isRespawn = false;
}
