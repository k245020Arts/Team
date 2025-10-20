#include "enemy.h"
#include "transform.h"
#include "EnemyStateManager.h"
#include "Animator.h"
#include "Physics.h"
#include "controllerInputManager.h"
#include "sphereCollider.h"
#include "color.h"
#include "Shaker.h"
#include "../ImGui/imgui.h"
#include "player.h"
#include "playerStateManager.h"
#include "EnemyBlowAway.h"
#include "cameraManager.h"
#include "camera.h"
#include "EffectManager.h"
#include "SoundManager.h"
#include "weaponManager.h"
#include "EnemyIdol.h"
#include "enemyAttack1.h"
#include "EnemyRun.h"
#include "PlayerAttackStateBase.h"
#include "FadeTransitor.h"
#include "GameManager.h"
#include "EnemyDie.h"

namespace {
	
}

Enemy::Enemy()
{
	damageFlash = 0.0f;
	enemyTransform = nullptr;
	pState = nullptr;
	enemyBaseComponent = EnemyInformation::EnemyComponent();
	debugId = 13;
	tag = Function::GetClassNameC<Enemy>();
	hp = MAX_HP;
	maxHp = hp;
	
}

Enemy::~Enemy()
{
	//delete enemyBaseComponent.state;
}

void Enemy::Update()
{
	//enemyBaseComponent.state->Update();
	//攻撃の時間が終わったら当たり判定は削除
	EnemyBase::Update();
}

void Enemy::Draw()
{
	enemyBaseComponent.state->Draw();
}

void Enemy::Start(Object3D* _obj) 
{
	enemyBaseComponent.state = obj->Component()->AddComponent<StateManager>();
	
	

	enemyTransform = obj->GetTransform();

	enemyBaseComponent.enemy = this;
	enemyBaseComponent.anim = obj->Component()->GetComponent<Animator>();
	enemyBaseComponent.physics = obj->Component()->GetComponent<Physics>();
	enemyBaseComponent.control = FindGameObject<ControllerInputManager>();
	enemyBaseComponent.color = obj->Component()->GetComponent<Color>();
	enemyBaseComponent.color->setRGB(Color::Rgb(0.0f, 0.0f, 0.0f, 255.0f));
	enemyBaseComponent.shaker = obj->Component()->GetComponent<Shaker>();
	enemyBaseComponent.effect = FindGameObject<EffectManager>();
	enemyBaseComponent.sound = FindGameObject<SoundManager>();

	enemyBaseComponent.camera =  FindGameObject<CameraManager>()->GetCamera()->Component()->GetComponent<Camera>();
	enemyBaseComponent.weapon = FindGameObject<WeaponManager>();
	
	attackColl = nullptr;
	collName = "e_attack";

	enemyBaseComponent.gameManager = FindGameObject<GameManager>();

	using namespace ID;
	//ステートをセット
	enemyBaseComponent.state->CreateState<EnemyIdol>(GetID(E_ANIM_IDOL));
	enemyBaseComponent.state->CreateState<EnemyAttack1>(GetID(E_ATTACK1));
	enemyBaseComponent.state->CreateState<EnemyDamage>(GetID(E_DAMAGE));
	enemyBaseComponent.state->CreateState<EnemyBlowAway>(GetID(E_BLOWAWAY));
	enemyBaseComponent.state->CreateState<EnemyRun>(GetID(E_RUN));
	enemyBaseComponent.state->CreateState<EnemyDie>(GetID(E_DIE));

	

	enemyBaseComponent.state->NodeDrawReady();

	enemyBaseComponent.state->SetComponent<Enemy>(this);

	enemyBaseComponent.state->StartState(E_ANIM_IDOL);
}

void Enemy::ImguiDraw()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth;
	bool open = ImGui::TreeNodeEx("PlayerState", flags);
	//stateを即座に変えるデバック用
	if (open) {
		//enemyBaseComponent.state->StateNodeDraw();
		if (ImGui::Button("e_attack1")) {
			enemyBaseComponent.state->ChangeState(ID::E_ATTACK1);
		}
		if (ImGui::Button("e_BlowAway")) {
			enemyBaseComponent.state->ChangeState(ID::E_BLOWAWAY);
		}
		ImGui::TreePop();
	}
	if (ImGui::Button("enemyShake")) {
		enemyBaseComponent.shaker->ShakeStart(VECTOR3(100,100,100), Shaker::MIX_SHAKE, false, -1.2f);
	}
	if (ImGui::Button("stopShakeEnemy")){
		enemyBaseComponent.shaker->ShakeFinish();
	}
}

void Enemy::PlayerHit()
{
	/*if (pState->GetState<PlayerAttackStateBase>() == nullptr) {
		hitCounter = 0.0f;
		loopNum = -1;
		return;
	}*/
	/*if (!pState->GetState<PlayerAttackStateBase>()->IsAttack()) {
		hitCounter = 0.0f;
		loopNum = -1;
		return;
	}*/
	ID::IDType attackID = pState->GetState<PlayerStateBase>()->GetID();
	float damage = 0;;
	if (pState->GetState<PlayerAttackStateBase>() != nullptr) {
		damage = pState->GetState<PlayerAttackStateBase>()->GetHitDamage();
	}
	else {
		loopNum = -1;
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
		hit = true;
		break;
	case ID::P_ANIM_ATTACK2:
		//enemyBaseComponent.state->NowChangeState(ID::E_DAMAGE);
		dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 1500.0f), VECTOR3(100, 100, 100), 0.25f, 0.8f);
		enemyBaseComponent.control->ControlVibrationStartFrame(40, 30);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
		//enemyBaseComponent.effect->CreateEffekseer(Transform(VZero, VOne * VECTOR3(0, 0, 70.0f * DegToR{
		// ad), VOne), obj, ID::PLAYER_SLASH_ATTACK, 1.0f);
		hit = true;
		break;
	case ID::P_ANIM_ATTACK3:
		//enemyBaseComponent.state->NowChangeState(ID::E_DAMAGE);
		dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 1500.0f), VECTOR3(100, 100, 100), 0.35f, 0.8f);
		enemyBaseComponent.control->ControlVibrationStartFrame(10, 10);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(0,100,0), VOne * VECTOR3(0, 0, 80.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
		hit = true;
		break;
	case ID::P_ANIM_ATTACK4:
		enemyBaseComponent.state->NowChangeState(ID::E_DAMAGE);
		dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 3500.0f), VECTOR3(200, 200, 200), 0.45f, 0.8f);
		enemyBaseComponent.control->ControlVibrationStartFrame(50, 50);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, -120.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
		hit = true;
		break;
	case ID::P_ANIM_JUST_AVOID_ATTACK1:
		enemyBaseComponent.state->NowChangeState(ID::E_DAMAGE);
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
			enemyBaseComponent.state->NowChangeState(ID::E_DAMAGE);
			dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 3500.0f), VECTOR3(100, 100, 100), 0.5f, 1.2f);
			enemyBaseComponent.control->ControlVibrationStartFrame(250, 60);
			enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE * 14.5f), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
			//angleRan = GetRand(360);
			enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(random[0] * 2.0f, 100, random[2]), VOne * VECTOR3(0, 0, 90 * DegToRad), VOne * 1.5f), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
			hit = true;
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
		enemyBaseComponent.state->NowChangeState(ID::E_DAMAGE);
		dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 7000.0f), VECTOR3(200, 200, 200), 0.2f, 0.8f);
		enemyBaseComponent.control->ControlVibrationStartFrame(40, 40);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, 80.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
		break;
	case ID::P_ANIM_JUST_AVOID_ATTACK5:
		enemyBaseComponent.state->NowChangeState(ID::E_BLOWAWAY);
		bInfo = EnemyBlowAway::EnemyBlowAwayInfo(4000.0f, 3000.0f, VECTOR3(15.0f, 15.0f, 15.0f), VECTOR3(0.3f, 0.3f, 0.3f), 2000.0f, 20.0f);
		enemyBaseComponent.control->ControlVibrationStartFrame(100, 60);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::SPECIAL_HIT_EFFECT, 5.0f);
		enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, 0.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
		break;
	default:
		Debug::DebugLog("PlayerNoAttackState");
	}
	hit = true;
	EnemyDamageMove(dInfo);
	hp -= damage;
	//ダメージか吹っ飛ばしの状態になっていたらダメージのパラメーターをいれる。
	std::shared_ptr<EnemyDamage> eD =  enemyBaseComponent.state->GetState<EnemyDamage>();
	std::shared_ptr <EnemyBlowAway> eB =  enemyBaseComponent.state->GetState<EnemyBlowAway>();
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

bool Enemy::IsShake()
{
	return enemyBaseComponent.shaker->IsShakeing();
}

bool Enemy::PlayerPointerSet(BaseObject* _obj)
{
	enemyBaseComponent.playerObj = _obj;
	pState = enemyBaseComponent.playerObj->Component()->GetComponent<Player>()->GetPlayerStateManager();
	return true;
}