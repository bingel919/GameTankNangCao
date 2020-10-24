#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "dxgraphics.h"
#include "dxinput.h"
#include "Collision.h"
#include "tinyxml.h"

using namespace std;

class Tiles;
typedef class Tiles* LPTiles;

struct TileInfo
{
	string _tileName;
	RECT _rectLocation;
};



class Tiles
{
protected:
	TileInfo* _tilesArray;
public:
	Tiles();
	virtual ~Tiles();
	Tiles(string fileName, int numberTiles);

	RECT getRectLocation(int ID);
	void TilesClear();

};

