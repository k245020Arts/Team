#include "playerAttack3.h"
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

PlayerAttack3::PlayerAttack3()
{
	string = Function::GetClassNameC<PlayerAttack3>();
	id = ID::P_ANIM_ATTACK3;
	animId = ID::P_ANIM_ATTACK3;
	collTrans = Transform(VECTOR3(0, 80, 100), VZero, VECTOR3(300, 0, 0));
	nextAttackID = ID::P_ANIM_ATTACK4; 
	frontSpeed = 1000.0f;
	hitDamage = 15.0f;
}

PlayerAttack3::~PlayerAttack3()
{
}

void PlayerAttack3::Update()
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
			p->playerCom.anim->SetPlaySpeed(3.5f);
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

void PlayerAttack3::Draw()
{
}

void PlayerAttack3::Start()
{
	PlayerStateBase::Start();
	PlayerAttackStateBase::Start();
}

void PlayerAttack3::Finish()
{
	Player* p = GetBase<Player>();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.anim->AnimEventReset();
	PlayerAttackStateBase::Finish();
}
