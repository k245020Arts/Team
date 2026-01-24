#include "TrashEnemy.h"
#include "../../Component/Animator/Animator.h"
#include "../../Component/Physics/Physics.h"
#include "../../Common/InputManager/ControllerInputManager.h"
#include "../../Component/Collider/SphereCollider.h"
#include "../../Component/Color/Color.h"
#include "../../Component/Shaker/Shaker.h"
#include "../../../ImGui/imgui.h"
#include "../../Player/Player.h"
#include "../TrashEnemy/EnemyState/EnemyBlowAway.h"
#include "../../Camera/CameraManager.h"
#include "../../Camera/Camera.h"
#include "../../Common/Effect/EffectManager.h"
#include "../../Common/Sound/SoundManager.h"
#include "../../Common/ID/ID.h"//
#include "../../Weapon/WeaponManager.h"
#include "../../State/StateManager.h"
#include "../../GameManager/GameManager.h"
#include "../../Player/PlayerState/AttackState/PlayerAttackStateBase.h"
#include "../../Weapon/SwordEffect.h"
#include "../../Weapon/CharaWeapon.h"
#include "../../Common/LoadManager.h"
#include "../../Player/PlayerState/AttackState/PlayerSpecialAttack.h"
//#include

#include "TrashEnemyState/T_EnemyStatus.h"
#include "TrashEnemyState/T_EnemyIdol.h"
#include "TrashEnemyState/T_EnemyRun.h"
#include "TrashEnemyState/T_EnemyAttack.h"
#include "TrashEnemyState/T_EnemyDead.h"
#include "TrashEnemyState/CooperateAttack1.h"
#include "TrashEnemyState/Standby.h"

namespace
{
	const std::unordered_map<StateID::State_ID, EnemyInformation::EnemyReaction> enemyTable = {
		// 通常攻撃
		{  StateID::PLAYER_ATTACK1_S, { StateID::PLAYER_ATTACK1_S, EnemyDamage::EnemyDamageInfo(VECTOR3(0,0,1500), VECTOR3(100,100,100),0.15f,0.8f),
			EnemyBlowAway::EnemyBlowAwayInfo(), EnemyInformation::EnemyReaction::Type::Normal, StateID::B_THREAT_S, 10, 10,
			Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME, EnemyInformation::HIT_EFFECT_SCALE_RATE,
			true, 40.0f, Effect_ID::PLAYER_SLASH_ATTACK, true, -1, 0.0f } },

		{  StateID::PLAYER_ATTACK2_S, { StateID::PLAYER_ATTACK2_S, EnemyDamage::EnemyDamageInfo(VECTOR3(0,0,1500), VECTOR3(100,100,100),0.25f,0.8f),
			EnemyBlowAway::EnemyBlowAwayInfo(), EnemyInformation::EnemyReaction::Type::Normal,  StateID::B_THREAT_S, 40, 30,
			Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME, EnemyInformation::HIT_EFFECT_SCALE_RATE,
			true, 70.0f * DegToRad, Effect_ID::PLAYER_SLASH_ATTACK, true, -1, 0.0f } },

		{  StateID::PLAYER_ATTACK3_S, { StateID::PLAYER_ATTACK3_S, EnemyDamage::EnemyDamageInfo(VECTOR3(0,0,1500), VECTOR3(100,100,100),0.35f,0.8f),
			EnemyBlowAway::EnemyBlowAwayInfo(), EnemyInformation::EnemyReaction::Type::Normal,  StateID::B_THREAT_S, 10, 10,
			Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME, EnemyInformation::HIT_EFFECT_SCALE_RATE,
			true, 80.0f * DegToRad, Effect_ID::PLAYER_SLASH_ATTACK, true, -1, 0.0f } },

		{  StateID::PLAYER_ATTACK4_S, { StateID::PLAYER_ATTACK4_S, EnemyDamage::EnemyDamageInfo(VECTOR3(0,400,0), VECTOR3(200,200,200),0.85f,1.0f),
			EnemyBlowAway::EnemyBlowAwayInfo(), EnemyInformation::EnemyReaction::Type::Normal,  StateID::B_THREAT_S, 50, 50,
			Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME, EnemyInformation::HIT_EFFECT_SCALE_RATE,
			true, 0.0f * DegToRad, Effect_ID::PLAYER_SLASH_ATTACK, true, -1, 0.0f } },

			// ジャスト回避攻撃
			{ StateID::PLAYER_JUST_AVOID_ATTACK1_S, { StateID::PLAYER_JUST_AVOID_ATTACK1_S, EnemyDamage::EnemyDamageInfo(VECTOR3(0,-20000,2000), VECTOR3(400,400,400),0.05f,0.8f),
				EnemyBlowAway::EnemyBlowAwayInfo(), EnemyInformation::EnemyReaction::Type::Normal,  StateID::B_THREAT_S, 30, 40,
				Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME, EnemyInformation::HIT_EFFECT_SCALE_RATE,
				true, 0.0f, Effect_ID::PLAYER_SLASH_ATTACK, false, -1, 0.0f } },

			{ StateID::PLAYER_JUST_AVOID_ATTACK2_S, { StateID::PLAYER_JUST_AVOID_ATTACK2_S, EnemyDamage::EnemyDamageInfo(), EnemyBlowAway::EnemyBlowAwayInfo(),
				EnemyInformation::EnemyReaction::Type::LoopCombo,  StateID::B_THREAT_S, 0, 0,
				Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME, EnemyInformation::HIT_EFFECT_SCALE_RATE,
				true, 90.0f * DegToRad, Effect_ID::PLAYER_SLASH_ATTACK, true, 5, 0.13f } },

			{StateID::PLAYER_JUST_AVOID_ATTACK3_S, { StateID::PLAYER_JUST_AVOID_ATTACK3_S, EnemyDamage::EnemyDamageInfo(VECTOR3(0,-2000,-2000), VECTOR3(200,200,200),0.2f,0.8f),
				EnemyBlowAway::EnemyBlowAwayInfo(), EnemyInformation::EnemyReaction::Type::Normal,  StateID::B_THREAT_S, 70, 40,
				Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME, EnemyInformation::HIT_EFFECT_SCALE_RATE,
				true, 180.0f * DegToRad, Effect_ID::PLAYER_SLASH_ATTACK, true, -1, 0.0f } },

			{ StateID::PLAYER_JUST_AVOID_ATTACK4_S, { StateID::PLAYER_JUST_AVOID_ATTACK4_S, EnemyDamage::EnemyDamageInfo(VECTOR3(0,0,7000), VECTOR3(200,200,200),0.2f,0.8f),
				EnemyBlowAway::EnemyBlowAwayInfo(), EnemyInformation::EnemyReaction::Type::Normal,  StateID::B_THREAT_S, 40, 40,
				Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME, EnemyInformation::HIT_EFFECT_SCALE_RATE,
				true, 80.0f * DegToRad, Effect_ID::PLAYER_SLASH_ATTACK, true, -1, 0.0f } },

				// 吹っ飛ばし攻撃
				{ StateID::PLAYER_JUST_AVOID_ATTACK5_S, { StateID::PLAYER_JUST_AVOID_ATTACK5_S, EnemyDamage::EnemyDamageInfo(),
					EnemyBlowAway::EnemyBlowAwayInfo(4000,3000,VECTOR3(15,15,15),VECTOR3(0.3f,0.3f,0.3f),2000,20),
					EnemyInformation::EnemyReaction::Type::BlowAway,  StateID::B_THREAT_S, 100, 60,
					Effect_ID::SPECIAL_HIT_EFFECT, 5.0f, EnemyInformation::HIT_EFFECT_SCALE_RATE,
					true, 0.0f, Effect_ID::PLAYER_SLASH_ATTACK, true, -1, 0.0f } },

			{  StateID::PLAYER_SPECIAL_ATTACK_S, { StateID::PLAYER_SPECIAL_ATTACK_S, EnemyDamage::EnemyDamageInfo(VECTOR3(0,0,1500), VECTOR3(100,100,100),0.15f,0.8f),
		EnemyBlowAway::EnemyBlowAwayInfo(), EnemyInformation::EnemyReaction::Type::Special, StateID::B_THREAT_S, 10, 10,
		Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME, EnemyInformation::HIT_EFFECT_SCALE_RATE,
		true, 40.0f, Effect_ID::PLAYER_SLASH_ATTACK, true, -1, 0.0f } },

	};
}

TrashEnemy::TrashEnemy()
{
	tag = Function::GetClassNameC<TrashEnemy>();
	eStatus = new T_EnemyStatus;

	/*hp = eStatus->GetStatus().maxHp;
	maxHp = hp;*/

	//speed = eStatus->GetStatus().runSpeed;
	isAttack = false;
	isStandby = false;
	isCooperateAtk = false;

	isEnemyFollow = false;

	wayPoint = VZero;

	isMovingToPlayer = false;

	slowCounter = 0;
}

TrashEnemy::~TrashEnemy()
{
	delete eStatus;
	eStatus = nullptr;
}

void TrashEnemy::Update()
{
	slowCounter += Time::DeltaTimeRate();
	EnemyBase::Update();

	if (hp <= 0)
		enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_DEAD);

	/*if (!isCooperateAtk)
		targetPos = enemyBaseComponent.playerObj->GetTransform()->position;*/

	if (CheckHitKey(KEY_INPUT_9))
		hp -= maxHp;
}

void TrashEnemy::Draw()
{
	EnemyBase::Draw();
}

void TrashEnemy::Start(Object3D* _obj)
{
	enemyBaseComponent.state = obj->Component()->AddComponent<StateManager>();
	enemyBaseComponent.playerObj = FindGameObjectWithTag<Object3D>("PLAYER");
	pState = enemyBaseComponent.playerObj->Component()->GetComponent<Player>()->GetPlayerStateManager();

	enemyBaseComponent.collider = FindGameObjects<ColliderBase>();

	enemyBaseComponent.anim = obj->Component()->GetComponent<Animator>();
	enemyBaseComponent.physics = obj->Component()->GetComponent<Physics>();
	enemyBaseComponent.control = FindGameObject<ControllerInputManager>();
	enemyBaseComponent.color = obj->Component()->GetComponent<Color>();
	enemyBaseComponent.color->setRGB(Color::Rgb(0.0f, 0.0f, 0.0f, 255.0f));
	enemyBaseComponent.shaker = obj->Component()->GetComponent<Shaker>();
	enemyBaseComponent.effect = FindGameObject<EffectManager>();
	enemyBaseComponent.sound = FindGameObject<SoundManager>();
	enemyBaseComponent.physics = obj->Component()->GetComponent<Physics>();

	using namespace ID;

	enemyBaseComponent.state->CreateState<T_EnemyIdol>("_T_EnemyIdol", StateID::T_ENEMY_IDOL_S);
	enemyBaseComponent.state->CreateState<T_EnemyRun>("_T_EnemyRun", StateID::T_ENEMY_RUN_S);
	enemyBaseComponent.state->CreateState<T_EnemyAttack>("_T_EnemyAttack", StateID::T_ENEMY_ATTACK_S);
	enemyBaseComponent.state->CreateState<T_EnemyDead>("_T_EnemyDead", StateID::T_ENEMY_DEAD);
	enemyBaseComponent.state->CreateState<CooperateAttack1>("_CooperateAttack1", StateID::COOPERATEATTACK1);
	enemyBaseComponent.state->CreateState<Standby>("_Standby", StateID::T_ENEMY_STANDBY);
	enemyBaseComponent.state->CreateState<EnemyDamage>("_T_EnemyDamage", StateID::T_ENEMY_DAMAGE);

	enemyBaseComponent.state->SetComponent<TrashEnemy>(this);

	// スタートステートも StateID 化
	enemyBaseComponent.state->StartState(StateID::T_ENEMY_IDOL_S);
	enemyBaseComponent.weapon = FindGameObject<WeaponManager>();

	chara = obj->Component()->AddComponent<CharaWeapon>();
	chara->ObjectPointer(_obj, 10, ID::E_MODEL, -1);
	chara->SetImage(Load::GetHandle(ID::SWORD_EFFECT_B));

	//playerSp = enemyBaseComponent.playerObj->Component()->GetComponent<PlayerSpecialAttack>();

	active = true;
}

void TrashEnemy::CreateTrashEnemy(VECTOR3 _pos, int kinds)
{
	obj->GetTransform()->position = _pos;
	number = kinds;

	const float MAX = 1.2f;
	const float MIN = 0.8f;

	switch (kinds)
	{
	case 0://すばしっこい敵
		hp = eStatus->GetStatus().maxHp * MIN;
		maxHp = hp;
		speed = eStatus->GetStatus().runSpeed * MAX;
		GetEnemyObj()->GetTransform()->scale = GetEnemyObj()->GetTransform()->scale * MIN;
		break;
	case 1:
		hp = eStatus->GetStatus().maxHp;
		maxHp = hp;
		speed = eStatus->GetStatus().runSpeed;
		break;
	default://重い敵
		hp = eStatus->GetStatus().maxHp * MAX;
		maxHp = hp;
		speed = eStatus->GetStatus().runSpeed * MIN;
		GetEnemyObj()->GetTransform()->scale = GetEnemyObj()->GetTransform()->scale * MAX;
		break;
	}
}

void TrashEnemy::LookTarget()
{
	VECTOR3 distance = targetPos - obj->GetTransform()->position;
	//向くべき角度
	float direction = -atan2f(distance.z, distance.x) - 0.5f * DX_PI_F;
	//その角度とどれだけ差があるか
	float sign = direction - obj->GetTransform()->rotation.y;
	//切り捨てして180の境界線を無くす
	sign -= floorf(sign / DX_PI_F / 2) * DX_PI_F * 2.0f;
	if (sign > DX_PI_F)
		sign -= 2 * DX_PI_F;
	//向くスピード(ラジアン)
	const float LOOK_SPEED = 0.5;
	//Playerの方をゆっくり向く
	if (sign > LOOK_SPEED)
		obj->GetTransform()->rotation.y += LOOK_SPEED;
	else if (sign < -LOOK_SPEED)
		obj->GetTransform()->rotation.y -= LOOK_SPEED;
	else
		obj->GetTransform()->rotation.y = direction;
}

bool TrashEnemy::IsPlayerSpecialMove()
{
	if (pState->GetState<PlayerSpecialAttack>() == nullptr)
		return false;
	else
		return true;
}

void TrashEnemy::Trail()
{
	chara->CreateSwordEffect(VECTOR3(70, 0, -50), VECTOR3(120, 0, 50), 200.0f, 10.0f, 00.0f, 155.0f, 28, 0.5f);
}

void TrashEnemy::PlayerHit()
{
	if (hp <= 0)
		return;

	StateID::State_ID attackID = pState->GetState<PlayerStateBase>()->GetID();
	float damage = 0;;
	if (pState->GetState<PlayerAttackStateBase>() != nullptr)
	{
		damage = pState->GetState<PlayerAttackStateBase>()->GetHitDamage();
	}
	else
	{
		loopNum = -1;
	}
	EnemyDamage::EnemyDamageInfo dInfo;
	EnemyBlowAway::EnemyBlowAwayInfo bInfo;
	float random[3] = {};
	//エフェクトの位置をランダムに派生
	for (float& r : random)
	{
		r = (float)GetRand(80) - 40.0f;
	}
	//プレイヤーの攻撃の種類によってダメージや吹っ飛び方の種類を変える
	float angleRan = 0.0f;
	bool lastAttack = false;
	bool lastBeforeAttack = false;

	auto bossParam = enemyTable.find(attackID);
	if (bossParam != enemyTable.end())
	{
		const auto& e = bossParam->second;
		switch (e.attackType)
		{
		case EnemyInformation::EnemyReaction::Type::Normal:
			enemyBaseComponent.control->ControlVibrationStartFrame(e.vibrationPower, e.vibrationType);
			enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * e.hitEffectScaleRate), obj, e.hitEffectID, e.hitEffectTime);
			enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, e.slashAngleRad), VOne), obj, e.slashEffectID, 1.0f);
			hit = true;
			break;
		case EnemyInformation::EnemyReaction::Type::BlowAway:

			break;
		case EnemyInformation::EnemyReaction::Type::LoopCombo:
			enemyBaseComponent.sound->RandamSe("swordHit00000", 7);
			if (loopNum == -1)
			{
				hitCounter = 0.23f;
				loopNum = 5;
			}
			else if (loopNum == 1)
			{
				hitCounter = 0.13f;
				loopNum--;
				lastBeforeAttack = true;
			}
			else if (loopNum == 0)
			{
				loopNum = -1;
				lastAttack = true;
			}
			else if (loopNum > 0)
			{
				hitCounter = 0.13f;
				loopNum--;
			}
			if (lastAttack)
			{
				//enemyBaseComponent.state->NowChangeState(ID::E_DAMAGE);
				dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 3500.0f), VECTOR3(100, 100, 100), 0.5f, 1.2f);
				enemyBaseComponent.control->ControlVibrationStartFrame(250, 60);
				enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * EnemyInformation::HIT_EFFECT_SCALE_RATE * 14.5f), obj, Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME);
				//angleRan = GetRand(360);
				enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(random[0] * 2.0f, 100, random[2]), VOne * VECTOR3(0, 0, 90 * DegToRad), VOne * 1.5f), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
				hit = true;
			}
			else if (lastBeforeAttack)
			{
				dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(100, 100, 100), 0.5f, 0.5f);
				enemyBaseComponent.shaker->ShakeStart(VECTOR3(20, 20, 20), Shaker::HORIZONAL_SHAKE, true, 0.3f);
				enemyBaseComponent.control->ControlVibrationStartFrame(250, 60);
				enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * EnemyInformation::HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME);
				angleRan = (float)GetRand(360);
				enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(random[0] * 2.0f, 100, random[2]), VOne * VECTOR3(0, 0, 90.0f * DegToRad), VOne * 1.5f), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
			}
			else
			{
				//enemyBaseComponent.state->NowChangeState(ID::E_DAMAGE);
				dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(20, 20, 20), 0.2f, 0.02f);
				enemyBaseComponent.control->ControlVibrationStartFrame(450, 20);
				enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * EnemyInformation::HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME);
				angleRan = (float)GetRand(360);
				enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(random[0] * 2.0f, 100, random[2]), VOne * VECTOR3(0, 0, angleRan * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
				enemyBaseComponent.playerObj->Component()->GetComponent<Shaker>()->ShakeStart(VECTOR3(200, 200, 200), Shaker::HORIZONAL_SHAKE, true, 0.05f);
			}
			hit = true;
			break;
		case EnemyInformation::EnemyReaction::Type::Special:
			if (!specialAttackHit) {
				return;
			}
			enemyBaseComponent.control->ControlVibrationStartFrame(e.vibrationPower, e.vibrationType);
			enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * e.hitEffectScaleRate), obj, e.hitEffectID, e.hitEffectTime);
			enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, e.slashAngleRad), VOne), obj, e.slashEffectID, 1.0f);
			specialAttackHit = false;
			break;
		default:
			break;
		}
	}
	EnemyDamageMove(dInfo);
	
	hp -= damage;
	//ダメージか吹っ飛ばしの状態になっていたらダメージのパラメーターをいれる。
	std::shared_ptr<EnemyDamage> eD = enemyBaseComponent.state->GetState<EnemyDamage>();
	std::shared_ptr <EnemyBlowAway> eB = enemyBaseComponent.state->GetState<EnemyBlowAway>();
	enemyBaseComponent.sound->RandamSe("E_DamageV", 2);
	enemyBaseComponent.color->setRGB(Color::Rgb(255, 0, 0, 255));
	damageFlash = 0.5f;
}

void TrashEnemy::GetWayPoint(VECTOR3 _pos, StateID::State_ID _id)
{
	if (_pos.Size() >= eStatus->GetStatus().chaseRange)
		return;

	wayPoint = _pos;

	enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_RUN_S);
	
	isCooperateAtk = true;
}

void TrashEnemy::ChangeState(StateID::State_ID _id)
{
	enemyBaseComponent.state->ChangeState(_id);
}

void TrashEnemy::AttackCommand()
{

	enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_ATTACK_S);
}

void TrashEnemy::CooperateAtkFinish()
{
	isCooperateAtk = false;
	isMovingToPlayer = false;
	enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_IDOL_S);
}

void TrashEnemy::Move(float _speed, float _max)
{
	VECTOR3 pos = GetBaseObject()->GetTransform()->position;

	// ターゲット座標へ向かうベクトル
	VECTOR3 dir = VECTOR3(targetPos - pos).Normalize();
	dir.y = 0;

	// スピードをかける
	VECTOR3 velocity = dir * _speed;

	enemyBaseComponent.physics->AddVelocity(velocity, false);

	VECTOR3 moveVelo = enemyBaseComponent.physics->GetVelocity() * VECTOR3(1, 0, 1);

	// 最大速度制限
	if (moveVelo.SquareSize() >= _max * _max) {
		moveVelo = moveVelo.Normalize() * _max;
		moveVelo.y = enemyBaseComponent.physics->GetVelocity().y;
		enemyBaseComponent.physics->SetVelocity(moveVelo);
	}
}

void TrashEnemy::AddPos(VECTOR3 _pos)
{
	obj->GetTransform()->position = obj->GetTransform()->position + _pos;
}
