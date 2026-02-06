#include "PlayerHeavyCharge.h"
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

PlayerHeavyCharge::PlayerHeavyCharge()
{
	string = Function::GetClassNameC<PlayerHeavyCharge>();
	//id = ID::P_ANIM_ATTACK1;
	nextAttack = false;
	animId = ID::P_HEAVY_CHARGE;
	collTrans = Transform(VECTOR3(0, 80, 100), VZero, VECTOR3(300, 0, 0));
	nextAttackID = StateID::PLAYER_HEAVY_ATTACK_S;
	frontSpeed = 5500.0f;
	//frontSpeed		= 0.0f;
	time = 0.0f;
	hitDamage = 30.0f;
	chargeCount = 0.0f;
	defalutTrail = false;
}

PlayerHeavyCharge::~PlayerHeavyCharge()
{
}

void PlayerHeavyCharge::Update()
{
	Player* p = GetBase<Player>();
	//AttackCollsion();
	PlayerAttackStateBase::Update();
	if (p->playerCom.InputManager->KeyInput("heavyAttack")) {
		chargeCount -= Time::DeltaTimeRate();
		if (chargeCount <= 0.0f) {
			p->playerCom.stateManager->ChangeState(nextAttackID);
		}
	}
	else {
		if (chargeCount >= 0.7f) {
			p->playerCom.stateManager->ChangeState(nextAttackID);
		}
		else if (chargeCount >= 0.2f) {
			p->playerCom.stateManager->ChangeState(nextAttackID);
		}
		else {
			p->playerCom.stateManager->ChangeState(nextAttackID);
		}
	}
}

void PlayerHeavyCharge::Draw()
{
}

void PlayerHeavyCharge::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	PlayerAttackStateBase::Start();
	chargeCount = 1.0f;
	p->playerCom.camera->ChangeStateCamera(StateID::PLAYER_HEAVY_CHARGE_CAMERA_S);
	p->charge = true;
}

void PlayerHeavyCharge::Finish()
{
	Player* p = GetBase<Player>();
	PlayerAttackStateBase::Finish();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.anim->AnimEventReset();
	p->charge = false;
}
