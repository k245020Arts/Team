#include "playerJustAvoidAttack2.h"
#include "Animator.h"
#include "Physics.h"
#include "playerStateManager.h"
#include "controllerInputManager.h"
#include "player.h"
#include "function.h"
#include "Shaker.h"
#include "EffectManager.h"
#include "color.h"
#include "inputManager.h"

PlayerJustAvoidAttack2::PlayerJustAvoidAttack2()
{
	string = Function::GetClassNameC<PlayerJustAvoidAttack2>();
	id =  ID::P_ANIM_JUST_AVOID_ATTACK2;
	animId = ID::P_ANIM_JUST_AVOID_ATTACK2;
	collTrans = Transform(VECTOR3(0, 100, 200), VZero, VECTOR3(200, 0, 0));
	nextAttackID = ID::P_ANIM_ATTACK1;
	frontSpeed = 10000.0f;
	hitDamage = 5.0f;
}

PlayerJustAvoidAttack2::~PlayerJustAvoidAttack2()
{
}

void PlayerJustAvoidAttack2::Update()
{
	Player* p = GetBase<Player>();
	AttackCollsion();
	PlayerAttackStateBase::Update();
	if (!noStateChange) {
		if (distSize <= ATTACK_MOVE_DIST) {
			EnemyRotation();
		}
		if (p->playerCom.InputManager->KeyInputDown("avoid")) {
			p->playerCom.player->AvoidReady();
			noStateChange = true;
		}
		if (p->playerCom.InputManager->KeyInputDown("attack")) {
			nextAttack = true;
		}
		dist = targetTrans.position - p->playerCom.player->GetPlayerTransform()->position;
		if (dist.Size() <= DISTANCE_MOVE && beforeAttack) {
			p->playerCom.anim->SetPlaySpeed(0.8f);
			p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA + VECTOR3(30000.0f, 30000.0f, 30000.0f));
		}
		if (p->playerCom.anim->AnimEventCan()) {	
			p->playerCom.anim->SetPlaySpeed(1.6f);
			beforeAttack = false;
			rotation = true;
			frontSpeed = 2000.0f;
			p->playerCom.physics->SetVelocity(VECTOR3(0, 0, frontSpeed) * MGetRotY(angle));
			
		}
		else {
			if (beforeAttack)
				p->playerCom.anim->SetPlaySpeed(0.3f);
			else {
				p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA + VECTOR3(30000.0f, 30000.0f, 30000.0f));
				if (nextAttack) {
					runTimer = 1.0f;
					noStateChange = true;
					p->playerCom.anim->SetPlaySpeed(ATTACK_FINISH_ANIM_SPEED);
				}
				else {
					runTimer = 1.0f;
					noStateChange = true;
					p->playerCom.anim->SetPlaySpeed(ATTACK_FINISH_ANIM_SPEED);
					p->playerCom.color->setRGB(Color::Rgb(255, 255, 255, 255));
				}
			}
		}
	}
}

void PlayerJustAvoidAttack2::Draw()
{
}

void PlayerJustAvoidAttack2::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	PlayerAttackStateBase::Start();
	if (p->playerCom.targetObj != nullptr) {
		targetTrans = *(p->playerCom.hitObj->GetTransform());
	}
	else {
		targetTrans = Transform();
	}
	if (distSize <= ATTACK_MOVE_DIST) {
		p->playerCom.physics->SetVelocity(norm * distSize * 4.5f);
	}
	p->playerCom.anim->SetPlaySpeed(0.01f);
	firstColl = true;
	attack = false;
	p->playerCom.shaker->ShakeStart(VECTOR3(20.0f, 10.0f, 10.0f), Shaker::HORIZONAL_SHAKE, false, 1.0f);
	p->playerCom.controller->ControlVibrationStartFrame(100, 20);
	/*p->playerCom.anim->SetPlaySpeed(0.1f);
	timer = 0.5f;
	p->playerCom.shaker->ShakeStart(VECTOR3(30.0f, 10.0f, 10.0f), Shaker::HORIZONAL_SHAKE, false, 0.4f);*/
	//p->playerCom.controller->ControlVibrationStartFrame(100, 20);
}

void PlayerJustAvoidAttack2::Finish()
{
	Player* p = GetBase<Player>();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA);
	p->playerCom.anim->AnimEventReset();
}
