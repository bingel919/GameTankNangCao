#include "ServerGame.h"
#include <chrono>
#include <iostream>
#include <ctime> 
#include <fstream>
using namespace std;
//id's to assign clients for our table
unsigned int ServerGame::client_id = 0;
ServerGame* ServerGame::instance = NULL;
SOCKET sock;
SOCKADDR_IN server_address;
void ReadPlayerAndIP(string &IP, int &player)
{
	ifstream myfile("configs.txt");
	string line;
	if (myfile.is_open())
	{
		getline(myfile, line);
		IP = line;
		getline(myfile, line);
		player = std::stoi(line);
		myfile.close();
	}
	myfile.close();
}
ServerGame::ServerGame()
{
	UINT sleep_granularity_ms = 1;
	sleep_granularity_was_set = timeBeginPeriod(sleep_granularity_ms) == TIMERR_NOERROR;

	QueryPerformanceFrequency(&clock_frequency);
}
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
int player = 1;
int ClientSetup()
{
	const int PORT = 9999;
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

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	string StrIP;
	ReadPlayerAndIP(StrIP, player);
	const char* IP = StrIP.c_str();
	server_address.sin_addr.S_un.S_addr = inet_addr(IP);

	__int32 player_x;
	__int32 player_y;
	SetSocketBlockingEnabled(sock, false);
}
void ReceivePack(Tank &tank1, Tank &tank2, int player)
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
	}
	else
	{
		auto end = std::chrono::system_clock::now();
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		auto timestamp = static_cast<int>(end_time);

		// grab data from packet
		__int32 read_index = 0;

		bool POS;
		memcpy(&POS, &buffer[read_index], sizeof(POS));
		read_index += sizeof(POS);
		if (POS == true)
		{
			__int32 player_x[2];
			__int32 player_y[2];

			bool shoot[2];

			memcpy(&player_x, &buffer[read_index], sizeof(player_x));
			read_index += sizeof(player_x);

			memcpy(&player_y, &buffer[read_index], sizeof(player_y));
			read_index += sizeof(player_y);

			memcpy(&shoot, &buffer[read_index], sizeof(shoot));
			read_index += sizeof(shoot);
			//auto end = std::chrono::system_clock::now();
			//std::time_t end_time = std::chrono::system_clock::to_time_t(end);
			//auto timenow = static_cast<int>(end_time);
			//

			//if (abs(objInfo.botLeftPosition.x - player_x) >= 20 && abs(objInfo.botLeftPosition.y - player_y) >= 10)
			//{
			//if (id == 0)
			//{
			//	tank1.UsePack(player_x, player_y);
			//}
			//else
			_RPT1(0, "%d ; %d \n", player_x[1], tank2.GetX());

				tank2.UsePack(player_x[1], player_y[1], player, shoot[1]);
				tank1.UsePack(player_x[0], player_y[0], player, shoot[0]);
			//}


		}
		else
		{
			__int32 player_x;
			__int32 player_y;

			memcpy(&player_x, &buffer[read_index], sizeof(player_x));
			read_index += sizeof(player_x);

			memcpy(&player_y, &buffer[read_index], sizeof(player_y));
			read_index += sizeof(player_y);
			ServerGame::DestroyBlock(player_x, player_y);
		}
	}
}

ServerGame::~ServerGame()
{
}
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
	ClientSetup();
	tank[0].sock = sock;
	tank[0].server_address = server_address;
	tank[0].SendPack('0');
	tank[1].sock = sock;
	tank[1].server_address = server_address;
	tank[1].SendPack('0');
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

	//socket;

	//socket

	//Update
	tank[0].UpdateInput(player);
	tank[1].UpdateInput(player);

	Update();
	ReceivePack(tank[0], tank[1], player);
	tank[0].Update(&map, tank, numberOfTanks);
	tank[1].Update(&map, tank, numberOfTanks);

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
	WSACleanup();
}
