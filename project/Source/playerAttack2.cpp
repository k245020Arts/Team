#include "playerAttack2.h"
#include "Animator.h"
#include "Physics.h"
#include "playerStateManager.h"
#include "controllerInputManager.h"
#include "player.h"
#include "function.h"
#include "inputManager.h"

PlayerAttack2::PlayerAttack2()
{
	string = Function::GetClassNameC<PlayerAttack2>();
	id = ID::P_ANIM_ATTACK2;
	animId = ID::P_ANIM_ATTACK2;
	collTrans = Transform(VECTOR3(0, 80, 100), VZero, VECTOR3(280, 0, 0));
	nextAttackID = ID::P_ANIM_ATTACK3;
	frontSpeed = 3000.0f;
	hitDamage = 4.0f;
}

PlayerAttack2::~PlayerAttack2()
{
}

void PlayerAttack2::Update()
{
	Player* p = GetBase<Player>();
	AttackCollsion();
	PlayerAttackStateBase::Update();
	if (!noStateChange) {
		//UŒ‚‚ÌŽž‚ÉŽŸ‚Ìƒ{ƒ^ƒ“‚ª‰Ÿ‚³‚ê‚Ä‚¢‚½‚çŽŸ‚ÌUŒ‚
		if (p->playerCom.InputManager->KeyInputDown("attack")) {
			nextAttack = true;
		}
		//UŒ‚‚ÌŽž‚É‰ñ”ðs“®‚ð‚¢‚ê‚½‚ç‰ñ”ðó‘Ô‚ÉˆÚs
		if (p->playerCom.InputManager->KeyInputDown("avoid")) {
			//p->playerCom.player->AvoidReady();
			nextAvoid = true;
			//noStateChange = true;
		}
		//“–‚½‚è”»’è‚ª‚ ‚éŠÔ‚ÍƒXƒs[ƒh‚ð‘‚­
		if (p->playerCom.anim->AnimEventCan()) {
			p->playerCom.anim->SetPlaySpeed(3.0f);
			beforeAttack = false;
			p->playerCom.physics->SetVelocity(VZero);
		}
		else {
			if(beforeAttack)
				p->playerCom.anim->SetPlaySpeed(1.0f);
			else {
				if (nextAttack) {
					//ŽŸ‚ÌUŒ‚‚És‚­‚Æ‚«‚ÌÝ’è
					runTimer = 0.1f;
					noStateChange = true;
					p->playerCom.anim->SetPlaySpeed(ATTACK_FINISH_ANIM_SPEED);
				}
				else {
					//UŒ‚‚ªI‚í‚é‚Æ‚«‚ÌÝ’è
					runTimer = 0.25f;
					noStateChange = true;
					p->playerCom.anim->SetPlaySpeed(ATTACK_FINISH_ANIM_SPEED);
				}
			}
			
		}
	}
}

void PlayerAttack2::Draw()
{
}

void PlayerAttack2::Start()
{
	PlayerStateBase::Start();
	PlayerAttackStateBase::Start();
}

void PlayerAttack2::Finish()
{
	Player* p = GetBase<Player>();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.anim->AnimEventReset();
}
