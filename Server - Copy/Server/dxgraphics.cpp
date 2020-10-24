//dxgraphics framework source


#include "dxgraphics.h"

using namespace std;

//variable declaration
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backbuffer = NULL;
LPD3DXSPRITE spritehandler = NULL;
LPD3DXFONT font = NULL;

int Init_Direct3D(HWND hWnd, int width, int height, int fullscreen)
{
	//initialize Direct3D
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL)
	{
		MessageBox(hWnd, "Error Initializing Direct3D", "Error", MB_OK);
		return 0;
	}

	//set Direct3D presentation parameters
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = (!fullscreen);
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.hDeviceWindow = hWnd;

	//Create Direct3DDevice
	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);

	if (d3ddev == NULL)
	{
		MessageBox(hWnd, "Error Creating Direct3D Device", "Error", MB_OK);
		return 0;
	}

	HRESULT result;
	result = D3DXCreateSprite(d3ddev, &spritehandler);
	if (result != D3D_OK)
	{
		MessageBox(hWnd, "Error Creating Sprite Handler", "Error", MB_OK);
		return 0;
	}

	//clear backbuffer to a color
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	//Create pointer to the back buffer
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

	//font init
	result = D3DXCreateFont(d3ddev, 100, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		FF_DONTCARE, "Arial", &font);
	if (result!=D3D_OK)
	{
		MessageBox(hWnd, "Error Creating Sprite Font", "Error", MB_OK);
		return 0;
	}
	return 1;
}

LPDIRECT3DSURFACE9 LoadSurface(string filename, D3DCOLOR transcolor)
{
	LPDIRECT3DSURFACE9 image = NULL;
	D3DXIMAGE_INFO info;
	HRESULT result;

	//get width, height from bitmap file
	result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
	if (result != D3D_OK)
		return NULL;

	//create surface
	result = d3ddev->CreateOffscreenPlainSurface(
		info.Width,
		info.Height,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&image,
		NULL);

	if (result != D3D_OK)
		return NULL;

	//load surface form file into newly created surface
	result = D3DXLoadSurfaceFromFile(
		image,				//destination surface
		NULL,				//destination pallete
		NULL,				//destination rectangle
		filename.c_str(),			//file name
		NULL,				//source rectangle
		D3DX_DEFAULT,		//how image is filtered
		transcolor,					//transparent?
		NULL);				//source image info (usually NULL)

	if (result != D3D_OK)
		return NULL;

	return image;

}

LPDIRECT3DTEXTURE9 LoadTexture(string filename, D3DCOLOR transcolor)
{
	LPDIRECT3DTEXTURE9 texture = NULL;
	D3DXIMAGE_INFO info;
	HRESULT result;

	//get width, height from bitmap file
	result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
	if (result != D3D_OK)
		return NULL;

	//create surface
	result = D3DXCreateTextureFromFileEx(
		d3ddev,
		filename.c_str(),
		info.Width,
		info.Height,
		1,				//Mipmap levels
		D3DPOOL_DEFAULT,			//D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transcolor,		// Transparent color
		&info,				// Image information
		NULL,
		&texture);

	if (result != D3D_OK)
		return NULL;

	return texture;
}

int GetTextWidth(string tmp, LPD3DXFONT font)
{
	RECT rcRect = { 0,0,0,0 };
	if (font)
	{
		// calculate required rect
		font->DrawText(NULL, tmp.c_str(), strlen((LPCSTR)&tmp), &rcRect, DT_CALCRECT,
			D3DCOLOR_XRGB(0, 0, 0));
	}

	// return width
	return rcRect.right - rcRect.left;
}

int GetTextHeight(string tmp, LPD3DXFONT font)
{
	RECT rcRect = { 0,0,0,0 };
	if (font)
	{
		// calculate required rect
		font->DrawText(NULL, tmp.c_str(), strlen((LPCSTR)&tmp), &rcRect, DT_CALCRECT,
			D3DCOLOR_XRGB(0, 0, 0));
	}

	// return width
	return rcRect.bottom - rcRect.top;
}
