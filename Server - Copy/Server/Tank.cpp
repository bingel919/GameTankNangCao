#include "Tank.h"
#include <iostream>
#include <chrono>
#include <ctime> 
#include <string>
#include "ServerGame.h"


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

bool rev = false;
int previousTime = 0;
void Tank::UpdateVelocity()
{
	bool sentPack = false;
	objInfo.velocity = D3DXVECTOR2(0, 0);
	FACING prevFace = curFacing;
	if (Key_Down(DIK_UP))
	{
		//objInfo.botLeftPosition.y += speed * collisionTime;
		//if (!rev)
		objInfo.velocity.y = speed;
		objInfo.direction.y = 1;
		objInfo.direction.x = 0;
		curFacing = UP;
		SendPack('w');
		sentPack = true;
	}
	else if (Key_Down(DIK_DOWN))
	{
	//	objInfo.botLeftPosition.y -= speed * collisionTime;
		//if (!rev)
		objInfo.velocity.y = -speed;
		objInfo.direction.y = -1;
		objInfo.direction.x = 0;
		curFacing = DOWN;
		SendPack('s');
		sentPack = true;
	}
	else if (Key_Down(DIK_LEFT))
	{
		//objInfo.botLeftPosition.x -= speed * collisionTime;
		//if (!rev)
		objInfo.velocity.x = -speed;
		objInfo.direction.x = -1;
		objInfo.direction.y = 0;
		curFacing = LEFT;
		SendPack('a');
		sentPack = true;
	}
	else if (Key_Down(DIK_RIGHT))
	{
	//	objInfo.botLeftPosition.x += speed * collisionTime;
		//if (!rev)
		objInfo.velocity.x = speed;
		objInfo.direction.x = 1;
		objInfo.direction.y = 0;
		curFacing = RIGHT;
		SendPack('d');
		sentPack = true;
	}

	if (Key_Down(DIK_SPACE) && bullet == NULL)
	{
		D3DXVECTOR2 firingPos = objInfo.GetCenterPos();
		//firingPos += objInfo.direction * (objInfo.width / 2 - 7);
		bullet = new Bullet(firingPos.x, firingPos.y, curFacing);
		SendPack('q');
		sentPack = true;
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
	//if (!rev)
	objInfo.botLeftPosition += objInfo.velocity *collisionTime;

	collisionTime = 1;
	normalX = normalY = 0;

	if (bullet != NULL)
	{
		bullet->UpdateBullet(mapInfo);
		if (bullet->isDestroy)
		{
			delete bullet;
			bullet = NULL;
		}
	}
	ReceivPack();
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

int Tank::SendPack(char command)
{
	const int SOCKET_BUFFER_SIZE = 8000;
	__int8 buffer[SOCKET_BUFFER_SIZE];
	__int32 player_x;
	__int32 player_y;
	__int32 buffertimestamp;

	//scanf_s("\n%c", &buffer[0], 1);
	auto end = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	auto timestamp = static_cast<int>(end_time);
	auto shorttime = timestamp % 10000;
	//auto stringtime = std::to_string(shorttime);
	//auto timestampchar = static_cast<char*>(shorttime);
	buffer[0] = command;
	buffertimestamp = shorttime;
	/*for (int i = 0; i < 4; i++)
	{
		buffer[i + 1] = stringtime[i];
	}*/

	memcpy(&buffer[1], &buffertimestamp, sizeof(buffertimestamp));
	//write_index += sizeof(player_x);

	// send to server
	int buffer_length = sizeof(command) + sizeof(buffertimestamp);
	int flags = 0;
	SOCKADDR* to = (SOCKADDR*)&server_address;
	int to_length = sizeof(server_address);
	if (sendto(sock, buffer, buffer_length, flags, to, to_length) == SOCKET_ERROR)
	{
		printf("sendto failed: %d", WSAGetLastError());
		auto err = WSAGetLastError();
		return 1;
	}
}

int Tank::ReceivPack()
{
	const int SOCKET_BUFFER_SIZE = 8000;
	__int8 buffer[SOCKET_BUFFER_SIZE];

	int flags = 0;
	SOCKADDR_IN from;
	int from_size = sizeof(from);
	int bytes_received = recvfrom(sock, buffer, SOCKET_BUFFER_SIZE, flags, (SOCKADDR*)&from, &from_size);

	if (bytes_received == SOCKET_ERROR)
	{
		printf("recvfrom returned SOCKET_ERROR, WSAGetLastError() %d", WSAGetLastError());
		auto err = WSAGetLastError();
		auto err2 = err;
		return 0;
	}
	else
	{
		auto end = std::chrono::system_clock::now();
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		auto timestamp = static_cast<int>(end_time);

		// grab data from packet
		__int32 read_index = 0;

		__int32 player_x;
		__int32 player_y;

		memcpy(&player_x, &buffer[read_index], sizeof(player_x));
		read_index += sizeof(player_x);

		memcpy(&player_y, &buffer[read_index], sizeof(player_y));
		read_index += sizeof(player_y);
		bool POS;
		memcpy(&POS, &buffer[read_index], sizeof(POS));
		if (POS == true)
		{
			//auto end = std::chrono::system_clock::now();
			//std::time_t end_time = std::chrono::system_clock::to_time_t(end);
			//auto timenow = static_cast<int>(end_time);
			//
			
			if (abs(objInfo.botLeftPosition.x - player_x) >= 20 && abs(objInfo.botLeftPosition.y - player_y) >= 20)
			{
				objInfo.botLeftPosition.x = player_x;
				objInfo.botLeftPosition.y = player_y;
			}
			//
			
		}
		else
		{
			ServerGame::DestroyBlock(player_x, player_y);
		}
		return 1;
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
