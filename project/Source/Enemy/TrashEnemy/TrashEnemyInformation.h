#pragma once
#include "../EnemyBase.h"

namespace EnemyTable
{
	const std::unordered_map<StateID::State_ID, EnemyInformation::EnemyReaction> enemyTable = {

	{ StateID::PLAYER_ATTACK1_S,
		EnemyInformation::EnemyReaction(
			StateID::PLAYER_ATTACK1_S,
			EnemyDamage::EnemyDamageInfo(VECTOR3(0,0,0), VECTOR3(100,100,100), 0.15f, 0.8f),
			EnemyBlowAway::EnemyBlowAwayInfo(),
			EnemyInformation::EnemyReaction::Type::Normal,
			StateID::STATE_MAX,
			10, 10,
			Effect_ID::HIT_EFFECT,
			EnemyInformation::HIT_EFFECT_TIME,
			EnemyInformation::HIT_EFFECT_SCALE_RATE,
			true,
			40.0f,
			Effect_ID::PLAYER_SLASH_ATTACK,
			true,
			-1,
			0.0f
		)
	},

	{ StateID::PLAYER_ATTACK2_S,
		EnemyInformation::EnemyReaction(
			StateID::PLAYER_ATTACK2_S,
			EnemyDamage::EnemyDamageInfo(VECTOR3(0,0,0), VECTOR3(100,100,100), 0.25f, 0.8f),
			EnemyBlowAway::EnemyBlowAwayInfo(),
			EnemyInformation::EnemyReaction::Type::Normal,
			StateID::STATE_MAX,
			40, 30,
			Effect_ID::HIT_EFFECT,
			EnemyInformation::HIT_EFFECT_TIME,
			EnemyInformation::HIT_EFFECT_SCALE_RATE,
			true,
			70.0f * DegToRad,
			Effect_ID::PLAYER_SLASH_ATTACK,
			true,
			-1,
			0.0f
		)
	},

	{ StateID::PLAYER_ATTACK3_S,
		EnemyInformation::EnemyReaction(
			StateID::PLAYER_ATTACK3_S,
			EnemyDamage::EnemyDamageInfo(VECTOR3(0,0,0), VECTOR3(100,100,100), 0.35f, 0.8f),
			EnemyBlowAway::EnemyBlowAwayInfo(),
			EnemyInformation::EnemyReaction::Type::Normal,
			StateID::STATE_MAX,
			10, 10,
			Effect_ID::HIT_EFFECT,
			EnemyInformation::HIT_EFFECT_TIME,
			EnemyInformation::HIT_EFFECT_SCALE_RATE,
			true,
			80.0f * DegToRad,
			Effect_ID::PLAYER_SLASH_ATTACK,
			true,
			-1,
			0.0f
		)
	},

	{ StateID::PLAYER_ATTACK4_S,
		EnemyInformation::EnemyReaction(
			StateID::PLAYER_ATTACK4_S,
			EnemyDamage::EnemyDamageInfo(VECTOR3(0,10,0), VECTOR3(200,200,200), 0.85f, 1.0f),
			EnemyBlowAway::EnemyBlowAwayInfo(),
			EnemyInformation::EnemyReaction::Type::Normal,
			StateID::STATE_MAX,
			50, 50,
			Effect_ID::HIT_EFFECT,
			EnemyInformation::HIT_EFFECT_TIME,
			EnemyInformation::HIT_EFFECT_SCALE_RATE,
			true,
			0.0f * DegToRad,
			Effect_ID::PLAYER_SLASH_ATTACK,
			true,
			-1,
			0.0f
		)
	},

	{ StateID::PLAYER_JUST_AVOID_ATTACK1_S,
		EnemyInformation::EnemyReaction(
			StateID::PLAYER_JUST_AVOID_ATTACK1_S,
			EnemyDamage::EnemyDamageInfo(),
			EnemyBlowAway::EnemyBlowAwayInfo(),
			EnemyInformation::EnemyReaction::Type::LoopCombo,
			StateID::STATE_MAX,
			0, 0,
			Effect_ID::HIT_EFFECT,
			EnemyInformation::HIT_EFFECT_TIME,
			EnemyInformation::HIT_EFFECT_SCALE_RATE,
			true,
			90.0f * DegToRad,
			Effect_ID::PLAYER_SLASH_ATTACK,
			true,
			5,
			0.13f
		)
	},

	{ StateID::PLAYER_ATTACK5_S,
		EnemyInformation::EnemyReaction(
			StateID::PLAYER_ATTACK5_S,
			EnemyDamage::EnemyDamageInfo(VECTOR3(0,0,0), VECTOR3(200,200,200), 0.2f, 0.8f),
			EnemyBlowAway::EnemyBlowAwayInfo(),
			EnemyInformation::EnemyReaction::Type::Normal,
			StateID::STATE_MAX,
			70, 40,
			Effect_ID::HIT_EFFECT,
			EnemyInformation::HIT_EFFECT_TIME,
			EnemyInformation::HIT_EFFECT_SCALE_RATE,
			true,
			180.0f * DegToRad,
			Effect_ID::PLAYER_SLASH_ATTACK,
			true,
			-1,
			0.0f
		)
	},

	{ StateID::PLAYER_ATTACK6_S,
	EnemyInformation::EnemyReaction(
		StateID::PLAYER_ATTACK6_S,
		EnemyDamage::EnemyDamageInfo(VECTOR3(0,000,4000), VECTOR3(200,200,200), 0.2f, 0.8f,false,4000.0f),
		EnemyBlowAway::EnemyBlowAwayInfo(),
		EnemyInformation::EnemyReaction::Type::Normal,
		StateID::T_ENEMY_DAMAGE,
		70, 40,
		Effect_ID::HIT_EFFECT,
		EnemyInformation::HIT_EFFECT_TIME,
		EnemyInformation::HIT_EFFECT_SCALE_RATE,
		true,
		90.0f * DegToRad,
		Effect_ID::PLAYER_SLASH_ATTACK,
		true,
		-1,
		0.0f
	)
	},

	{ StateID::PLAYER_SPECIAL_ATTACK_S,
		EnemyInformation::EnemyReaction(
			StateID::PLAYER_SPECIAL_ATTACK_S,
			EnemyDamage::EnemyDamageInfo(VECTOR3(0,0,1500), VECTOR3(100,100,100), 0.15f, 0.8f),
			EnemyBlowAway::EnemyBlowAwayInfo(),
			EnemyInformation::EnemyReaction::Type::Special,
			StateID::STATE_MAX,
			10, 10,
			Effect_ID::HIT_EFFECT,
			EnemyInformation::HIT_EFFECT_TIME,
			EnemyInformation::HIT_EFFECT_SCALE_RATE,
			true,
			40.0f,
			Effect_ID::PLAYER_SLASH_ATTACK,
			true,
			-1,
			0.0f
		)
	},

	{ StateID::PLAYER_HEAVY_ATTACK_S,
	EnemyInformation::EnemyReaction(
		StateID::PLAYER_HEAVY_ATTACK_S,
		EnemyDamage::EnemyDamageInfo(VECTOR3(0,-2000,-2000), VECTOR3(200,200,200), 0.2f, 0.8f),
		EnemyBlowAway::EnemyBlowAwayInfo(),
		EnemyInformation::EnemyReaction::Type::Normal,
		StateID::STATE_MAX,
		70, 40,
		Effect_ID::HIT_EFFECT,
		EnemyInformation::HIT_EFFECT_TIME,
		EnemyInformation::HIT_EFFECT_SCALE_RATE,
		true,
		180.0f * DegToRad,
		Effect_ID::PLAYER_SLASH_ATTACK,
		true,
		-1,
		0.0f
	)
	},
	{ StateID::PLAYER_HEAVY_ATTACK2_S,
	EnemyInformation::EnemyReaction(
	StateID::PLAYER_HEAVY_ATTACK2_S,
	EnemyDamage::EnemyDamageInfo(VECTOR3(0,000,3000), VECTOR3(200,200,200), 0.2f, 0.8f,false,4000.0f),
	EnemyBlowAway::EnemyBlowAwayInfo(),
	EnemyInformation::EnemyReaction::Type::Normal,
	StateID::STATE_MAX,
	70, 40,
	Effect_ID::HIT_EFFECT,
	EnemyInformation::HIT_EFFECT_TIME,
	EnemyInformation::HIT_EFFECT_SCALE_RATE,
	true,
	180.0f * DegToRad,
	Effect_ID::PLAYER_SLASH_ATTACK,
	true,
	-1,
	0.0f
)
	},

	};
}