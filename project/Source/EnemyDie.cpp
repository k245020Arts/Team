#include "EnemyDie.h"
#include "Animator.h"
#include "GameManager.h"
#include "camera.h"
#include "enemy.h"

EnemyDie::EnemyDie()
{
	animId = ID::E_DIE;
	id = ID::E_DIE;
	string = Function::GetClassNameC<EnemyDie>();
}

EnemyDie::~EnemyDie()
{
}

void EnemyDie::Update()
{
	Enemy* e = GetBase<Enemy>();
	if (e->eCom.anim->IsFinish()) {
		e->eCom.gameManager->ChangeState("LOSE");

	}
	else {
		if (slowTime <= 0.0f) {
			e->eCom.enemy->GetEnemyObj()->SetObjectTimeRate(1.0f);
			e->eCom.camera->CameraShakeStop();
			e->eCom.shaker->ShakeFinish();
		}
		else {
			slowTime -= Time::DeltaTime();
		}
	}
}

void EnemyDie::Draw()
{
}

void EnemyDie::Start()
{
	Enemy* e = GetBase<Enemy>();
	EnemyStateBase::Start();
	e->eCom.enemy->GetEnemyObj()->SetObjectTimeRate(0.05f);
	slowTime = 1.0f;
	e->eCom.camera->CameraShake(VOne * 10.0f, Shaker::MIX_SHAKE, false, -1.0f);
	e->eCom.shaker->ShakeStart(VOne * 10.0f, Shaker::MIX_SHAKE, false, -1.0f);
}

void EnemyDie::Finish()
{
}
