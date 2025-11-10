#include "playerJustAvoidAttack5.h"
#include "Animator.h"
#include "Physics.h"
#include "playerStateManager.h"
#include "controllerInputManager.h"
#include "player.h"
#include "function.h"
#include "Shaker.h"
#include "EffectManager.h"
#include "SoundManager.h"
#include "color.h"
#include "camera.h"
#include "inputManager.h"

PlayerJustAvoidAttack5::PlayerJustAvoidAttack5()
{
	string = Function::GetClassNameC<PlayerJustAvoidAttack5>();
	id = ID::P_ANIM_JUST_AVOID_ATTACK5;
	animId = ID::P_ANIM_JUST_AVOID_ATTACK4;
	collTrans = Transform(VECTOR3(0, 100, 200), VZero, VECTOR3(200, 0, 0));
	attack = false;
	startTimer = 0.0f;
	timer = 0.0f;
	oneEffect = false;
	hitDamage = 10.0f;
	//frontSpeed = 2000.0f;
}

PlayerJustAvoidAttack5::~PlayerJustAvoidAttack5()
{
}

void PlayerJustAvoidAttack5::Update()
{
	Player* p = GetBase<Player>();
	AttackCollsion();
	PlayerAttackStateBase::Update();
	if (!noStateChange) {
		if (p->playerCom.InputManager->KeyInputDown("avoid")) {
			p->playerCom.player->AvoidReady();
			noStateChange = true;
		}
		timer -= Time::DeltaTimeRate();
		if (timer <= 0.0f) {		
			if (attack) {
				//攻撃可能なら攻撃
				p->playerCom.anim->SetPlaySpeed(1.0f);
				p->playerCom.sound->PlaySe(Sound_ID::V_P_JUST_AVOID);
			}
			else {
				//攻撃不可能なら落下していく
				p->playerCom.stateManager->ChangeState(ID::P_BLOWAWAY);
				p->playerCom.sound->PlaySe(Sound_ID::SOUND_ID::V_P_LOSE);
			}
			
		}
		else {
			//光が光るための条件、時間以外になるまでにボタンを押したら攻撃に入れる
			if (startTimer < p->playerCom.anim->GetCurrentFrame()) {
				if (oneEffect) {
					oneEffect = false;
					p->playerCom.effect->CreateEffekseer(Transform(VECTOR3(0, 100, 0), VECTOR3(0, 0, 0), VECTOR3(1, 1, 1)), p->playerCom.player->GetPlayerObj(), Effect_ID::EFFECT_ID::PLAYER_ATTACK, 1.0f);
					p->playerCom.sound->PlaySe(Sound_ID::SOUND_ID::PLAYER_ATTACK_BEFORE);
				}
				if (p->playerCom.InputManager->KeyInputDown("attack")) {
					attack = true;
				}
			}
		}
		//距離が一定以上に近づいたら止める
		dist = p->playerCom.targetObj->GetTransform()->position - p->playerCom.player->GetPlayerTransform()->position;
		if (dist.Size() <= DISTANCE_MOVE + 800.0f) {
			p->playerCom.physics->SetFirction(INTEREA + VECTOR3(30000.0f, 15000.0f, 30000.0f));
		}
		//攻撃できるようになったらカメラを変える
		if (p->playerCom.anim->AnimEventCan()) {
			p->playerCom.anim->SetPlaySpeed(1.0f);
			beforeAttack = false;
			p->playerCom.camera->ChangeStateCamera(ID::C_HIT);
		}
		else {
			if (beforeAttack) {
				//p->playerCom.anim->SetPlaySpeed(1.5f);
			}
			else {
				runTimer = 2.0f;
				noStateChange = true;
				p->playerCom.anim->SetPlaySpeed(0.05f);
			}
		}
	}
	
}

void PlayerJustAvoidAttack5::Draw()
{
}

void PlayerJustAvoidAttack5::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	PlayerAttackStateBase::Start();
	if (distSize <= ATTACK_MOVE_DIST) {
		p->playerCom.physics->SetVelocity(norm * distSize * 3.0f);
	}
	p->playerCom.anim->SetPlaySpeed(0.1f);
	timer = 1.0f;
	p->playerCom.shaker->ShakeStart(VECTOR3(30.0f, 10.0f, 10.0f), Shaker::HORIZONAL_SHAKE, false, 0.9f);
	//攻撃のための初速
	p->playerCom.physics->AddVelocity(VECTOR3(0.0f, 5100.0f, 0.0f), false);
	p->playerCom.physics->SetGravity(VECTOR3(0.0f, -8000.0f, 0.0f));
	p->playerCom.sound->PlaySe(Sound_ID::V_P_ATTACK4);
	//光が光始める時間のセット
	startTimer = p->playerCom.anim->EventStartTime(ID::P_ANIM_JUST_AVOID_ATTACK4) - 5.0f;
	oneEffect = true;
	attack = false;
	p->playerCom.physics->SetFirction(INTEREA);
}

void PlayerJustAvoidAttack5::Finish()
{
	Player* p = GetBase<Player>();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.physics->SetFirction(INTEREA);
	p->playerCom.physics->SetGravity(VECTOR3(0.0f, -6000.0f, 0.0f));
	p->playerCom.anim->AnimEventReset();
	p->playerCom.camera->ChangeStateCamera(ID::C_FOLLOW);
	p->playerCom.color->setRGB(Color::Rgb(255, 255, 255, 255));
	PlayerAttackStateBase::Finish();
}
