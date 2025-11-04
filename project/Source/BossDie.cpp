#include "BossDie.h"
#include "camera.h"
#include "Shaker.h"
#include "Boss.h"
#include "Animator.h"
#include "GameManager.h"

BossDie::BossDie()
{
	animId = ID::BOSS_DIE;
	id = ID::BOSS_DIE;
	string = Function::GetClassNameC<BossDie>();
}

BossDie::~BossDie()
{
}

void BossDie::Update()
{
	Boss* b = GetBase<Boss>();
	if (b->enemyBaseComponent.anim->IsFinish()) {
		b->enemyBaseComponent.gameManager->ChangeState("WIN");
	}
	else {
		//éÄÇÒÇæèuä‘ÇÕÉXÉçÅ[çƒê∂ÇÇ≥ÇπÇÈ
		if (slowTime <= 0.0f) {
			Time::ChangeDeltaRate(1.0f);
			b->enemyBaseComponent.camera->CameraShakeStop();
			b->enemyBaseComponent.shaker->ShakeFinish();
		}
		else {
			slowTime -= Time::DeltaTime();
		}
	}
}

void BossDie::Draw()
{
}

void BossDie::Start()
{
	Boss* b = GetBase<Boss>();
	EnemyStateBase::Start();
	Time::ChangeDeltaRate(0.1f);
	slowTime = 0.5f;
	b->enemyBaseComponent.camera->CameraShake(VOne * 10.0f, Shaker::MIX_SHAKE, false, -1.0f);
	b->enemyBaseComponent.shaker->ShakeStart(VOne * 10.0f, Shaker::MIX_SHAKE, false, -1.0f);
}

void BossDie::Finish()
{
}
