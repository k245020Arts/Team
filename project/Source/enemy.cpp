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
#include "EnemyBlowAway.h"
#include "EnemyRun.h"
#include "PlayerAttackStateBase.h"
#include "FadeTransitor.h"
#include "GameManager.h"
#include "EnemyDie.h"

namespace {
	const float HIT_EFFECT_TIME = 0.2f;
	const float HIT_EFFECT_SCALE_RATE = 0.1f;
	const float MAX_HP = 100;
}

Enemy::Enemy()
{
	damageFlash = 0.0f;
	enemyTransform = nullptr;
	pState = nullptr;
	eCom = EnemyInformation::EnemyComponent();
	debugId = 13;
	tag = Function::GetClassNameC<Enemy>();
	hp = MAX_HP;
	maxHp = hp;
}

Enemy::~Enemy()
{
	//delete eCom.state;
}

void Enemy::Update()
{
	//eCom.state->Update();
	//攻撃の時間が終わったら当たり判定は削除
	if (!eCom.anim->AnimEventCan()) {
		if (attackColl != nullptr) {
			DeleteColliderComponent();
			attackColl = nullptr;
		}
	}
	//敵のダメージフラッシュが終わったら元に戻す
	if (damageFlash > 0.0f) {
		damageFlash -= Time::DeltaTimeRate();
		if (damageFlash < 0.0f) {
			eCom.color->setRGB(Color::Rgb(0.0f, 0.0f, 0.0f, 255.0f));
		}
	}
	if (CheckHitKey(KEY_INPUT_G)) {
		enemyTransform->position = EnemyInformation::BASE_POS;
	}
	if (hp <= 0.0f && eCom.state->GetState<StateBase>()->GetID() == ID::E_ANIM_IDOL) {
		eCom.state->NowChangeState(ID::E_DIE);
		eCom.state->SetNoStateChange(true);
	}
}

void Enemy::Draw()
{
	eCom.state->Draw();
}

void Enemy::Start(Object3D* _obj) 
{
	eCom.state = obj->Component()->AddComponent<StateManager>();
	
	

	enemyTransform = obj->GetTransform();

	eCom.enemy = this;
	eCom.anim = obj->Component()->GetComponent<Animator>();
	eCom.physics = obj->Component()->GetComponent<Physics>();
	eCom.control = FindGameObject<ControllerInputManager>();
	eCom.color = obj->Component()->GetComponent<Color>();
	eCom.color->setRGB(Color::Rgb(0.0f, 0.0f, 0.0f, 255.0f));
	eCom.shaker = obj->Component()->GetComponent<Shaker>();
	eCom.effect = FindGameObject<EffectManager>();
	eCom.sound = FindGameObject<SoundManager>();

	eCom.camera =  FindGameObject<CameraManager>()->GetCamera()->Component()->GetComponent<Camera>();
	eCom.weapon = FindGameObject<WeaponManager>();
	
	attackColl = nullptr;
	collName = "e_attack";

	eCom.gameManager = FindGameObject<GameManager>();

	using namespace ID;
	//ステートをセット
	eCom.state->CreateState<EnemyIdol>(GetID(E_ANIM_IDOL));
	eCom.state->CreateState<EnemyAttack1>(GetID(E_ATTACK1));
	eCom.state->CreateState<EnemyDamage>(GetID(E_DAMAGE));
	eCom.state->CreateState<EnemyBlowAway>(GetID(E_BLOWAWAY));
	eCom.state->CreateState<EnemyRun>(GetID(E_RUN));
	eCom.state->CreateState<EnemyDie>(GetID(E_DIE));

	

	eCom.state->NodeDrawReady();

	eCom.state->SetComponent<Enemy>(this);

	eCom.state->StartState(E_ANIM_IDOL);
}

void Enemy::ImguiDraw()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth;
	bool open = ImGui::TreeNodeEx("PlayerState", flags);
	//stateを即座に変えるデバック用
	if (open) {
		//eCom.state->StateNodeDraw();
		if (ImGui::Button("e_attack1")) {
			eCom.state->ChangeState(ID::E_ATTACK1);
		}
		if (ImGui::Button("e_BlowAway")) {
			eCom.state->ChangeState(ID::E_BLOWAWAY);
		}
		ImGui::TreePop();
	}
	if (ImGui::Button("enemyShake")) {
		eCom.shaker->ShakeStart(VECTOR3(100,100,100), Shaker::MIX_SHAKE, false, -1.2f);
	}
	if (ImGui::Button("stopShakeEnemy")){
		eCom.shaker->ShakeFinish();
	}
}

void Enemy::PlayerHit()
{
	ID::IDType attackID = pState->GetState<PlayerStateBase>()->GetID();
	float damage = pState->GetState<PlayerAttackStateBase>()->GetHitDamage();
	EnemyDamage::EnemyDamageInfo dInfo;
	EnemyBlowAway::EnemyBlowAwayInfo bInfo;
	float random[3] = {};
	//エフェクトの位置をランダムに派生
	for (float& r : random) {
		r = (float)GetRand(80) - 40.0f;
	}
	//プレイヤーの攻撃の種類によってダメージや吹っ飛び方の種類を変える
	switch (attackID)
	{
	case ID::P_ANIM_ATTACK1:
		//eCom.state->NowChangeState(ID::E_DAMAGE);
		dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 1500.0f), VECTOR3(100, 100, 100), 0.15f, 0.8f);
		eCom.control->ControlVibrationStartFrame(10, 10);
		eCom.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
		eCom.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, -40.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
		
		break;
	case ID::P_ANIM_ATTACK2:
		//eCom.state->NowChangeState(ID::E_DAMAGE);
		dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 1500.0f), VECTOR3(100, 100, 100), 0.25f, 0.8f);
		eCom.control->ControlVibrationStartFrame(40, 30);
		eCom.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
		//eCom.effect->CreateEffekseer(Transform(VZero, VOne * VECTOR3(0, 0, 70.0f * DegToRad), VOne), obj, ID::PLAYER_SLASH_ATTACK, 1.0f);
		break;
	case ID::P_ANIM_ATTACK3:
		//eCom.state->NowChangeState(ID::E_DAMAGE);
		dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 1500.0f), VECTOR3(100, 100, 100), 0.35f, 0.8f);
		eCom.control->ControlVibrationStartFrame(10, 10);
		eCom.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
		eCom.effect->CreateEffekseer(Transform(VOne * VECTOR3(0,100,0), VOne * VECTOR3(0, 0, 80.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
		break;
	case ID::P_ANIM_ATTACK4:
		eCom.state->NowChangeState(ID::E_DAMAGE);
		dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 3500.0f), VECTOR3(200, 200, 200), 0.45f, 0.8f);
		eCom.control->ControlVibrationStartFrame(50, 50);
		eCom.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
		eCom.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, -120.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
		break;
	case ID::P_ANIM_JUST_AVOID_ATTACK1:
		eCom.state->NowChangeState(ID::E_DAMAGE);
		obj->SetObjectTimeRate(1.0f);
		dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 2000.0f), VECTOR3(400, 400, 400), 0.05f, 0.8f);
		eCom.control->ControlVibrationStartFrame(30, 40);
		eCom.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
		//eCom.effect->CreateEffekseer(Transform(VZero, VOne * VECTOR3(0, 0, 50.0f * DegToRad), VOne), obj, ID::PLAYER_SLASH_ATTACK, 1.0f);
		break;
	case ID::P_ANIM_JUST_AVOID_ATTACK2:
		eCom.state->NowChangeState(ID::E_DAMAGE);
		dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 4000.0f), VECTOR3(400, 400, 400), 0.8f, 1.2f);
		eCom.control->ControlVibrationStartFrame(350, 60);
		eCom.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
		eCom.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, 70.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
		break;
	case ID::P_ANIM_JUST_AVOID_ATTACK3:
		//eCom.state->NowChangeState(ID::E_DAMAGE);
		dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 4000.0f), VECTOR3(200, 200, 200), 0.2f, 0.8f);
		eCom.control->ControlVibrationStartFrame(70, 40);
		eCom.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
		eCom.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, -50.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
		break;
	case ID::P_ANIM_JUST_AVOID_ATTACK4:
		eCom.state->NowChangeState(ID::E_DAMAGE);
		dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 7000.0f), VECTOR3(200, 200, 200), 0.2f, 0.8f);
		eCom.control->ControlVibrationStartFrame(40, 40);
		eCom.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, HIT_EFFECT_TIME);
		eCom.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, 80.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
		break;
	case ID::P_ANIM_JUST_AVOID_ATTACK5:
		eCom.state->NowChangeState(ID::E_BLOWAWAY);
		bInfo = EnemyBlowAway::EnemyBlowAwayInfo(4000.0f, 3000.0f, VECTOR3(15.0f, 15.0f, 15.0f), VECTOR3(0.3f, 0.3f, 0.3f), 2000.0f, 20.0f);
		eCom.control->ControlVibrationStartFrame(100, 60);
		eCom.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * HIT_EFFECT_SCALE_RATE), obj, Effect_ID::SPECIAL_HIT_EFFECT, 5.0f);
		eCom.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, 0.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
		break;
	default:
		Debug::DebugLog("PlayerNoAttackState");
	}
	EnemyDamageMove(dInfo);
	hp -= damage;
	//ダメージか吹っ飛ばしの状態になっていたらダメージのパラメーターをいれる。
	std::shared_ptr<EnemyDamage> eD =  eCom.state->GetState<EnemyDamage>();
	std::shared_ptr <EnemyBlowAway> eB =  eCom.state->GetState<EnemyBlowAway>();
	eCom.color->setRGB(Color::Rgb(255, 0, 0, 255));
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
	return eCom.shaker->IsShakeing();
}

bool Enemy::PlayerPointerSet(BaseObject* _obj)
{
	eCom.playerObj = _obj;
	pState = eCom.playerObj->Component()->GetComponent<Player>()->GetPlayerStateManager();
	return true;
}

void Enemy::EnemyDamageMove(EnemyDamage::EnemyDamageInfo _info)
{
	//敵がダメージを受けた時の吹っ飛ばし量の設定
	eCom.physics->SetVelocity(_info.speed * MGetRotY(eCom.playerObj->GetTransform()->rotation.y));
	//eCom.shaker->ShakeStart(_info.shakePower, Shaker::HORIZONAL_SHAKE, true, _info.shakeTime);
}

void Enemy::DrawTrail() {

	//剣の軌跡を作成
	eCom.weapon->CreateTrailEnemy(VECTOR3(0, 0, 0), VECTOR3(-70, 200, -230), 100.0f, 10.0f, 200.0f, 255.0f, 28, 0.5f);
}