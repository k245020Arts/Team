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
		EFFECT_MAX
	};

	std::string GetEffectID(EFFECT_ID _type);

	void SetEffectID(std::string _effect, EFFECT_ID _type);
}