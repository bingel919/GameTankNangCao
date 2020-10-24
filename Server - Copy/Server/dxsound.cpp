#include "dxsound.h"

CSoundManager* soundManager;
vector<CSound*> sound;

int init_SoundManager(HWND hWnd)
{
	soundManager = new CSoundManager();
	HRESULT result;
	result = soundManager->Initialize(hWnd, DSSCL_PRIORITY);
	if (result != DS_OK)
		return 0;
	result = soundManager->SetPrimaryBufferFormat(2, 22050, 16);
	if (result != DS_OK)
		return 0;
	return 1;
}

int addSound(string path)
{
	if (soundManager == NULL)
		return 0;
	CSound *wave;
	HRESULT result = soundManager->Create(&wave, (LPTSTR)path.c_str());
	if (result != DS_OK)
		return 0;
	sound.push_back(wave);
	return 1;
}

void playSound(SoundType name, bool isLoop, LONG volume)
{
	sound[(int)name]->Play(0, isLoop, volume);
}

void stopSound(SoundType name)
{
	sound[(int)name]->Stop();
}

