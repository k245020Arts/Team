#include "EnemyDie.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../GameManager/GameManager.h"
#include "../../../Camera/Camera.h"
#include "../enemy.h"

EnemyDie::EnemyDie()
{
	animId = ID::E_DIE;
	//id = ID::E_DIE;
	string = Function::GetClassNameC<EnemyDie>();
}

EnemyDie::~EnemyDie()
{
}

void EnemyDie::Update()
{
	Enemy* e = GetBase<Enemy>();
	if (e->enemyBaseComponent.anim->IsFinish()) {
		e->enemyBaseComponent.gameManager->ChangeState("LOSE");

	}
	else {
		if (slowTime <= 0.0f) {
			e->enemyBaseComponent.enemy->GetEnemyObj()->SetObjectTimeRate(1.0f);
			e->enemyBaseComponent.camera->CameraShakeStop();
			e->enemyBaseComponent.shaker->ShakeFinish();
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
	e->enemyBaseComponent.enemy->GetEnemyObj()->SetObjectTimeRate(0.05f);
	slowTime = 1.0f;
	e->enemyBaseComponent.camera->CameraShake(VOne * 10.0f, Shaker::MIX_SHAKE, false, -1.0f);
	e->enemyBaseComponent.shaker->ShakeStart(VOne * 10.0f, Shaker::MIX_SHAKE, false, -1.0f);
}

void EnemyDie::Finish()
{
}
