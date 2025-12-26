#pragma once
#include <string>

namespace Effect_ID {

	enum EFFECT_ID
	{
		ENEMY_FLASH,
		HIT_EFFECT,
		SPECIAL_HIT_EFFECT,
		JUST_AVOID_EFFECT,
		PLAYER_FLASH,
		PLAYER_AURA,
		PLAYER_HIT,
		PLAYER_ATTACK,
		PLAYER_SLASH_ATTACK,
		ENEMY_ATTACK_CICLE,
		EFFECT_WALL,
		BOSS_WAVE,
		BOSS_GROUND,
		BOSS_ROAR,

		EFFECT_MAX
	};

	void DeleteEffectID();

	std::string GetEffectID(EFFECT_ID _type);

	void SetEffectID(std::string _effect, EFFECT_ID _type);

	EFFECT_ID StringToID(std::string _str);
}