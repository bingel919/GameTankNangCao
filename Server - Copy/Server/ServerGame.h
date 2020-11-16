#pragma once

#include <math.h>
#include <windows.h>
#include "VClock.h"
#include <d3dx9math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "dxgraphics.h"
#include "dxinput.h"
#include "dxsound.h"
#include "Collision.h"
#include <string>
#include "Tank.h"
#include "Map.h"

const unsigned int	TICKS_PER_SECOND = 60.0f;
const float	SECONDS_PER_TICK = 1.0f / float(TICKS_PER_SECOND);
const int MAX_PLAYER = 1;


class ServerGame
{
protected:
	static ServerGame *instance;
	static unsigned int client_id;

	LARGE_INTEGER clock_frequency;
	bool sleep_granularity_was_set;

	Camera camera;
	Tank tank;
	Map map;
public:
	VClock virtualClock;

	ServerGame();
	~ServerGame();
	Map* GetMap()
	{
		return &map;
	}
	int Game_Init();
	void Game_Run();
	void Game_End();
	static void DestroyBlock(int i, int j)
	{
		
		instance->GetMap()->DestroyBlock(i, j);
	}
	static ServerGame* GetInstace()
	{
		if (!instance)
		{
			instance = new ServerGame();
		}
		return instance;
	}

	void Update();

	

};

