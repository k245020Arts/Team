#include "Sound.h"

Sound::Sound()
{

}

Sound::~Sound()
{

}

void Sound::SoundData(int _soundHandle, int _volume)
{
	soundHandle = _soundHandle;
	volume = _volume;
	lastPlayintBgm = _soundHandle;
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
