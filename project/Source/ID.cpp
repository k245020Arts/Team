#include "ID.h"
#include <unordered_map>
#include <assert.h>
#include "Debug.h"

namespace {
	std::unordered_map<int,std::string> modelId;
	std::unordered_map<int,std::string> soundId;
	std::unordered_map<int,std::string> effectId;

	std::string InitID[ID::ID_MAX] = {
		"enemy",
		"player",
		"stage",
		"p_idol",
		"p_run",
		"p_avoid",
		"p_just_avoid",
		"p_attack1",
		"p_just_avoid_attack1",
		"p_attack2",
		"p_just_avoid_attack2",
		"p_attack3",
		"p_just_avoid_attack3",
		"p_attack4",
		"p_just_avoid_attack4",
		"p_just_avoid_attack5",
		"p_damage",
		"p_blowAway",
		"p_fall",
		"p_getUp",
		"p_weapon",
		"e_idol",
		"e_attack",
		"e_damage",
		"e_blowaway",
		"e_fall",
		"e_getUp",
		"e_weapon",
		"c_follow",
		"c_avoid_attack",
		"e_run",
		"c_hit",
		"sky",
		"swordLoad",
		"wall",
		"p_hp_guage",
		"e_die",
		"p_die",
		"start_num",
	};
}

void ID::Init() {

	for (int i = 0; i < ID_MAX; i++) {
		modelId[i] = InitID[i];
	}
}

std::string ID::GetID(IDType _type)
{
	int type = (int)_type;
	if (type < 0 && type >= (int)ID_MAX) {
		Debug::CreateMessageBox("modelHandleMiss", "miss");
	}
	return modelId[type];
}

std::string ID::GetSoundID(SOUND_ID _type)
{
	int type = (int)_type;
	if (type < 0 && type >= (int)SOUND_MAX) {
		Debug::CreateMessageBox("soundHandleMiss", "miss");
	}
	return soundId[type];
}

void ID::SetSoundId(std::string _sound,SOUND_ID _type)
{
	int type = (int)_type;
	if (type < 0 && type >= (int)SOUND_MAX) {
		Debug::CreateMessageBox("soundHandleSetMiss", "miss");
	}
	soundId[type] = _sound;
}

std::string ID::GetEffectID(EFFECT_ID _type)
{
	int type = (int)_type;
	if (type < 0 && type >= (int)EFFECT_MAX) {
		Debug::CreateMessageBox("EffectHandleMiss", "miss");
	}
	return effectId[type];
}

void ID::SetEffectID(std::string _effect, EFFECT_ID _type)
{
	int type = (int)_type;
	if (type < 0 && type >= (int)SOUND_MAX) {
		Debug::CreateMessageBox("EffectHandleMiss", "miss");
	}
	effectId[type] = _effect;
}
