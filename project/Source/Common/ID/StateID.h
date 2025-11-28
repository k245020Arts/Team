#pragma once
#include <string>

namespace StateID{

    enum State_ID
    {
        PLAYER_WAIT_S,
        PLAYER_WALK_S,
        PLAYER_AVOID_S,
        PLAYER_JUST_AVOID_S,
        PLAYER_ATTACK1_S,
        PLAYER_JUST_AVOID_ATTACK1_S,
        PLAYER_ATTACK2_S,
        PLAYER_JUST_AVOID_ATTACK2_S,
        PLAYER_ATTACK3_S,
        PLAYER_JUST_AVOID_ATTACK3_S,
        PLAYER_ATTACK4_S,
        PLAYER_JUST_AVOID_ATTACK4_S,
        PLAYER_JUST_AVOID_ATTACK5_S,
        PLAYER_DAMAGE_S,
        PLAYER_BLOW_AWAY_S,
        PLAYER_DIE_S,

        BOSS_IDOL_S,
        BOSS_RUN_S,
        BOSS_COOL_TIME_S,
        ATTACK_SORTING_S,
        BOSS_NORMAL_ATTACK1_S,
        BOSS_NORMAL_ATTACK2_S,
        BOSS_NORMAL_ATTACK3_S,
        BOSS_NORMAL_ATTACK4_S,
        BOSS_NORMAL_ATTACK5_S,
        BOSS_NORMAL_ATTACK6_S,
        BOSS_SPECIAL_ATTACK1_S,
        BOSS_SPECIAL_SMALL_ATTACK1_S,
        BOSS_SPECIAL_ATTACK2_S,
        BOSS_DIE_S,
        B_ROAR_S,

        FOLLOW_CAMERA_S,
        JUST_AVOID_CAMERA_S,
        JUST_AVOID_ATTACK_CAMERA_S,
        JUST_AVOID_ATTACK_HIT_CAMERA_S,
        FREE_CAMERA_S,

        T_ENEMY_IDOL_S,
        T_ENEMY_RUN_S,
        T_ENEMY_ATTACK_S,
        T_ENEMY_DEAD,

        STATE_MAX,
    };

	std::string GetID(State_ID _type);
	void SetID(std::string _state, State_ID _type);
}