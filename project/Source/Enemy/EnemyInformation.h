#pragma once
#include "../Common/ID/EffectID.h"
#include "TrashEnemy/EnemyState/EnemyBlowAway.h"
#include "TrashEnemy/EnemyState/EnemyDamage.h"

class StateManager;
class Enemy;
class TrashEnemy;
class Animator;
class Physics;
class ControllerInputManager;
class Color;
class Shaker;
class BaseObject;
class Camera;
class EffectManager;
class SoundManager;
class WeaponManager;
class GameManager;
class ColliderBase;

namespace EnemyInformation {

	struct EnemyComponent
	{
		EnemyComponent() {
			state = nullptr;
			enemy = nullptr;
			physics = nullptr;
			camera = nullptr;
			control = nullptr;
			playerObj = nullptr;
			anim = nullptr;
			color = nullptr;
			shaker = nullptr;
			effect = nullptr;
			sound = nullptr;
			weapon = nullptr;
			gameManager = nullptr;
			trashEnemy = nullptr;
		}
		StateManager* state;
		Enemy* enemy;
		TrashEnemy* trashEnemy;
		Physics* physics;
		Animator* anim;
		ControllerInputManager* control;
		Color* color;
		Shaker* shaker;
		BaseObject* playerObj;
		Camera* camera;
		EffectManager* effect;
		SoundManager* sound;
		WeaponManager* weapon;
		GameManager* gameManager;
		std::list<ColliderBase*> collider;
	};

	//最初のポジション
	const VECTOR3 BASE_POS = VECTOR3(500, 1000, 0);
	const float HIT_EFFECT_TIME = 0.2f;
	const float HIT_EFFECT_SCALE_RATE = 0.1f;

	enum AttackType {
		Normal,         // 通常攻撃
		JustAvoid,      // ジャスト回避攻撃
		BlowAway,       // 吹っ飛ばし
		LoopCombo       // 多段ヒット・特殊コンボ
	};

	struct EnemyReaction {
		ID::IDType attackID;

		EnemyDamage::EnemyDamageInfo dInfo;
		EnemyBlowAway::EnemyBlowAwayInfo bInfo;

		enum class Type { Normal, BlowAway, LoopCombo } attackType;
		ID::IDType changeStateID;

		int vibrationPower;
		int vibrationType;

		Effect_ID::EFFECT_ID hitEffectID;
		float hitEffectTime;
		float hitEffectScaleRate;

		bool hit;
		float slashAngleRad;
		Effect_ID::EFFECT_ID slashEffectID;
		bool useSlashEffect;

		int loopMax;          // -1でループなし
		float loopInterval;   // ループ間隔
	};
	
};