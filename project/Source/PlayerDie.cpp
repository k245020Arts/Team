#include "PlayerDie.h"
#include "Animator.h"
#include "GameManager.h"
#include "camera.h"
#include "player.h"
#include "SoundManager.h"

PlayerDie::PlayerDie()
{
	animId = ID::P_DIE;
	id = ID::P_DIE;
	string = Function::GetClassNameC<PlayerDie>();
	one = false;
}

PlayerDie::~PlayerDie()
{
}

void PlayerDie::Update()
{
	Player* p = GetBase<Player>();
	if (p->playerCom.anim->IsFinish()) {
		p->playerCom.gameManager->ChangeState("LOSE");
		if (one) {
			p->playerCom.sound->StopBGM(Sound_ID::PLAY_BGM);
			p->playerCom.sound->PlaySe(Sound_ID::LOSE);
			one = false;
		}
	
		
	}
	else {
		//éÄÇÒÇæèuä‘ÇÕÉXÉçÅ[çƒê∂ÇÇ≥ÇπÇÈ
		if (slowTime <= 0.0f) {
			Time::ChangeDeltaRate(1.0f);
			p->playerCom.camera->CameraShakeStop();
			p->playerCom.shaker->ShakeFinish();
		}
		else {
			slowTime -= Time::DeltaTime();
		}
	}
}

void PlayerDie::Draw()
{
}

void PlayerDie::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	Time::ChangeDeltaRate(0.1f);
	slowTime = 0.5f;
	p->playerCom.camera->CameraShake(VOne * 10.0f, Shaker::MIX_SHAKE, false, -1.0f);
	p->playerCom.shaker->ShakeStart(VOne * 10.0f, Shaker::MIX_SHAKE, false, -1.0f);
	one = true;
}

void PlayerDie::Finish()
{
}
