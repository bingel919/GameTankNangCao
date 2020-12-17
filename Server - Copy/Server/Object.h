#pragma once
#include <d3dx9math.h>
#include "Collision.h"



enum OBJECT_TYPE
{
	TankObj = 0, BulletObj = 1
};

enum FACING
{
	UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3
};

class Object
{
public:
	unsigned int id = 0;
	Box objInfo;
	float collisionTime = 1.0f;
	float normalX = 0;
	float normalY = 0;
	OBJECT_TYPE objType;
	bool isDestroy = false;
	bool isRespawn = false;
};

struct ObjectItemLinkedList
{
	Object curItem;
	ObjectItemLinkedList* nextItem;
};

//class ListObjectInGame
//{
//private:
//	ListObjectInGame() {}
//public:
//	static ListObjectInGame* instance;
//	ObjectItemLinkedList* headItem;
//
//	
//
//	~ListObjectInGame()
//	{
//		ObjectItemLinkedList* tmpItem;
//		if (headItem != NULL)
//		{
//			tmpItem = headItem;
//			headItem = headItem->nextItem;
//			delete(tmpItem);
//		}
//
//		if (instance != NULL)
//			delete(instance);
//	}
//
//	static ListObjectInGame* GetInstance()
//	{
//		if (!instance)
//			instance = new ListObjectInGame();
//		return instance;
//	}
//
//	void AddItem(Object item)
//	{
//		ObjectItemLinkedList* tmp = new ObjectItemLinkedList();
//		tmp->curItem = item;
//		if (headItem != NULL)
//			tmp->nextItem = headItem;
//		headItem = tmp;
//	}
//
//	void RemoveItem(Object item)
//	{
//		ObjectItemLinkedList* tmpPos = headItem;
//		ObjectItemLinkedList* prevPos = NULL;
//		while (tmpPos != NULL)
//		{
//			if (tmpPos->curItem.id == item.id && tmpPos->curItem.objType == item.objType)
//			{
//				//is first element
//				if (prevPos == NULL)
//				{
//					headItem = tmpPos->nextItem;
//					delete(tmpPos);
//					return;
//				}
//				prevPos->nextItem = tmpPos->nextItem;
//				delete(tmpPos);
//				return;
//			}
//			prevPos = tmpPos;
//			tmpPos = tmpPos->nextItem;
//		}
//	}
//
//};

const unsigned int TANK_MAX_RANGE = 4;
const unsigned int BULLET_MAX_RANGE = 100;




