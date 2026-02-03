#pragma once
#include "../../../Library/myDxLib.h"
#include <string>

class Sound
{
public:
	Sound();
	~Sound();

	void Update();

	void SoundData(int _soundHandle,int _volume);
	//’ÊíÄ¶
	void PlaySe();
	//bgm‚ÌÄ¶
	void PlayBGM(bool _loop,bool firstPlay);

	//ƒTƒEƒ“ƒh‚ğ~‚ß‚é
	void StopBGM();
	void StopSE();

	void BGMFeedIn(float time);
	void BGMFeedOut(float time);

	bool IsActive() { return active; }
	void ChangeVolumeSound(float _volume);
	void BaseChangeVolumeSound();

	const float GetVolumn() { return volume; }

	void PlayRamdomChangeFrequencySe(int _ramdom,int _baseFrequ);

private:

	int soundHandle;
	int volume;
	bool loop;
	bool firstPlay;
	int lastPlayintBgm;

	float feedInTime;
	float feedOutTime;
	float timeMax;

	bool active;
};