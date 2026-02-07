#include "PlayerHeavyAttack.h"
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

PlayerHeavyAttack::PlayerHeavyAttack()
{
	string = Function::GetClassNameC<PlayerHeavyAttack>();
	//id = ID::P_ANIM_ATTACK1;
	nextAttack = false;
	animId = ID::P_HEAVY_ATTACK;
	collTrans = Transform(VECTOR3(0, 80, 100), VZero, VECTOR3(300, 0, 0));
	nextAttackID = StateID::PLAYER_HEAVY_ATTACK_S;
	frontSpeed = 4000.0f;
	//frontSpeed		= 0.0f;
	time = 0.0f;
	hitDamage = 300.0f;
	chargeCount = 0.0f;
	defalutTrail = true;
}

PlayerHeavyAttack::~PlayerHeavyAttack()
{
}

void PlayerHeavyAttack::Update()
{
	Player* p = GetBase<Player>();
	if (p->playerCom.anim->IsFinish()) {
		p->AvoidFinishState();
	}
	collsionCreate = false;
	AttackCollsion();
	PlayerAttackStateBase::Update();
	
	if (!noStateChange) {

		EnemyRotation();
		//攻撃の時に次のボタンが押されていたら次の攻撃
		if (p->playerCom.InputManager->KeyInputDown("attack")) {
			nextAttack = true;
		}
		//攻撃の時に回避行動をいれたら回避状態に移行
		if (p->playerCom.InputManager->KeyInputDown("avoid")) {
			//p->playerCom.player->AvoidReady();
			nextAvoid = true;
			//noStateChange = true;
		}
		//当たり判定がある間はスピードを早く
		if (p->playerCom.anim->AnimEventCan()) {
			if (beforeAttack) {
				AttackMoveStart();
			}
			//p->playerCom.anim->SetPlaySpeed(4.5f);
			beforeAttack = false;

		}
		else {
			if (beforeAttack)
				p->playerCom.anim->SetPlaySpeed(1.0f);
			else {
				if (nextAvoid) {
					runTimer = 0.05f;
					noStateChange = true;
					p->playerCom.anim->SetPlaySpeed(ATTACK_FINISH_ANIM_SPEED);
				}
			}
		}
	}
}

void PlayerHeavyAttack::Draw()
{
}

void PlayerHeavyAttack::Start()
{
	PlayerStateBase::Start();
	PlayerAttackStateBase::Start();
	chargeCount = 1.0f;
	AgainTimerSet(0.2f, 5);
}

void PlayerHeavyAttack::Finish()
{
	Player* p = GetBase<Player>();
	PlayerAttackStateBase::Finish();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.anim->AnimEventReset();
	p->playerCom.camera->ChangeStateCamera(StateID::FREE_CAMERA_S);

}
