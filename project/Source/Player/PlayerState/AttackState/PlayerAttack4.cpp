#include "playerAttack4.h"
#include "../../../Component/Animator/Animator.h"
#include "../playerStateManager.h"
#include "../../../Common/InputManager/ControllerInputManager.h"
#include "../../Player.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Camera/Camera.h"
#include "../../../Common/Easing.h"
#include "../../../Component/Shaker/Shaker.h"
#include "../../../Common/Function.h"
#include "../../../Common/InputManager/InputManager.h"

PlayerAttack4::PlayerAttack4()
{
	string			= Function::GetClassNameC<PlayerAttack4>();
	animId			= ID::P_ANIM_ATTACK4;
	collTrans		= Transform(VECTOR3(0, 80, 100), VZero, VECTOR3(300, 0, 0));
	frontSpeed		= 5000.0f;
	//frontSpeed		= 0.0f;
	hitDamage		= 300.0f;
	nextAttackID	= StateID::PLAYER_ATTACK5_S;
}

PlayerAttack4::~PlayerAttack4()
{
}

void PlayerAttack4::Update()
{
	Player* p = GetBase<Player>();
	collsionCreate = false;
	AttackCollsion();
	PlayerAttackStateBase::Update();
	if (!noStateChange) {
		EnemyRotation();
		if (p->playerCom.InputManager->KeyInputDown("avoid")) {
			//p->playerCom.player->AvoidReady();
			nextAvoid = true;
			//noStateChange = true;
		}
		if (p->playerCom.InputManager->KeyInputDown("attack")) {
			nextAttack = true;
		}
		timer -= Time::DeltaTimeRate();
		if (timer <= 0.0f) {
			p->playerCom.anim->SetPlaySpeed(1.0f);
		}
		if (p->playerCom.anim->AnimEventCan()) {
			if (beforeAttack) {
				AttackMoveStart();
			}
			beforeAttack = false;
			//p->playerCom.physics->SetVelocity(VZero);
			p->playerCom.physics->AddVelocity(VECTOR3(0, 20000, 0), true);
		}
		else {
			if (beforeAttack) {
				//p->playerCom.anim->SetPlaySpeed(1.5f);
			}
			else {
				runTimer = 0.4f;
				noStateChange = true;
				p->playerCom.anim->SetPlaySpeed(ATTACK_FINISH_ANIM_SPEED);
			}
		}
	}
}

void PlayerAttack4::Draw()
{
}

void PlayerAttack4::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	PlayerAttackStateBase::Start();
	timer = 0.01f;
	//p->playerCom.anim->SetPlaySpeed(0.1f);
	p->playerCom.physics->SetGravity(VZero);
	//UŒ‚‚Ì‘O‚É‚½‚ß‚ð‚¢‚ê‚é‚½‚ß‚Ìshake
	//p->playerCom.shaker->ShakeStart(VECTOR3(20.0f, 10.0f, 10.0f), Shaker::HORIZONAL_SHAKE, false, 0.4f);
	//p->playerCom.controller->ControlVibrationStartFrame(100, 20);
	p->playerCom.physics->SetGravity(PlayerInformation::BASE_GRAVITY);
	AgainTimerSet(0.3f, 3);
}

void PlayerAttack4::Finish()
{
	Player* p = GetBase<Player>();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.anim->AnimEventReset();
	PlayerAttackStateBase::Finish();
}
