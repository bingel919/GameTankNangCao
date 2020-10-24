#include "Camera.h"



Camera::Camera()
{
}

Camera::Camera(int gameWidth)
{
	_Speed = CAMERA_SPEED;
	_X = 0;
	_Y = SCREEN_HEIGHT;
	_gameWidth = gameWidth;
}


Camera::~Camera()
{
}

D3DXVECTOR3 Camera::WorldToView(D3DXVECTOR3 pos)
{
	D3DXMATRIX tmp;
	D3DXMatrixIdentity(&tmp);
	tmp._22 = -1.0f;
	tmp._41 = -_X;
	tmp._42 = _Y;
	D3DXVECTOR4 tmpPos;
	D3DXVec3Transform(&tmpPos, &pos, &tmp);

	D3DXVECTOR3 result(tmpPos.x, tmpPos.y, 0);

	return result;
}

void Camera::Update(int x)
{
	/*if (Key_Down(DIK_UP))
		_Y += _Speed;
	if (Key_Down(DIK_DOWN))
		_Y -= _Speed;
	if (Key_Down(DIK_LEFT))
		_X -= _Speed;
	if (Key_Down(DIK_RIGHT))
		_X += _Speed;*/

	if (x - _X > 1.0f / 2 * (SCREEN_WIDTH * 1.0f))
		_X = x - 1.0f / 2 * (SCREEN_WIDTH * 1.0f);
	if (x - _X < 1.0f / 2 * (SCREEN_WIDTH * 1.0f))
		_X = x - 1.0f / 2 * (SCREEN_WIDTH * 1.0f);

	if (_X < 0)
		_X = 0;
	if (_X > _gameWidth - SCREEN_WIDTH)
		_X = _gameWidth - SCREEN_WIDTH;
	/*if (_Y < SCREEN_HEIGHT / SCALE)
		_Y = SCREEN_HEIGHT / SCALE;
	if (_Y > GAME_HEIGHT)
		_Y = GAME_HEIGHT;*/

}
