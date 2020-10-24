#include "ServerGame.h"
#include <iostream>
#include <fcntl.h>
#include <chrono>
#include <ctime> 
#include <string>

//id's to assign clients for our table
unsigned int ServerGame::client_id = 0;
int previousTime;

bool SetSocketBlockingEnabled(int fd, bool blocking)
{
	if (fd < 0) return false;

#ifdef _WIN32
	unsigned long mode = blocking ? 0 : 1;
	return (ioctlsocket(fd, FIONBIO, &mode) == 0) ? true : false;
#else
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) return false;
	flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
	return (fcntl(fd, F_SETFL, flags) == 0) ? true : false;
#endif
}
void NaiveSend(int inSocket, const package* inTank)
{
	send(inSocket,
		reinterpret_cast<const char*>(inTank),
		sizeof(Tank), 0);
}
void NaiveRecv(int inSocket, package* outTank)
{
	recv(inSocket,
		reinterpret_cast<char*>(outTank),
		sizeof(Tank), 0);
}
SOCKET sock;
ServerGame::ServerGame()
{
	UINT sleep_granularity_ms = 1;
	sleep_granularity_was_set = timeBeginPeriod(sleep_granularity_ms) == TIMERR_NOERROR;

	QueryPerformanceFrequency(&clock_frequency);
}
int ServerSetUp()
{
	WORD winsock_version = 0x202;
	WSADATA winsock_data;
	if (WSAStartup(winsock_version, &winsock_data))
	{
		printf("WSAStartup failed: %d", WSAGetLastError());
		return 1;
	}
	int address_family = AF_INET;
	int type = SOCK_DGRAM;

	int protocol = IPPROTO_UDP;
	sock = socket(address_family, type, protocol);

	if (sock == INVALID_SOCKET)
	{
		printf("socket failed: %d", WSAGetLastError());
		return 1;
	}

	SOCKADDR_IN local_address;
	local_address.sin_family = AF_INET;
	local_address.sin_port = htons(9999);
	local_address.sin_addr.s_addr = INADDR_ANY;
	if (bind(sock, (SOCKADDR*)&local_address, sizeof(local_address)) == SOCKET_ERROR)
	{
		printf("bind failed: %d", WSAGetLastError());
		return 1;
	}
	SetSocketBlockingEnabled(sock, false);

	auto end = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	auto timenow = static_cast<int>(end_time);

	previousTime = timenow;
	return 0;
}
int ServerRun(Tank &tank)
{
	const int SOCKET_BUFFER_SIZE = 32;
	__int8 buffer[SOCKET_BUFFER_SIZE];
	// get input packet from player
	int flags = 0;
	SOCKADDR_IN from;
	int from_size = sizeof(from);
	int bytes_received = recvfrom(sock, buffer, SOCKET_BUFFER_SIZE, flags, (SOCKADDR*)&from, &from_size);

	int lagX = 0;
	int lagY = 0;

	if (bytes_received == SOCKET_ERROR)
	{
		printf("recvfrom returned SOCKET_ERROR, WSAGetLastError() %d", WSAGetLastError());
		auto err = WSAGetLastError();
		//return 0;
	}
	else
	{

		// process input
		char client_input = buffer[0];
		int delay = 0;
		__int32 timestamp;
		/*for (int i = 0; i < 4; i++)
		{
			timestamp[i] = buffer[i + 1];
		}
		int timeint = atoi(timestamp);*/
		memcpy(&timestamp, &buffer[1], sizeof(timestamp));
		//read_index += sizeof(player_x);
		auto timeint = timestamp;

		auto end = std::chrono::system_clock::now();
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		auto timenow = static_cast<int>(end_time);
		//auto shorttime = timenow % 10000;
		timeint = timenow - timenow % 10000 + timeint;
		delay = timenow - timeint;
		printf("%d.%d.%d.%d:%d - %c\n", from.sin_addr.S_un.S_un_b.s_b1, from.sin_addr.S_un.S_un_b.s_b2, from.sin_addr.S_un.S_un_b.s_b3, from.sin_addr.S_un.S_un_b.s_b4, from.sin_port, client_input);

		if (!tank.history.empty())
		{
			for (int i = 0; i++; i < tank.history.size())
			{
				if (tank.history[i].timestamp <= timeint && timeint <= tank.history[i + 1].timestamp)
				{
					tank.CalculateSnapshot(client_input, tank.history[i].timestamp, i);
				}
			}
		}
		tank.SaveSnapShot(client_input, timenow);


		switch (client_input)
		{
		case 'w':
			tank.GoUp();
			lagY += tank.GetVelocity().y * delay*2;
			break;

		case 'a':
			tank.GoLeft();
			lagX -= tank.GetVelocity().x * delay*2;
			break;

		case 's':
			tank.GoDown();
			lagY -= tank.GetVelocity().y * delay*2;
			break;

		case 'd':
			tank.GoRight();
			lagX += tank.GetVelocity().x * delay*2;
			break;

		default:
			printf("unhandled input %c\n", client_input);
			break;
		}

	
	}
	
	auto end = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	auto timenow = static_cast<int>(end_time);
	
	auto waittime = timenow - previousTime;
		previousTime = timenow;
		flags = 0;
		SOCKADDR* to = (SOCKADDR*)&from;
		int to_length = sizeof(from);
		__int8 buffer2[SOCKET_BUFFER_SIZE];

		__int32 player_x = tank.GetX() + lagX;
		__int32 player_y = tank.GetY() + lagY;

		// create state packet
		__int32 write_index = 0;
		memcpy(&buffer2[write_index], &player_x, sizeof(player_x));
		write_index += sizeof(player_x);

		memcpy(&buffer2[write_index], &player_y, sizeof(player_y));
		write_index += sizeof(player_y);

		//memcpy(&buffer[write_index], &is_running, sizeof(is_running));

		// grab data from packet
		/*__int32 player_x2;
		__int32 player_y2;
		__int32 read_index = 0;

		memcpy(&player_x2, &buffer2[read_index], sizeof(player_x2));
		read_index += sizeof(player_x2);

		memcpy(&player_y2, &buffer2[read_index], sizeof(player_y2));
		read_index += sizeof(player_y2);
		//objInfo.botLeftPosition.x = player_x;
		//objInfo.botLeftPosition.y = player_y;*/

		// send back to client
		int buffer_length = sizeof(player_x) + sizeof(player_y);
		if (sendto(sock, buffer2, buffer_length, flags, to, to_length) == SOCKET_ERROR)
		{
			printf("sendto failed: %d", WSAGetLastError());
			auto err = WSAGetLastError();
			return 1;
		}
		else
			auto err = "a";
}


ServerGame::~ServerGame()
{
}
SOCKET server, client;
SOCKADDR_IN serverAddr, clientAddr;
int ServerGame::Game_Init()
{
	virtualClock.Start();
	tank = Tank("Resources/Tank/Tank1/Tank1", 13, 13, 0, 0, UP, 8);
	camera = Camera(GAME_WIDTH);
	map = Map("Resources/Map/Map", 8, 8, 7);
	//socket
	ServerSetUp();
	//socket
	return 1;
}

void ServerGame::Game_Run()
{
	//reacquire input
	dikeyboard->Acquire();
	dimouse->Acquire();
	Poll_Keyboard();
	Poll_Mouse();

	//Get start time of the beginning period
	//For FPS
	virtualClock.setStartTickTime();

	//socket
	/*int clientAddrSize = sizeof(clientAddr);
	package* pak = tank.GetPackage();
	if ((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)
	{
		NaiveSend(client, pak);
	}*/

	//Update
	tank.UpdateVelocity();

	//socket
	ServerRun(tank);

	Update();
	tank.Update(map);

	//Render
	//start render
	if (d3ddev == NULL)
		return;

	if (d3ddev->BeginScene())
	{
		d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

		//spritehandler render
		spritehandler->Begin(D3DXSPRITE_ALPHABLEND);

		//begin

		tank.Render(camera);
		map.Render(camera);

		//end

		spritehandler->End();
		d3ddev->EndScene();
	}

	d3ddev->Present(NULL, NULL, NULL, NULL);

	//Sleep for FPS
	float time_taken_s = virtualClock.getTimeSince_miliSec() / 1000.0f;
	while (time_taken_s < SECONDS_PER_TICK)
	{
		if (virtualClock.sleep_granularity_was_set)
		{
			DWORD time_to_wait_ms = DWORD((SECONDS_PER_TICK - time_taken_s) * 1000);
			if (time_to_wait_ms > 0)
			{
				Sleep(time_to_wait_ms);
			}
		}
		time_taken_s = virtualClock.getTimeSince_miliSec() / 1000.0f;
	}
}

void ServerGame::Update()
{

}

void ServerGame::Game_End()
{
	if (d3d != NULL) d3d->Release();
	if (d3ddev != NULL) d3ddev->Release();
	if (backbuffer != NULL) backbuffer->Release();
	if (spritehandler != NULL) spritehandler->Release();
	if (dinput != NULL) dinput->Release();
	if (font != NULL) font->Release();
	Kill_Keyboard();
	Kill_Mouse();
	closesocket(server);
	WSACleanup();
}
