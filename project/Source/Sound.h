#pragma once
#include "../Library/myDxLib.h"
#include <string>

class Sound
{
public:
	Sound();
	~Sound();
	void SoundData(int _soundHandle,int _volume);

	void PlaySe();
	void PlayBGM(bool _loop,bool firstPlay);

	void StopBGM();
	void StopSE();


private:

	int soundHandle;
	int volume;
	bool loop;
	bool firstPlay;
	int lastPlayintBgm;
};