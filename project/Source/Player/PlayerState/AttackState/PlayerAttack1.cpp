#include "playerAttack1.h"
#include "../../../Component/Animator/Animator.h"
#include "../playerStateManager.h"
#include <typeinfo>
#include "../../../Common/InputManager/ControllerInputManager.h"
#include "../../Player.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Camera/Camera.h"
#include "../../../Common/Easing.h"
#include "../../../Component/Shaker/Shaker.h"
#include "../../../Common/Function.h"
#include "../../../Common/InputManager/InputManager.h"

PlayerAttack1::PlayerAttack1()
{
	string			= Function::GetClassNameC<PlayerAttack1>();
	//id = ID::P_ANIM_ATTACK1;
	nextAttack		= false;
	animId			= ID::P_ANIM_ATTACK1;
	playerAttackData.collTrans		= Transform(VECTOR3(0, 80, 100), VZero, VECTOR3(300, 0, 0));
	playerAttackData.normalAttackNextID	= StateID::PLAYER_ATTACK2_S;
	playerAttackData.frontSpeed		= 5500.0f;
	//frontSpeed		= 0.0f;
	time			= 0.0f;
	playerAttackData.hitDamage		= 150.0f;
	playerAttackData.state = StateID::PLAYER_ATTACK1_S;
}

PlayerAttack1::~PlayerAttack1()
{
}

void PlayerAttack1::Update()
{
	Player* p = GetBase<Player>();
	collsionCreate = false;
	
	AttackCollsion();
	PlayerAttackStateBase::Update();
	if (!noStateChange) {

		EnemyRotation();
		//UŒ‚‚ÌŽž‚ÉŽŸ‚Ìƒ{ƒ^ƒ“‚ª‰Ÿ‚³‚ê‚Ä‚¢‚½‚çŽŸ‚ÌUŒ‚
		if (InputManager::GetInstance()->KeyInputDown("attack")) {
			nextAttack = true;
		}
		//UŒ‚‚ÌŽž‚É‰ñ”ðs“®‚ð‚¢‚ê‚½‚ç‰ñ”ðó‘Ô‚ÉˆÚs
		if (InputManager::GetInstance()->KeyInputDown("avoid")) {
			//p->playerCom.player->AvoidReady();
			nextAvoid = true;
			//noStateChange = true;
		}
		//“–‚½‚è”»’è‚ª‚ ‚éŠÔ‚ÍƒXƒs[ƒh‚ð‘‚­
		if (p->playerCom.anim->AnimEventCan()) {
			if (beforeAttack) {
				AttackMoveStart();
			}
			p->playerCom.anim->SetPlaySpeed(4.5f);
			beforeAttack = false;
			
		}
		else {
			if (beforeAttack)
			p->playerCom.anim->SetPlaySpeed(1.0f);
			else {
				//p->playerCom.physics->SetVelocity(VZero);
				p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA + VECTOR3(40000.0f,0.0f,40000.0f));
				if (nextAttack) {
					//ŽŸ‚ÌUŒ‚‚És‚­‚Æ‚«‚ÌÝ’è
					runTimer		= 0.1f;
					noStateChange	= true;
					p->playerCom.anim->SetPlaySpeed(ATTACK_FINISH_ANIM_SPEED);
				}
				else {
					//UŒ‚‚ªI‚í‚é‚Æ‚«‚ÌÝ’è
					runTimer		= 0.05f;
					noStateChange	= true;
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
	Player* p = GetBase<Player>();
	//AgainTimerSet(0.0f, 0);
	//p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA + VECTOR3(40000.0f, 0.0f, 40000.0f));
}

void PlayerAttack1::Finish()
{
	Player* p = GetBase<Player>();
	PlayerAttackStateBase::Finish();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.anim->AnimEventReset();
}
