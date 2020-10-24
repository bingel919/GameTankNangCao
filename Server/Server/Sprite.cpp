#include "Sprite.h"



Sprite::Sprite()
{
}


Sprite::~Sprite()
{
}

Sprite::Sprite(string Path)
{
	_Image = LoadTexture(Path, D3DCOLOR_XRGB(255, 0, 255));
}

//void Sprite::Render(Camera camera)
//{
//	D3DXMatrixIdentity(&_renderMatrix);
//	D3DXMATRIX tmp;
//	D3DXVECTOR3 position((float)_X, (float)_Y, 0);
//	position = camera.WorldToView(position);
//
//	//Flip
//	D3DXMatrixScaling(&tmp, FlipX, 1, 1);
//	_renderMatrix *= tmp;
//
//	//ZOOM for View
//	//Translate the sprite
//	D3DXMatrixTranslation(&tmp, position.x, position.y, 0.0f);
//	_renderMatrix *= tmp;
//
//	// Scale the sprite.
//	D3DXMatrixScaling(&tmp, SCALE, SCALE, SCALE);
//	_renderMatrix *= tmp;
//
//	
//	
//	spritehandler->SetTransform(&_renderMatrix);
//	D3DXVECTOR3 center = D3DXVECTOR3((rect.right-rect.left)/2.0f, (rect.top - rect.bottom) / 2.0f, 0);
//	spritehandler->Draw(
//		_Image,
//		&rect,
//		&center,
//		NULL,
//		D3DCOLOR_XRGB(255, 255, 255)
//	);
//}
//
//void Sprite::Render(Camera camera, RECT renderRect)
//{
//	D3DXMatrixIdentity(&_renderMatrix);
//	D3DXMATRIX tmp;
//	D3DXVECTOR3 position((float)_X, (float)_Y, 0);
//	position = camera.WorldToView(position);
//	//Flip
//	D3DXMatrixScaling(&tmp, FlipX, FlipY, 1);
//	_renderMatrix *= tmp;
//
//	//ZOOM for View
//	//Translate the sprite
//	D3DXMatrixTranslation(&tmp, position.x, position.y, 0.0f);
//	_renderMatrix *= tmp;
//
//	// Scale the sprite.
//	D3DXMatrixScaling(&tmp, SCALE, SCALE, SCALE);
//	_renderMatrix *= tmp;
//
//	D3DXVECTOR3 center = D3DXVECTOR3((renderRect.right - renderRect.left) / 2.0f, (renderRect.bottom - renderRect.top), 0);
//	spritehandler->SetTransform(&_renderMatrix);
//	spritehandler->Draw(
//		_Image,
//		&renderRect,
//		&center,
//		NULL,
//		D3DCOLOR_XRGB(255, 255, 255)
//	);
//}
//
//void Sprite::Render(Camera camera, RECT renderRect, D3DXVECTOR3 center)
//{
//	D3DXMatrixIdentity(&_renderMatrix);
//	D3DXMATRIX tmp;
//	D3DXVECTOR3 position((float)_X, (float)_Y, 0);
//	position = camera.WorldToView(position);
//	//Flip
//	D3DXMatrixScaling(&tmp, FlipX, FlipY, 1);
//	_renderMatrix *= tmp;
//
//	//ZOOM for View
//	//Translate the sprite
//	D3DXMatrixTranslation(&tmp, position.x, position.y, 0.0f);
//	_renderMatrix *= tmp;
//
//	// Scale the sprite.
//	D3DXMatrixScaling(&tmp, SCALE, SCALE, SCALE);
//	_renderMatrix *= tmp;
//
//	spritehandler->SetTransform(&_renderMatrix);
//	spritehandler->Draw(
//		_Image,
//		&renderRect,
//		&center,
//		NULL,
//		D3DCOLOR_XRGB(255, 255, 255)
//	);
//}

void Sprite::Render(Camera camera, RECT renderRect, D3DXVECTOR3 center, float X, float Y, float FlipX, float FlipY, float Scale)
{
	D3DXMatrixIdentity(&_renderMatrix);
	D3DXMATRIX tmp;
	D3DXVECTOR3 position(X, Y, 0);
	position = camera.WorldToView(position);

	//Flip
	D3DXMatrixScaling(&tmp, FlipX, FlipY, 1);
	_renderMatrix *= tmp;

	// Scale the sprite.
	D3DXMatrixScaling(&tmp, Scale, Scale, Scale);
	_renderMatrix *= tmp;

	//Translate the sprite
	D3DXMatrixTranslation(&tmp, position.x, position.y, position.z);
	_renderMatrix *= tmp;

	spritehandler->SetTransform(&_renderMatrix);
	spritehandler->Draw(
		_Image,
		&renderRect,
		&center,
		NULL,
		D3DCOLOR_XRGB(255, 255, 255)
	);
}

void Sprite::Render(Camera camera, RECT renderRect, SpecialCenter sp, float X, float Y, float FlipX, float FlipY, float Scale)
{
	D3DXVECTOR3 center;
	switch (sp)
	{
	case MiddleCenter:
		center = D3DXVECTOR3((renderRect.right - renderRect.left) / 2.0f, (renderRect.bottom - renderRect.top) / 2.0f, 0);
		break;
	case BottomCenter:
		center = D3DXVECTOR3((renderRect.right - renderRect.left) / 2.0f, (renderRect.bottom - renderRect.top), 0);
		break;
	case TopCenter:
		center = D3DXVECTOR3((renderRect.right - renderRect.left) / 2.0f, 0, 0);
		break;
	case TopLeft:
		center = D3DXVECTOR3(0, 0, 0);
		break;
	case TopRight:
		center = D3DXVECTOR3((renderRect.right - renderRect.left), 0, 0);
		break;
	case BottomLeft:
		center = D3DXVECTOR3(0, (renderRect.bottom - renderRect.top), 0);
		break;
	case BottomRight:
		center = D3DXVECTOR3((renderRect.right - renderRect.left), (renderRect.bottom - renderRect.top), 0);
		break;
	case LeftCenter:
		center = D3DXVECTOR3(0, (renderRect.bottom - renderRect.top) / 2.0f, 0);
		break;
	case RightCenter:
		center = D3DXVECTOR3((renderRect.right - renderRect.left), (renderRect.bottom - renderRect.top) / 2.0f, 0);
		break;
	default:
		break;
	}
	Render(camera, renderRect, center, X, Y, FlipX, FlipY, Scale);
}

void Sprite::Render(Camera camera, RECT renderRect, Box obj, float Scale, bool isFlip)
{
	D3DXMatrixIdentity(&_renderMatrix);
	D3DXMATRIX tmp;
	D3DXVECTOR3 position = D3DXVECTOR3(obj.GetCenterPos().x, obj.GetCenterPos().y, 0);
	D3DXVECTOR3 center = D3DXVECTOR3(obj.center.x, obj.center.y, 0);
	position = camera.WorldToView(position);

	//Flip
	if (isFlip)
	{
		D3DXMatrixScaling(&tmp, obj.direction.x, obj.direction.y, 1);
		_renderMatrix *= tmp;
	}
	// Scale the sprite.
	D3DXMatrixScaling(&tmp, Scale, Scale, Scale);
	_renderMatrix *= tmp;

	//Translate the sprite
	D3DXMatrixTranslation(&tmp, position.x, position.y, position.z);
	_renderMatrix *= tmp;

	spritehandler->SetTransform(&_renderMatrix);
	spritehandler->Draw(
		_Image,
		&renderRect,
		&center,
		NULL,
		D3DCOLOR_XRGB(255, 255, 255)
	);
}

void Sprite::KillSprite()
{
	if (_Image != NULL)
		_Image->Release();
}

int Sprite::GetWidth()
{
	return _Width;
}

int Sprite::GetHeight()
{
	return _Height;
}

