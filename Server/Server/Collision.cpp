#include "Collision.h"

float SweptAABB(Box mainBox, Box box2, float &normal_x, float &normal_y)
{
	Box box1 = mainBox;
	box1.botLeftPosition += D3DXVECTOR2(0.1, 0.1);
	box1.width -= 0.1;
	box1.height -= 0.1;

	box1.velocity.x -= box2.velocity.x;
	box1.velocity.y -= box2.velocity.y;
	float xInvEntry, yInvEntry;
	float xInvExit, yInvExit;

	//distance
	if (box1.velocity.x > 0)
	{
		xInvEntry = box2.botLeftPosition.x - (box1.botLeftPosition.x + box1.width);
		xInvExit = (box2.botLeftPosition.x + box2.width) - box1.botLeftPosition.x;
	}
	else
	{
		xInvEntry = (box2.botLeftPosition.x + box2.width) - box1.botLeftPosition.x;
		xInvExit = box2.botLeftPosition.x - (box1.botLeftPosition.x + box1.width);
	}

	if (box1.velocity.y > 0)
	{	
		yInvEntry = box2.botLeftPosition.y - (box1.botLeftPosition.y + box1.height);
		yInvExit = (box2.botLeftPosition.y + box2.height) - box1.botLeftPosition.y;
	}
	else
	{
		yInvEntry = (box2.botLeftPosition.y + box2.height) - box1.botLeftPosition.y;
		yInvExit = box2.botLeftPosition.y - (box1.botLeftPosition.y + box1.width);
	}

	//time of collision and leaving
	float xEntry, yEntry;
	float xExit, yExit;

	if (box1.velocity.x == 0)
	{
		xEntry = -std::numeric_limits<float>::infinity();
		xExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		xEntry = xInvEntry / box1.velocity.x;
		xExit = xInvExit / box1.velocity.x;
	}

	if (box1.velocity.y == 0)
	{
		yEntry = -std::numeric_limits<float>::infinity();
		yExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		yEntry = yInvEntry / box1.velocity.y;
		yExit = yInvExit / box1.velocity.y;
	}

	//earliest and latest times of collision
	float entryTime = fmax(xEntry, yEntry);
	float exitTime = fmin(xExit, yExit);

	//no collision?
	if (entryTime > exitTime || (xEntry<0 && yEntry<0) || xEntry>1.0f || yEntry>1.0f || 
		(box1.velocity.x == 0 && fabs(xInvEntry + xInvExit) > box1.width + box2.width) ||
		(box1.velocity.y == 0 && fabs(yInvEntry + yInvExit) > box1.height + box2.height))
	{
		normal_x = 0;
		normal_y = 0;
		return 1.0f;
	}
	else
	{
		//which surface is collided
		if (xEntry > yEntry)
		{
			if (xInvEntry < 0)
			{
				//right box2
				normal_x = 1.0f;
				normal_y = 0;
			}
			else if (xInvEntry > 0)
			{
				//left box2
				normal_x = -1.0f;
				normal_y = 0;
			}
			else
			{
				if (box1.velocity.x > 0)
				{
					normal_x = -1.0f;
					normal_y = 0;
				}
				else
				{
					normal_x = 1.0f;
					normal_y = 0;
				}
			}
		}
		else
		{
			if (yInvEntry < 0)
			{
				//Top box2
				normal_x = 0;
				normal_y = 1.0f;
			}
			else if (yInvEntry > 0)
			{
				//Bottom box2
				normal_x = 0;
				normal_y = -1.0f;
			}
			else
			{
				if (box1.velocity.y > 0)
				{
					normal_x = 0;
					normal_y = -1.0f;
				}
				else
				{
					normal_x = 0;
					normal_y = 1.0f;
				}
			}
		}
		return entryTime;
	}
}

bool OverlappedBox(Box box1, Box box2)
{
	float xInvEntry, yInvEntry;
	float xInvExit, yInvExit;

	//distance
	if (box1.velocity.x > 0)
	{
		xInvEntry = box2.botLeftPosition.x - (box1.botLeftPosition.x + box1.width);
		xInvExit = (box2.botLeftPosition.x + box2.width) - box1.botLeftPosition.x;
	}
	else
	{
		xInvEntry = (box2.botLeftPosition.x + box2.width) - box1.botLeftPosition.x;
		xInvExit = box2.botLeftPosition.x - (box1.botLeftPosition.x + box1.width);
	}

	if (box1.velocity.y > 0)
	{
		yInvEntry = box2.botLeftPosition.y - box2.height - box1.botLeftPosition.y;
		yInvExit = box2.botLeftPosition.y - (box1.botLeftPosition.y - box1.height);
	}
	else
	{
		yInvEntry = box2.botLeftPosition.y - (box1.botLeftPosition.y - box1.height);
		yInvExit = box2.botLeftPosition.y - box2.height - box1.botLeftPosition.y;
	}
	if ((fabs(xInvEntry + xInvExit) <= box1.width + box2.width) && (fabs(yInvEntry + yInvExit) <= box1.height + box2.height))
		return true;
	return false;
}
