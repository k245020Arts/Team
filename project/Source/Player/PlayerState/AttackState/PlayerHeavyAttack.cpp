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
	playerAttackData.collTrans = Transform(VECTOR3(0, 80, 100), VZero, VECTOR3(300, 0, 0));
	playerAttackData.normalAttackNextID = StateID::PLAYER_HEAVY_ATTACK_S;
	playerAttackData.frontSpeed = 4000.0f;
	//frontSpeed		= 0.0f;
	time = 0.0f;
	playerAttackData.hitDamage = 300.0f;
	chargeCount = 0.0f;
	defalutTrail = true;

	playerAttackData.state = StateID::PLAYER_HEAVY_ATTACK_S;
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
		if (InputManager::GetInstance()->KeyInputDown("attack")) {
			nextAttack = true;
		}
		//攻撃の時に回避行動をいれたら回避状態に移行
		if (InputManager::GetInstance()->KeyInputDown("avoid")) {
			//p->playerCom.player->AvoidReady();
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
	Player* p = GetBase<Player>();
	chargeCount = 1.0f;
	p->playerCom.camera->ChangeStateCamera(StateID::PLAYER_HEAVY_CHARGE_CAMERA_S);
	switch (p->attackLevel)
	{
	case Player::LEVEL1:
		AgainTimerSet(0.2f, 0);
		playerAttackData.frontSpeed = 1000.0f;
		playerAttackData.hitDamage = 50.0f;
		break;
	case Player::LEVEL2:
		AgainTimerSet(0.3f, 2);
		playerAttackData.frontSpeed = 2000.0f;
		playerAttackData.hitDamage = 150.0f;
		break;
	case Player::LEVEL3:
		AgainTimerSet(0.1f, 6);
		playerAttackData.frontSpeed = 4000.0f;
		playerAttackData.hitDamage = 300.0f;
		break;
	default:
		my_error_assert("チャージ攻撃のレベルがセットされていません");
		break;
	}
	
}

void PlayerHeavyAttack::Finish()
{
	Player* p = GetBase<Player>();
	PlayerAttackStateBase::Finish();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.anim->AnimEventReset();
	p->playerCom.camera->ChangeStateCamera(StateID::FREE_CAMERA_S);

}
