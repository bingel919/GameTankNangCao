#include "ServerGame.h"
#include <iostream>
#include <fcntl.h>
#include <chrono>
#include <ctime> 
#include <string>
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <fstream>
#ifndef _USE_OLD_IOSTREAMS
using namespace std;
#endif
// maximum mumber of lines the output console should have
static const WORD MAX_CONSOLE_LINES = 500;
#ifdef _DEBUG

void RedirectIOToConsole()
{
	int hConHandle;
	long lStdHandle;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE *fp;

	// allocate a console for this app
	AllocConsole();

	// set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = MAX_CONSOLE_LINES;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

	// redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);

	// redirect unbuffered STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);

	// redirect unbuffered STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
	// point to console as well
	ios::sync_with_stdio();
}

#endif
//End of File
//id's to assign clients for our table
unsigned int ServerGame::client_id = 0;
int previousTime;
bool client1 = false;
bool client2 = false;
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
SOCKADDR_IN nullsock;
SOCKADDR_IN from;
SOCKADDR_IN from1;
SOCKADDR_IN from2;
int lagX = 0;
int lagY = 0;
int previousX[2];
int previousY[2];
void ProcessInput(Tank &tank, _int8 buffer[])
{
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
	char debug[50] = "Receive package";
	OutputDebugString(debug);
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
		lagY = tank.GetSpeed()*delay;
		break;

	case 'a':
		tank.GoLeft();
		lagX = -tank.GetSpeed()*delay;
		//SendBack(tank, from);
		break;

	case 's':
		tank.GoDown();
		lagY = -tank.GetSpeed()*delay;
		//SendBack(tank, from);
		break;

	case 'd':
		tank.GoRight();
		lagX = tank.GetSpeed()*delay;
		//SendBack(tank, from);
		break;
	case 'q':
		tank.Shoot();
		//SendBack(tank, from);
		break;
	default:
		printf("unhandled input %c\n", client_input);
		break;
	}
}
int ServerRun(Tank &tank1, Tank &tank2)
{
	const int SOCKET_BUFFER_SIZE = 8000;
	__int8 buffer[SOCKET_BUFFER_SIZE];
	// get input packet from player
	int flags = 0;
	int from_size = sizeof(from);
	int bytes_received = recvfrom(sock, buffer, SOCKET_BUFFER_SIZE, flags, (SOCKADDR*)&from, &from_size);


	if (bytes_received == SOCKET_ERROR)
	{
		printf("recvfrom returned SOCKET_ERROR, WSAGetLastError() %d", WSAGetLastError());
		auto err = WSAGetLastError();
		//return 0;
	}
	else
	{
		if (!client1)
		{
			from1 = from;
			client1 = true;
		}
		else
		{
			if (!client2 && from1.sin_addr.s_addr != from.sin_addr.s_addr)
			{
				from2 = from;
				client2 = true;
			}
		}
		// process input
		if (from1.sin_addr.s_addr == from.sin_addr.s_addr)
		{
			ProcessInput(tank1, buffer);
		}
		if (from2.sin_addr.s_addr == from.sin_addr.s_addr)
		{
			ProcessInput(tank2, buffer);
		}
		
	}
	return 0;
}
bool SentFrom1 = false;
int SendBack(Tank tank[], SOCKADDR_IN from1, SOCKADDR_IN from2)
{
	if (from2.sin_addr.s_addr == from.sin_addr.s_addr)
	{
		int a = 0;
	}
	if (tank[0].GetX() == previousX[0] && tank[0].GetY() == previousY[0] && tank[1].GetX() == previousX[1] && tank[1].GetY() == previousY[1])
		return 0;
	if (tank[1].GetX() != previousX[1] && tank[1].GetY() != previousY[1])
	{
		int debug = 0;
	}
	const int SOCKET_BUFFER_SIZE = 8000;
	int flags = 0;
	SOCKADDR* to1 = (SOCKADDR*)&from1;
	SOCKADDR* to2 = (SOCKADDR*)&from2;
	int to_length = sizeof(from);
	__int8 buffer2[SOCKET_BUFFER_SIZE];
	//if (tank.GetX() == previousX[tank.GetID()])
		//lagX = 0;
	//if (tank.GetY() == previousY[tank.GetID()])
		//lagY = 0;
	__int32 player_x[2];
	__int32 player_y[2];

	player_x[0] = tank[0].GetX();
	player_y[0] = tank[0].GetY();
	previousX[0] = player_x[0];
	previousY[0] = player_y[0];

	player_x[1] = tank[1].GetX();
	player_y[1] = tank[1].GetY();
	previousX[1] = player_x[1];
	previousY[1] = player_y[1];

	//player_x[1] = player_x[1] + lagX*3;
	//player_y[1] = player_y[1] + lagY*3;

	// create state packet
	__int32 write_index = 0;

	bool POS = true;

	memcpy(&buffer2[write_index], &POS, sizeof(POS));
	write_index += sizeof(POS);

	memcpy(&buffer2[write_index], &player_x, sizeof(player_x));
	write_index += sizeof(player_x);

	memcpy(&buffer2[write_index], &player_y, sizeof(player_y));
	write_index += sizeof(player_y);
	

	//send back to client
	int buffer_length = sizeof(player_x) + sizeof(player_y) + sizeof(POS);
	if (SentFrom1)
	{
		if (sendto(sock, buffer2, buffer_length, flags, to1, to_length) == SOCKET_ERROR)
		{
			printf("sendto failed: %d", WSAGetLastError());
			auto err = WSAGetLastError();
			if (from2.sin_addr.s_addr == from.sin_addr.s_addr)
			{
				int a = 0;
			}
		}
		else
		{
			if (from2.sin_addr.s_addr == from.sin_addr.s_addr)
			{
				int a = 0;
			}
			char debug[50] = "Send back package";
			OutputDebugString(debug);
			auto err = "a";
		}
	}
	else
	{
		if (sendto(sock, buffer2, buffer_length, flags, to2, to_length) == SOCKET_ERROR)
		{
			printf("sendto failed: %d", WSAGetLastError());
			auto err = WSAGetLastError();
			if (from2.sin_addr.s_addr == from.sin_addr.s_addr)
			{
				int a = 0;
			}
		}
		else
		{
			if (from2.sin_addr.s_addr == from.sin_addr.s_addr)
			{
				int a = 0;
			}
			char debug[50] = "Send back package";
			OutputDebugString(debug);
			auto err = "a";
		}
	}
	SentFrom1 = !SentFrom1;
	return 1;
}
void ServerGame::SendBrickStatus(int i, int j)
{

	int flags = 0;
	SOCKADDR* to1 = (SOCKADDR*)&from1;
	SOCKADDR* to2 = (SOCKADDR*)&from2;
	auto to_length = sizeof(from);

	const int SOCKET_BUFFER_SIZE = 8000;
	__int8 buffer2[SOCKET_BUFFER_SIZE];
	// create state packet
	__int32 write_index = 0;

	bool POS = false;
	memcpy(&buffer2[write_index], &POS, sizeof(POS));
	write_index += sizeof(POS);

	memcpy(&buffer2[write_index], &i, sizeof(i));
	write_index += sizeof(i);

	memcpy(&buffer2[write_index], &j, sizeof(j));
	write_index += sizeof(j);

	

	// send back to client
	int buffer_length = sizeof(i) + sizeof(j) + sizeof(POS);
	if (sendto(sock, buffer2, buffer_length, flags, to1, to_length) == SOCKET_ERROR)
	{
		printf("sendto failed: %d", WSAGetLastError());
		auto err = WSAGetLastError();
		err = err;
	}
	else
		auto err = "a";
	if (sendto(sock, buffer2, buffer_length, flags, to2, to_length) == SOCKET_ERROR)
	{
		printf("sendto failed: %d", WSAGetLastError());
		auto err = WSAGetLastError();
		err = err;
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
	//ListObjectInGame* objList = ListObjectInGame::GetInstance();
	numberOfTanks = 2;
	tank[0] = Tank(12, 12, 0, 0, UP, 8);
	tank[1] = Tank(12, 12, GAME_WIDTH-14, GAME_HEIGHT-14, UP, 8);
	camera = Camera(GAME_WIDTH);
	map = Map(8, 8, 7);
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
	tank[0].Respawn();
	tank[1].Respawn();

	Bullet* tmp = tank[0].UpdateInput();
	if (tmp != NULL)
		bullets.push_back(tmp);
	tmp = tank[1].UpdateInput();
	if (tmp != NULL)
		bullets.push_back(tmp);

	//socket
	ServerRun(tank[1], tank[0]);
	ServerRun(tank[1], tank[0]);
	ServerRun(tank[1], tank[0]);
	SendBack(tank, from1, from2);

	Update();
	tank[0].Update(&map, tank, numberOfTanks);
	tank[1].Update(&map, tank, numberOfTanks);

	for (int i = 0; i < bullets.size(); i++)
	{
		tank[0].TankCollideBullet(bullets[i]);
		tank[1].TankCollideBullet(bullets[i]);
	}

	tank[0].BulletReset();
	tank[1].BulletReset();

	for (int i = 0; i < bullets.size(); i++)
		if (bullets[i]->isDestroy)
		{
			delete bullets[i];
			bullets[i] = nullptr;
			bullets.erase(bullets.begin() + i);
		}

	tank[0].UpdateVelocity();
	tank[1].UpdateVelocity();

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

		tank[0].Render(camera);
		tank[1].Render(camera);
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
