#include "Boss.h"
#include "Animator.h"
#include "Physics.h"
#include "controllerInputManager.h"
#include "sphereCollider.h"
#include "color.h"
#include "Shaker.h"
#include "../ImGui/imgui.h"
#include "player.h"
#include "EnemyBlowAway.h"
#include "cameraManager.h"
#include "camera.h"
#include "EffectManager.h"
#include "SoundManager.h"
#include "weaponManager.h"
#include "stateManager.h"
#include "GameManager.h"
#include "BossIdol.h"
#include "BossRun.h"
#include "BossNormalAttack1.h"
#include "BossNormalAttack2.h"
#include "BossNormalAttack3.h"
#include "enemyDamage.h"
#include "PlayerAttackStateBase.h"

Boss::Boss()
{
	debugId = 19;
	tag = Function::GetClassNameC<Boss>();
}

Boss::~Boss()
{
}

void Boss::Update()
{
	EnemyBase::Update();
}

void Boss::Draw()
{
}

void Boss::Start(Object3D* _obj) 
{
	enemyBaseComponent.state = obj->Component()->AddComponent<StateManager>();
	enemyBaseComponent.playerObj = FindGameObjectWithTag<Object3D>("PLAYER");
	pState = enemyBaseComponent.playerObj->Component()->GetComponent<Player>()->GetPlayerStateManager();

	bossTransform = obj->GetTransform();

	//enemyBaseComponent.boss = this;
	enemyBaseComponent.anim = obj->Component()->GetComponent<Animator>();
	enemyBaseComponent.physics = obj->Component()->GetComponent<Physics>();
	enemyBaseComponent.control = FindGameObject<ControllerInputManager>();
	enemyBaseComponent.color = obj->Component()->GetComponent<Color>();
	enemyBaseComponent.color->setRGB(Color::Rgb(0.0f, 0.0f, 0.0f, 255.0f));
	enemyBaseComponent.shaker = obj->Component()->GetComponent<Shaker>();
	enemyBaseComponent.effect = FindGameObject<EffectManager>();
	enemyBaseComponent.sound = FindGameObject<SoundManager>();
	enemyBaseComponent.physics = obj->Component()->GetComponent<Physics>();

	enemyBaseComponent.camera = FindGameObject<CameraManager>()->GetCamera()->Component()->GetComponent<Camera>();
	//enemyBaseComponent.weapon = FindGameObject<WeaponManager>();

	attackColl = nullptr;
	collName = "b_attack";

	enemyBaseComponent.weapon = FindGameObject<WeaponManager>();

	enemyBaseComponent.gameManager = FindGameObject<GameManager>();

	using namespace ID;

	enemyBaseComponent.state->CreateState<BossIdol>(GetID(B_IDOL));
	enemyBaseComponent.state->CreateState<BossRun>(GetID(B_RUN));
	enemyBaseComponent.state->CreateState<BossNormalAttack1>(GetID(B_N_ATTACK1));
	enemyBaseComponent.state->CreateState<BossNormalAttack2>(GetID(B_N_ATTACK2));
	enemyBaseComponent.state->CreateState<BossNormalAttack3>(GetID(B_N_ATTACK3));
	/*eCom.state->NodeDrawReady();*/

	enemyBaseComponent.state->SetComponent<Boss>(this);

	enemyBaseComponent.state->StartState(B_IDOL);
}

void Boss::LookPlayer()
{
	//プレイヤーのポジションを格納させる
	VECTOR3 targetPos = enemyBaseComponent.playerObj->GetTransform()->position;
	//Playerの方をゆっくり向く
	VECTOR3 distance = targetPos - bossTransform->position;

	float direction = -atan2f(distance.z, distance.x) - 0.5f * DX_PI_F;
	float sign = direction - bossTransform->rotation.y;

	sign -= floorf(sign / DX_PI_F / 2) * DX_PI_F * 2;
	const float LOOK_SPEED = 0.2f;
	if (sign > DX_PI_F)
		sign -= 2 * DX_PI_F;
	if (sign > LOOK_SPEED)
		bossTransform->rotation.y += LOOK_SPEED;
	else if (sign < -LOOK_SPEED)
		bossTransform->rotation.y -= LOOK_SPEED;
	else
		bossTransform->rotation.y = direction;
}

void Boss::PlayerHit()
{
	ID::IDType attackID = pState->GetState<PlayerStateBase>()->GetID();
	float damage = 0;;
	if (pState->GetState<PlayerAttackStateBase>() != nullptr) {
		damage = pState->GetState<PlayerAttackStateBase>()->GetHitDamage();
	}

	EnemyDamage::EnemyDamageInfo dInfo;
	EnemyBlowAway::EnemyBlowAwayInfo bInfo;
	float random[3] = {};
	//エフェクトの位置をランダムに派生
	for (float& r : random) {
		r = (float)GetRand(80) - 40.0f;
	}
	//プレイヤーの攻撃の種類によってダメージや吹っ飛び方の種類を変える
	float angleRan = 0.0f;
	bool lastAttack = false;
	bool lastBeforeAttack = false;
	switch (attackID)
	{
	case ID::P_ANIM_ATTACK1:
		//enemyBaseComponent.state->NowChangeState(ID::E_DAMAGE);
		dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 1500.0f), VECTOR3(100, 100, 100), 0.15f, 0.8f);
		enemyBaseComponent.control->ControlVibrationStartFrame(10, 10);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, -40.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);

		break;
	case ID::P_ANIM_ATTACK2:
		//enemyBaseComponent.state->NowChangeState(ID::E_DAMAGE);
		dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 1500.0f), VECTOR3(100, 100, 100), 0.25f, 0.8f);
		enemyBaseComponent.control->ControlVibrationStartFrame(40, 30);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
		//enemyBaseComponent.effect->CreateEffekseer(Transform(VZero, VOne * VECTOR3(0, 0, 70.0f * DegToR{
		// ad), VOne), obj, ID::PLAYER_SLASH_ATTACK, 1.0f);

		break;
	case ID::P_ANIM_ATTACK3:
		//enemyBaseComponent.state->NowChangeState(ID::E_DAMAGE);
		dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 1500.0f), VECTOR3(100, 100, 100), 0.35f, 0.8f);
		enemyBaseComponent.control->ControlVibrationStartFrame(10, 10);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, 80.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
		break;
	case ID::P_ANIM_ATTACK4:
		//enemyBaseComponent.state->NowChangeState(ID::E_DAMAGE);
		dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 3500.0f), VECTOR3(200, 200, 200), 0.45f, 0.8f);
		enemyBaseComponent.control->ControlVibrationStartFrame(50, 50);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, -120.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
		break;
	case ID::P_ANIM_JUST_AVOID_ATTACK1:
		//enemyBaseComponent.state->NowChangeState(ID::E_DAMAGE);
		obj->SetObjectTimeRate(1.0f);
		dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 2000.0f), VECTOR3(400, 400, 400), 0.05f, 0.8f);
		enemyBaseComponent.control->ControlVibrationStartFrame(30, 40);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
		//enemyBaseComponent.effect->CreateEffekseer(Transform(VZero, VOne * VECTOR3(0, 0, 50.0f * DegToRad), VOne), obj, ID::PLAYER_SLASH_ATTACK, 1.0f);
		break;
	case ID::P_ANIM_JUST_AVOID_ATTACK2:
		enemyBaseComponent.sound->RandamSe("swordHit00000", 7);
		if (loopNum == -1) {
			hitCounter = 0.23f;
			loopNum = 5;
		}
		else if (loopNum == 1) {
			hitCounter = 0.13f;
			loopNum--;
			lastBeforeAttack = true;
		}
		else if (loopNum == 0) {
			loopNum = -1;
			lastAttack = true;
		}
		else if (loopNum > 0) {
			hitCounter = 0.13f;
			loopNum--;
		}
		if (lastAttack) {
			//enemyBaseComponent.state->NowChangeState(ID::E_DAMAGE);
			dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 3500.0f), VECTOR3(100, 100, 100), 0.5f, 1.2f);
			enemyBaseComponent.control->ControlVibrationStartFrame(250, 60);
			enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE * 14.5f), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
			//angleRan = GetRand(360);
			enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(random[0] * 2.0f, 100, random[2]), VOne * VECTOR3(0, 0, 90 * DegToRad), VOne * 1.5f), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
		}
		else if (lastBeforeAttack) {
			dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(100, 100, 100), 0.5f, 0.5f);
			enemyBaseComponent.shaker->ShakeStart(VECTOR3(20, 20, 20), Shaker::HORIZONAL_SHAKE, true, 0.3f);
			enemyBaseComponent.control->ControlVibrationStartFrame(250, 60);
			enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
			angleRan = GetRand(360);
			enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(random[0] * 2.0f, 100, random[2]), VOne * VECTOR3(0, 0, 90.0f * DegToRad), VOne * 1.5f), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
		}
		else {
			//enemyBaseComponent.state->NowChangeState(ID::E_DAMAGE);
			dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(20, 20, 20), 0.2f, 0.02f);
			enemyBaseComponent.control->ControlVibrationStartFrame(450, 20);
			enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
			angleRan = GetRand(360);
			enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(random[0] * 2.0f, 100, random[2]), VOne * VECTOR3(0, 0, angleRan * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
			enemyBaseComponent.playerObj->Component()->GetComponent<Shaker>()->ShakeStart(VECTOR3(200, 200, 200), Shaker::HORIZONAL_SHAKE, true, 0.05f);
		}

		break;
	case ID::P_ANIM_JUST_AVOID_ATTACK3:
		//enemyBaseComponent.state->NowChangeState(ID::E_DAMAGE);
		dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 4000.0f), VECTOR3(200, 200, 200), 0.2f, 0.8f);
		enemyBaseComponent.control->ControlVibrationStartFrame(70, 40);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, -50.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
		break;
	case ID::P_ANIM_JUST_AVOID_ATTACK4:
		//enemyBaseComponent.state->NowChangeState(ID::E_DAMAGE);
		dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 7000.0f), VECTOR3(200, 200, 200), 0.2f, 0.8f);
		enemyBaseComponent.control->ControlVibrationStartFrame(40, 40);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, 80.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
		break;
	case ID::P_ANIM_JUST_AVOID_ATTACK5:
		//enemyBaseComponent.state->NowChangeState(ID::E_BLOWAWAY);
		bInfo = EnemyBlowAway::EnemyBlowAwayInfo(4000.0f, 3000.0f, VECTOR3(15.0f, 15.0f, 15.0f), VECTOR3(0.3f, 0.3f, 0.3f), 2000.0f, 20.0f);
		enemyBaseComponent.control->ControlVibrationStartFrame(100, 60);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::SPECIAL_HIT_EFFECT, 5.0f);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, 0.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
		break;
	default:
		Debug::DebugLog("PlayerNoAttackState");
	}
	EnemyDamageMove(dInfo);
	hp -= damage;
	//ダメージか吹っ飛ばしの状態になっていたらダメージのパラメーターをいれる。
	std::shared_ptr<EnemyDamage> eD = enemyBaseComponent.state->GetState<EnemyDamage>();
	std::shared_ptr <EnemyBlowAway> eB = enemyBaseComponent.state->GetState<EnemyBlowAway>();
	enemyBaseComponent.sound->RandamSe("E_DamageV", 2);
	enemyBaseComponent.color->setRGB(Color::Rgb(255, 0, 0, 255));
	damageFlash = 0.5f;
	if (eD != nullptr) {
		eD->DamageInit(dInfo);
	}
	else if (eB != nullptr) {
		eB->EnemyBlowAwayInfoSet(bInfo);
	}
}
