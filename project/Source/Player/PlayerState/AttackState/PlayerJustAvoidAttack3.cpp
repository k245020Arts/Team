#include "playerJustAvoidAttack3.h"
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
#include "../../../Common/Sound/SoundManager.h"


PlayerJustAvoidAttack3::PlayerJustAvoidAttack3()
{
	string = Function::GetClassNameC<PlayerJustAvoidAttack3>();
	//id = ID::P_ANIM_JUST_AVOID_ATTACK3;
	animId = ID::P_ANIM_JUST_AVOID_ATTACK3;
	collTrans = Transform(VECTOR3(0, 80, 100), VZero, VECTOR3(300, 0, 0));
	nextAttackID = StateID::PLAYER_ATTACK1_S;
	//frontSpeed = 500.0f;
	frontSpeed = 0.0f;
	hitDamage = 55.0f;
	defalutTrail = true;
	timer = 0.0f;
	count = 0;
}

PlayerJustAvoidAttack3::~PlayerJustAvoidAttack3()
{
}

void PlayerJustAvoidAttack3::Update()
{
	/*Player* p = GetBase<Player>();
	AttackCollsion();
	PlayerAttackStateBase::Update();
	if (!noStateChange) {
		if (distSize <= ATTACK_MOVE_DIST) {
			EnemyRotation();
		}
		if (count == 0) {
			if (!p->playerCom.anim->IsFinish()) {
				
			}
			else {
				if (nextAttack) {
					runTimer = 0.1f;
					noStateChange = true;
					p->playerCom.anim->SetPlaySpeed(ATTACK_FINISH_ANIM_SPEED);
				}
				else {
					runTimer = 0.2f;
					noStateChange = true;
					p->playerCom.color->setRGB(Color::Rgb(255, 255, 255, 255));
					p->playerCom.anim->SetPlaySpeed(ATTACK_FINISH_ANIM_SPEED);
				}
			}
		}
		else {
			if (p->playerCom.anim->GetCurrentFrame() >= 17.0f) {
				if (nextAttack) {
					Again();
					return;
				}
				else {
					runTimer = 0.2f;
					noStateChange = true;
					p->playerCom.color->setRGB(Color::Rgb(255, 255, 255, 255));
					p->playerCom.anim->SetPlaySpeed(ATTACK_FINISH_ANIM_SPEED);
				}
			}
		}
		
		if (p->playerCom.controller->GetIsButtonPutNow(XINPUT_BUTTON_B)) {
			nextAttack = true;
		}
		if (p->playerCom.InputManager->KeyInputDown("avoid")) {
			p->playerCom.player->AvoidReady();
			noStateChange = true;
		}
		if (count >= ATTACK_NUMMAX) {
			dist = targetTrans.position - p->playerCom.player->GetPlayerTransform()->position;
			if (dist.Size() <= DISTANCE_MOVE) {
				p->playerCom.anim->SetPlaySpeed(1.0f);
				p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA + VECTOR3(40000.0f, 40000.0f, 40000.0f));
			}
		}
	}*/

	Player* p = GetBase<Player>();
	AttackCollsion();
	PlayerAttackStateBase::Update();

	
	if (!noStateChange) {
		if (p->playerCom.InputManager->KeyInputDown("avoid")) {
			//p->playerCom.player->AvoidReady();
			nextAvoid = true;
			//noStateChange = true;
		}
		if (p->playerCom.InputManager->KeyInputDown("attack")) {
			nextAttack = true;
		}
		timer -= Time::DeltaTimeRate();
		if (p->playerCom.physics->GetGravity().y <= -20000.0f) {
			p->playerCom.anim->SetPlaySpeed(2.0f);
		}
		else {
			p->playerCom.physics->AddGravity(VECTOR3(0, -1500, 0));
		}
		//if (p->playerCom.anim->GetCurrentFrame() >= p->playerCom.anim->EventStartTime(animId) - 3.0f) {
			
		//}
		if (p->playerCom.anim->AnimEventCan()) {
			if (beforeAttack) {
				AttackMoveStart();
			}
			beforeAttack = false;
			//p->playerCom.physics->SetVelocity(VZero);
			//p->playerCom.anim->SetPlaySpeed(1.0f);
		}
		else {
			if (!beforeAttack) {
				p->playerCom.anim->SetFrame(p->playerCom.anim->EventStartTime(animId));
				firstColl = true;
			}
			/*if (p->playerCom.physics->GetGround()) {
				if (beforeAttack) {
					p->playerCom.anim->SetPlaySpeed(1.5f);
				}
				else {
					runTimer = 0.4f;
					noStateChange = true;
					p->playerCom.anim->SetPlaySpeed(ATTACK_FINISH_ANIM_SPEED);
				}
			}*/
		}
		if (p->playerCom.physics->GetGround()) {
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

void PlayerJustAvoidAttack3::Draw()
{
}

void PlayerJustAvoidAttack3::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	PlayerAttackStateBase::Start();
	/*if (distSize <= ATTACK_MOVE_DIST) {
		p->playerCom.physics->SetVelocity(norm * distSize * 2.0f);
	}*/
	p->playerCom.anim->SetPlaySpeed(0.1f);
	p->playerCom.physics->AddGravity(VECTOR3(0, 0, 0));
	p->playerCom.physics->SetGravity(VZero);
	//timer = 2.0f;
	//count = ATTACK_NUMMAX;
}

void PlayerJustAvoidAttack3::Finish()
{
	Player* p = GetBase<Player>();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA);
	p->playerCom.anim->AnimEventReset();
	p->playerCom.physics->SetGravity(PlayerInformation::BASE_GRAVITY);
	PlayerAttackStateBase::Finish();
}

void PlayerJustAvoidAttack3::Again()
{
	Player* p = GetBase<Player>();
	PlayerAttackStateBase::Start();
	/*if (distSize <= ATTACK_MOVE_DIST) {
		p->playerCom.physics->SetVelocity(norm * distSize * 5.0f);
	}*/
	firstColl = true;
	count--;
	
	if (count % 2 == 0) {
		p->playerCom.anim->Play(animId,0.01f);
		p->playerCom.anim->SetPlaySpeed(2.5f);
	}
	else {
		p->playerCom.anim->Play(ID::P_ANIM_ATTACK3,0.01f);
		p->playerCom.anim->SetPlaySpeed(2.0f);
	}
	nextAttack = false;
	p->playerCom.sound->RandamSe("P_AttackV", 4);
	p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA);
}