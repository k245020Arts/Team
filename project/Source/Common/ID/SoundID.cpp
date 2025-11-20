#include "SoundID.h"
#include <unordered_map>
#include <assert.h>
#include "../Debug/Debug.h"

namespace {

	std::unordered_map<int, std::string> soundId;
}


std::string Sound_ID::GetSoundID(SOUND_ID _type)
{
	int type = (int)_type;
	if (type < 0 && type >= (int)SOUND_MAX) {
		Debug::CreateMessageBox("soundHandleMiss", "miss");
	}
	return soundId[type];
}

void Sound_ID::SetSoundId(std::string _sound, SOUND_ID _type)
{
	int type = (int)_type;
	if (type < 0 && type >= (int)SOUND_MAX) {
		Debug::CreateMessageBox("soundHandleSetMiss", "miss");
	}
	soundId[type] = _sound;
}
