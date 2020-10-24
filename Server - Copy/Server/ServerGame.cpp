#include "ServerGame.h"

//id's to assign clients for our table
unsigned int ServerGame::client_id = 0;

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
SOCKADDR_IN server_address;
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
	server_address.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	__int32 player_x;
	__int32 player_y;
	SetSocketBlockingEnabled(sock, false);
}


ServerGame::~ServerGame()
{
}
int ServerGame::Game_Init()
{
	virtualClock.Start();
	tank = Tank("Resources/Tank/Tank1/Tank1", 13, 13, 0, 0, UP, 8);
	camera = Camera(GAME_WIDTH);
	map = Map("Resources/Map/Map", 8, 8, 7);
	//socket
	ClientSetup();
	tank.sock = sock;
	tank.server_address = server_address;
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
	tank.UpdateVelocity();

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
	WSACleanup();
}
