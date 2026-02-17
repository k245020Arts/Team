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
#include "../../../Common/Effect/EffectManager.h"
#include "../../../Common/Sound/SoundManager.h"

PlayerHeavyCharge::PlayerHeavyCharge()
{
	string			= Function::GetClassNameC<PlayerHeavyCharge>();
	nextAttack		= false;
	animId			= ID::P_HEAVY_CHARGE;
	collTrans		= Transform(VECTOR3(0, 80, 100), VZero, VECTOR3(300, 0, 0));
	nextAttackID	= StateID::PLAYER_HEAVY_ATTACK_S;
	frontSpeed		= 5500.0f;
	time			= 0.0f;
	hitDamage		= 30.0f;
	chargeCount		= 0.0f;
	defalutTrail	= false;
	chargeFinish	= false;
	baseFrequ		= 0;
	maxCharge		= false;
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
			p->attackLevel = Player::LEVEL3;
			p->playerCom.shaker->SetShakePower(VECTOR3(20, 20, 20));
			p->playerCom.controller->ControlVibrationStartFrame(150, -1);
			
			if (!p->playerCom.effect->IsPlayIng(Effect_ID::PLAYER_CHARGE_FINAL)) {
				p->playerCom.effect->CreateEffekseer(Transform(), obj, Effect_ID::PLAYER_CHARGE_FINAL, 1.0f);
			}
			baseFrequ = 90000;
			if (!p->playerCom.sound->CheckSe(Sound_ID::PLAYER_MAX_CHARGE)) {
				p->playerCom.sound->PlayRamdomChangeFrequencySe(Sound_ID::PLAYER_MAX_CHARGE, 0, baseFrequ);
			}
			if (!maxCharge) {
				maxCharge = true;
				p->playerCom.sound->PlaySe(Sound_ID::PLAYER_CHARGE_END);
				p->playerCom.effect->CreateEffekseer(Transform(VECTOR3(0,100,0),VZero,VOne), obj, Effect_ID::PLAYER_CHARGE_END, 1.0f);
			}
		}
		else if (chargeCount <= 1.0f) {
			p->attackLevel = Player::LEVEL2;
			p->playerCom.shaker->SetShakePower(VECTOR3(10, 10, 10));
			p->playerCom.controller->ControlVibrationStartFrame(100, -1);
			if (!p->playerCom.effect->IsPlayIng(Effect_ID::PLAYER_CHARGE_SECOND)) {
				p->playerCom.effect->CreateEffekseer(Transform(), obj, Effect_ID::PLAYER_CHARGE_SECOND, 1.0f);
			}
			baseFrequ = 80000;
			if (!p->playerCom.sound->CheckSe(Sound_ID::PLAYER_CHARGE)) {
				p->playerCom.sound->PlayRamdomChangeFrequencySe(Sound_ID::PLAYER_CHARGE, 0, baseFrequ);
			}
		}
		
	}
	else {
		chargeFinish = true;
	}
	if (chargeFinish) {
		p->HeavyAttackChangeParam(p->attackLevel);
		p->playerCom.stateManager->ChangeState(nextAttackID);
		p->playerCom.shaker->ShakeFinish();
		p->playerCom.controller->StopControlVibrationStartFrame();
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
	chargeCount = 2.0f;
	p->playerCom.camera->ChangeStateCamera(StateID::PLAYER_HEAVY_CHARGE_CAMERA_S);
	p->charge = true;
	//p->playerCom.sound->PlaySe(Sound_ID::PLAYER_CHARGE);
	baseFrequ = 50000;
	chargeFinish = false;
	p->playerCom.shaker->ShakeStart(VECTOR3(5, 5, 5), Shaker::MIX_SHAKE, false, -1);
	maxCharge = false;
	p->attackLevel = Player::LEVEL1;
	p->playerCom.controller->ControlVibrationStartFrame(50, -1);
	p->playerCom.effect->CreateEffekseer(Transform(), obj, Effect_ID::PLAYER_CHARGE_FIRST, 1.0f);
}

void PlayerHeavyCharge::Finish()
{
	Player* p = GetBase<Player>();
	PlayerAttackStateBase::Finish();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.anim->AnimEventReset();
	p->charge = false;
	p->playerCom.camera->ChangeStateCamera(StateID::FREE_CAMERA_S);
	p->playerCom.shaker->ShakeFinish();
	p->playerCom.controller->StopControlVibrationStartFrame();
}
