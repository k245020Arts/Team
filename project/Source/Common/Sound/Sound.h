#pragma once
#include "../../../Library/myDxLib.h"
#include <string>

class Sound
{
public:
	Sound();
	~Sound();
	void SoundData(int _soundHandle,int _volume);
	//’ÊíÄ¶
	void PlaySe();
	//bgm‚ÌÄ¶
	void PlayBGM(bool _loop,bool firstPlay);

	//ƒTƒEƒ“ƒh‚ğ~‚ß‚é
	void StopBGM();
	void StopSE();


private:

	int soundHandle;
	int volume;
	bool loop;
	bool firstPlay;
	int lastPlayintBgm;
};