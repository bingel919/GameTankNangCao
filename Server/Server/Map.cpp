#include "Map.h"



Map::Map()
{
}

Map::Map(string path, int width, int height, int spriteElemNumber)
{
	spriteSheet = Sprite(path + ".png");
	spriteSheetInfo = Tiles(path + ".xml", spriteElemNumber);
	elemWidth = width;
	elemHeight = height;
	ifstream map;
	map.open(path + ".txt");
	for (int i = 27; i > -1; i--)
		for (int j = 0; j < 28; j++)
			map >> block[i][j];
			//block[i][j] = 0;
	block[5][5] = 1;
	map.close();
}


void Map::Render(Camera camera)
{
	D3DXVECTOR3 center = D3DXVECTOR3(elemWidth / 2.0f, elemHeight / 2.0f, 0);
	D3DXVECTOR2 offset = D3DXVECTOR2(elemWidth / 2.0f, elemHeight / 2.0f);
	for (int i = 0; i < 28; i++)
		for (int j = 0; j < 28; j++)
		{
			if (block[i][j] == 0 || block[i][j] == -1)
				continue;
			spriteSheet.Render(camera, spriteSheetInfo.getRectLocation(block[i][j] - 1), center,
				offset.x + elemWidth * (j - 1), offset.y + elemHeight * (i - 1));
		}
}

void Map::CollisionDetect(Object * objInfo, MapElement elemCollisionDetect[], int size)
{
	int startingXPos, startingYPos, endingXPos, endingYPos;
	startingXPos =(int) floor(objInfo->objInfo.botLeftPosition.x / elemWidth) - 1;
	startingYPos =(int) floor(objInfo->objInfo.botLeftPosition.y / elemHeight) - 1;
	D3DXVECTOR2 topRightPos = objInfo->objInfo.botLeftPosition;
	topRightPos += D3DXVECTOR2(objInfo->objInfo.width, objInfo->objInfo.height);
	endingXPos = (int)ceil(topRightPos.x / elemWidth) + 1;
	endingYPos = (int)ceil(topRightPos.y / elemHeight) + 1;
	
	startingXPos = startingXPos < 0 ? 0 : startingXPos;
	startingYPos = startingYPos < 0 ? 0 : startingYPos;

	endingXPos = endingXPos > MAP_X_WIDTH - 1 ? MAP_X_WIDTH - 1 : endingXPos;
	endingYPos = endingYPos > MAP_Y_HEIGHT - 1 ? MAP_Y_HEIGHT - 1 : endingYPos;

	for (int i=startingYPos; i<=endingYPos;i++)
		for (int j = startingXPos; j <= endingXPos; j++)
		{
			for (int k=0; k<size; k++)
				if (block[i][j] == (int)elemCollisionDetect[k] || block[i][j] == -1)
				{
					Box elemBox;
					elemBox.width = elemWidth;
					elemBox.height = elemHeight;
					elemBox.velocity = D3DXVECTOR2(0, 0);
					elemBox.botLeftPosition = D3DXVECTOR2((j - 1) * elemWidth, (i - 1) * elemHeight);
					float normalX, normalY;
					float collisionTime = SweptAABB(objInfo->objInfo, elemBox, normalX, normalY);
					if (collisionTime < 0.0f)
						collisionTime = 0;
					if (collisionTime < objInfo->collisionTime && (normalX != 0 || normalY != 0))
					{
						objInfo->collisionTime = collisionTime;
						objInfo->normalX = normalX;
						objInfo->normalY = normalY;
					}

					break;
				}
		}
}



Map::~Map()
{
}
