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
	collTrans = Transform(VECTOR3(0, 100, 200), VZero, VECTOR3(300, 0, 0));
	nextAttackID = ID::P_ANIM_ATTACK1;
	frontSpeed = 10000.0f;
	hitDamage = 30.0f;
	defalutTrail = false;
	attack = false;
	count = 0;
	timer = 0.0f;
}

PlayerJustAvoidAttack2::~PlayerJustAvoidAttack2()
{
}

void PlayerJustAvoidAttack2::Update()
{
	Player* p = GetBase<Player>();
	AttackCollsion();
	PlayerAttackStateBase::Update();
	if (p->playerCom.anim->AnimEventCan()) {
		p->playerCom.player->DrawTrail(VECTOR3(0, 0, -100), VECTOR3(0, 0, -350), 0.0f, 0.0f, 255.0f, 150.0f, 28, 0.8f);
	}
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

	nextAttack = false;
	nextAvoid = false;
	p->playerCom.player->SetAvoidStart(false);
	p->playerCom.anim->AnimEventReset();

	noStateChange = false;
	if (p->playerCom.hitObj != nullptr) {
		targetTrans = *(p->playerCom.hitObj->GetTransform());
	}
	else {
		targetTrans = Transform();
	}

	dist = targetTrans.position - p->playerCom.player->GetPlayerTransform()->position;

	//Šp“xŒvŽZ
	angle = atan2f(dist.x, dist.z);
	easingCount = 0.0f;
	beforeAngle = p->playerCom.player->GetPlayerTransform()->rotation.y;
	firstColl = true;
	distSize = dist.Size();
	norm = dist.Normalize();
	beforeAttack = true;
	runTimer = 0.0f;
	if (dist.Size() >= 2500 && p->playerCom.hitObj != nullptr) {
		//‹——£‚ª‰“‚¢‚Æ‚à‚Æ‚à‚Æ‚ÌŠp“x‚Ô‚ñUŒ‚‚ÌˆÚ“®ˆ—‚ð‚¢‚ê‚é
		rotation = false;;
		p->playerCom.physics->SetVelocity(VECTOR3(0, 0, frontSpeed) * MGetRotY(beforeAngle));
	}
	else {
		//‹ß‚¢‚Æ“G‚Ì•ûŒü‚ÉŒü‚©‚Á‚ÄUŒ‚‚ÌˆÚ“®ˆ—‚ð‚¢‚ê‚é
		rotation = true;
		p->playerCom.physics->SetVelocity(VECTOR3(0, 0, frontSpeed) * MGetRotY(angle));
	}


	if (distSize <= ATTACK_MOVE_DIST) {
		p->playerCom.physics->SetVelocity(norm * distSize * 3.5f);
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
	p->playerCom.color->setRGB(Color::Rgb(255, 255, 255, 255));
	PlayerAttackStateBase::Finish();
}
