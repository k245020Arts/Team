#include "playerJustAvoidAttack4.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../Component/Physics/Physics.h"
#include "../playerStateManager.h"
#include "../../../Common/InputManager/ControllerInputManager.h"
#include "../../player.h"
#include "../../../Common/function.h"
#include "../../../Component/Shaker/Shaker.h"
#include "../../../Common/Effect/EffectManager.h"
#include "../../../Component/Color/Color.h"
#include "../../../Common/InputManager/InputManager.h"
#include "../../../Camera/Camera.h"


PlayerJustAvoidAttack4::PlayerJustAvoidAttack4()
{
	string = Function::GetClassNameC<PlayerJustAvoidAttack4>();
	id = ID::P_ANIM_JUST_AVOID_ATTACK4;
	animId = ID::P_ANIM_JUST_AVOID_ATTACK2;
	collTrans = Transform(VECTOR3(0, 100, 200), VZero, VECTOR3(200, 0, 0));
	nextAttackID = ID::P_ANIM_JUST_AVOID_ATTACK5;
	frontSpeed = 1000.0f;
	hitDamage = 5.0f;
}

PlayerJustAvoidAttack4::~PlayerJustAvoidAttack4()
{
}

void PlayerJustAvoidAttack4::Update()
{
	Player* p = GetBase<Player>();
	AttackCollsion();
	PlayerAttackStateBase::Update();
	if (!noStateChange) {
		if (distSize <= ATTACK_MOVE_DIST) {
			EnemyRotation();
		}
		if (p->playerCom.InputManager->KeyInputDown("attack")) {
			nextAttack = true;
		}
		if (p->playerCom.InputManager->KeyInputDown("avoid")) {
			p->playerCom.player->AvoidReady();
			noStateChange = true;
		}
		dist = targetTrans.position - p->playerCom.player->GetPlayerTransform()->position;
		if (dist.Size() <= DISTANCE_MOVE) {
			p->playerCom.anim->SetPlaySpeed(0.8f);
			p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA + VECTOR3(15000.0f, 15000.0f, 15000.0f));
		}
		if (p->playerCom.anim->AnimEventCan()) {
			p->playerCom.anim->SetPlaySpeed(0.8f);
			beforeAttack = false;
		}
		else {
			if (beforeAttack)
				p->playerCom.anim->SetPlaySpeed(0.3f);
			else {
				if (nextAttack) {
					runTimer = 0.1f;
					noStateChange = true;
					p->playerCom.anim->SetPlaySpeed(ATTACK_FINISH_ANIM_SPEED);
				}
				else {
					runTimer = 0.1f;
					noStateChange = true;
					p->playerCom.anim->SetPlaySpeed(ATTACK_FINISH_ANIM_SPEED);
					p->playerCom.color->setRGB(Color::Rgb(255, 255, 255, 255));
					p->playerCom.camera->ChangeStateCamera(ID::C_FOLLOW);
				}
			}
		}
	}
}

void PlayerJustAvoidAttack4::Draw()
{
}

void PlayerJustAvoidAttack4::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	PlayerAttackStateBase::Start();
	if (distSize <= ATTACK_MOVE_DIST) {
		p->playerCom.physics->SetVelocity(norm * distSize * 2.5f);
	}
	p->playerCom.anim->SetPlaySpeed(0.1f);
	firstColl = true;
	p->playerCom.camera->ChangeStateCamera(ID::C_AVOID_ATTACK);
	nextAttack = true;
}

void PlayerJustAvoidAttack4::Finish()
{
	Player* p = GetBase<Player>();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA);
	p->playerCom.anim->AnimEventReset();
	PlayerAttackStateBase::Finish();
}
