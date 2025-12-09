#include "Sound.h"
#include "../../../Library/myDxLib.h"
#include "../../../Library/Time.h"
#include "../Easing.h"

Sound::Sound()
{
	feedInTime = 0.0f;
	feedOutTime = 0.0f;
}

Sound::~Sound()
{
	StopSound();
}

void Sound::Update()
{
	if (feedInTime > 0.0f) {
		float valume = Easing::EasingFlow<float>(&feedInTime, timeMax, volume, 0, Easing::EaseIn<float>);
		ChangeVolumeSoundMem(valume, soundHandle);
	}
	if (feedOutTime > 0.0f) {
		float valume = Easing::EasingFlow<float>(&feedOutTime, timeMax, 0, volume, Easing::EaseIn<float>);
		ChangeVolumeSoundMem(valume, soundHandle);
	}
}

void Sound::SoundData(int _soundHandle, int _volume)
{
	soundHandle = _soundHandle;
	volume = _volume;
	lastPlayintBgm = _soundHandle;
	ChangeVolumeSoundMem(volume,soundHandle);
}

void Sound::PlaySe()
{
	PlaySoundMem(soundHandle, DX_PLAYTYPE_BACK);
	loop = false;
}

void Sound::PlayBGM(bool _loop, bool firstPlay)
{
	loop = _loop;
	if (!loop) {
		PlaySoundMem(soundHandle, DX_PLAYTYPE_BACK);
	}
	else {
		PlaySoundMem(soundHandle, DX_PLAYTYPE_LOOP,firstPlay);
	}
	
}


void Sound::StopBGM()
{
	StopSoundMem(soundHandle);
}

void Sound::StopSE()
{
	StopSoundMem(soundHandle);
}

void Sound::BGMFeedIn(float time)
{
	feedInTime = time;
	timeMax = time;
	active = true;
}

void Sound::BGMFeedOut(float time)
{
	feedOutTime = time;
	timeMax = time;
	active = true;
}
