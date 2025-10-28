#include "playerAttack1.h"
#include "Animator.h"
#include "playerStateManager.h"
#include <typeinfo>
#include "controllerInputManager.h"
#include "player.h"
#include "Physics.h"
#include "camera.h"
#include "Easing.h"
#include "Shaker.h"
#include "function.h"
#include "inputManager.h"

PlayerAttack1::PlayerAttack1()
{
	string = Function::GetClassNameC<PlayerAttack1>();
	id = ID::P_ANIM_ATTACK1;
	nextAttack = false;
	animId = ID::P_ANIM_ATTACK1;
	collTrans = Transform(VECTOR3(0, 80, 100), VZero, VECTOR3(280, 0, 0));
	nextAttackID = ID::P_ANIM_ATTACK2;
	frontSpeed = 2500.0f;
	time = 0.0f;
	hitDamage = 5.0f;
}

PlayerAttack1::~PlayerAttack1()
{
}

void PlayerAttack1::Update()
{
	Player* p = GetBase<Player>();
	AttackCollsion();
	PlayerAttackStateBase::Update();
	if (!noStateChange) {

		EnemyRotation();
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
			p->playerCom.anim->SetPlaySpeed(4.5f);
			beforeAttack = false;
			p->playerCom.physics->SetVelocity(VZero);
		}
		else {
			if (beforeAttack)
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
					runTimer = 0.05f;
					noStateChange = true;
					p->playerCom.anim->SetPlaySpeed(ATTACK_FINISH_ANIM_SPEED);
				}
			}
		}
	}
}

void PlayerAttack1::Draw()
{

}

void PlayerAttack1::Start()
{
	PlayerStateBase::Start();
	PlayerAttackStateBase::Start();
}

void PlayerAttack1::Finish()
{
	Player* p = GetBase<Player>();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.anim->AnimEventReset();
}
