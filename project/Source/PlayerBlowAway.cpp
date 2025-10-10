#include "PlayerBlowAway.h"
#include "Animator.h"
#include "playerStateManager.h"
#include "color.h"
#include "Physics.h"
#include "player.h"
#include "Shaker.h"
#include "function.h"
#include "SoundManager.h"
#include "camera.h"

PlayerBlowAway::PlayerBlowAway()
{
	string = string = Function::GetClassNameC<PlayerBlowAway>();;
	animId = ID::P_DAMAGE;
	id= ID::P_BLOWAWAY;
}

PlayerBlowAway::~PlayerBlowAway()
{
}

void PlayerBlowAway::Update()
{
	Player* p = GetBase<Player>();
	//落下したらカメラを縦揺れ
	if (p->playerCom.player->GetPlayerTransform()->position.y <= 0.0f) {
		if (p->playerCom.anim->GetCurrentID() == ID::GetID(ID::P_FALL)) {
			p->playerCom.anim->Play(ID::P_GETUP);
			p->playerCom.shaker->ShakeFinish();
			p->playerCom.sound->PlaySe(Sound_ID::ENEMY_FALL);
			p->playerCom.camera->CameraShake(VECTOR3(100, 100, 0), Shaker::HORIZONAL_SHAKE, false, 0.5f);
		}
	}
	//ダメージ処理
	if (p->playerCom.anim->IsFinish()) {
		if (p->playerCom.anim->GetCurrentID() == ID::GetID(ID::P_DAMAGE)) {
			p->playerCom.anim->Play(ID::P_FALL);
			p->playerCom.anim->SetPlaySpeed(1.0f);
		}
		else if (p->playerCom.anim->GetCurrentID() == ID::GetID(ID::P_GETUP)) {
			p->playerCom.stateManager->ChangeState(ID::P_ANIM_RUN);
			
			
		}
	}
	//重力の加速
	p->playerCom.physics->AddGravity(VECTOR3(0, -20.0f, 0));
	if (p->playerCom.anim->GetCurrentID() == ID::GetID(ID::P_FALL)) {
		p->playerCom.shaker->AddShakePower(VECTOR3(0.1f, 0.1f, 0.1f));
	}
}

void PlayerBlowAway::Draw()
{
}

void PlayerBlowAway::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	eRotation = p->playerCom.hitObj->GetTransform()->rotation;
	p->playerCom.physics->SetVelocity(VECTOR3(0, 0, 3000) * MGetRotY(eRotation.y));
	p->playerCom.physics->AddVelocity(VECTOR3(0, 500, 0), false);
	p->playerCom.physics->SetGravity(VECTOR3(0, -3000.0f, 0));
	p->playerCom.player->GetPlayerTransform()->position.y += 10.0f;
	p->playerCom.anim->SetPlaySpeed(2.5f);
	p->playerCom.shaker->ShakeStart(VECTOR3(10, 10, 10), Shaker::MIX_SHAKE, false, -1.0f);
}

void PlayerBlowAway::Finish()
{
	Player* p = GetBase<Player>();
	p->playerCom.physics->SetGravity(VECTOR3(0, -6000.0f, 0));
	p->playerCom.shaker->ShakeFinish();
}
