//dxgraphics framework

#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <d3dx9core.h>


using namespace std;

#define GAME_WIDTH 26 * 8
#define GAME_HEIGHT 26 * 8


#define FULLSCREEN 0
#define SCREEN_WIDTH GAME_WIDTH
#define SCREEN_HEIGHT GAME_HEIGHT

#define BLANK_DISTANCE_WIDTH 0
#define BLANK_DISTANCE_HEIGHT 0

//function prototype
int Init_Direct3D(HWND, int, int, int);
LPDIRECT3DSURFACE9 LoadSurface(string, D3DCOLOR);
LPDIRECT3DTEXTURE9 LoadTexture(string, D3DCOLOR);

int GetTextWidth(string tmp, LPD3DXFONT font);
int GetTextHeight(string tmp, LPD3DXFONT font);

//variable declaration
extern LPDIRECT3D9 d3d;
extern LPDIRECT3DDEVICE9 d3ddev;
extern LPDIRECT3DSURFACE9 backbuffer;
extern LPD3DXSPRITE spritehandler;
extern LPD3DXFONT font;


