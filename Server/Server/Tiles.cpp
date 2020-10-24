#include "Tiles.h"



Tiles::Tiles()
{
}


Tiles::~Tiles()
{
}

Tiles::Tiles(string fileName, int numberTiles)
{
	_tilesArray = new TileInfo[numberTiles];
	TiXmlDocument doc(fileName.c_str());

	if (!doc.LoadFile())
	{
		MessageBox(NULL, "Failed to read file XML\nPlease check path file XML", "Error", MB_OK);
		return;
	}

	TiXmlElement* root = doc.RootElement();
	TiXmlElement* tile = nullptr;
	int i = 0;

	for (tile = root->FirstChildElement(); tile != NULL; tile = tile->NextSiblingElement())
	{
		int x, y, w, h;
		const char* nameTileTemp;
		TileInfo TileTemp;

		// get value from file xml
		nameTileTemp = tile->Attribute("n");
		tile->QueryIntAttribute("x", &x);
		tile->QueryIntAttribute("y", &y);
		tile->QueryIntAttribute("w", &w);
		tile->QueryIntAttribute("h", &h);

		TileTemp._tileName = nameTileTemp;
		TileTemp._rectLocation.left = x;
		TileTemp._rectLocation.top = y;
		TileTemp._rectLocation.right = x + w;
		TileTemp._rectLocation.bottom = y + h;

		// add into ListTile
		_tilesArray[i] = TileTemp;
 		i++;
	};
}

RECT Tiles::getRectLocation(int ID)
{
	return _tilesArray[ID]._rectLocation;
}

void Tiles::TilesClear()
{
	if (_tilesArray)
		delete[]_tilesArray;
}
