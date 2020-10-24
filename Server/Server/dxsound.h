#pragma once

#include "DSutil.h"
#include <vector>

using namespace std;

enum SoundType
{
	attack_sound = 0,
	attacked_sound = 1,
	boss_died_sound = 2,
	boss_jump_sound = 3,
	enemy_die_sound = 4,
	jump_sound = 5,
	pick_item_sound = 6,
	stage3_1_bgm = 7,
	stage3_2_bgm = 8,
	stageboss_bgm = 9,
	pause_sound = 10,
	score_count_sound = 11,
	skill_sound = 12,
};

extern CSoundManager* soundManager;
extern vector<CSound*> sound;

int init_SoundManager(HWND);

int addSound(string path);
void playSound(SoundType name, bool isLoop = false, LONG volume = 2L);
void stopSound(SoundType name);


