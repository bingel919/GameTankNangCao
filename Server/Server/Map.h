#pragma once

#include "Tiles.h"
#include "Sprite.h"
#include <fstream>
#include "Object.h"
#include <cmath>

const int MAP_X_WIDTH = 28;
const int MAP_Y_HEIGHT = 28;

enum MapElement
{
	BOUND = -1,
	NONE = 0,
	BRICK = 1,
	STONE = 2,
	GRASS = 3,
	ICE = 4,
	WATER = 5,

};

class Map
{
protected:
	static const string pathToResource;
	Sprite spriteSheet;
	Tiles spriteSheetInfo;
	int block[MAP_Y_HEIGHT][MAP_X_WIDTH];
	int elemWidth, elemHeight;

public:
	Map();
	Map(int width, int height, int spriteElemNumber);



	void Render(Camera camera);
	void CollisionDetect(Object * objInfo, MapElement elemCollisionDetect[], int size);
	
	~Map();

};

