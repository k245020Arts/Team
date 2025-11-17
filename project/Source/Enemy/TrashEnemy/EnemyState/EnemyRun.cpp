#include "EnemyRun.h"
#include "../../../Common/Function.h"
#include "EnemyStateManager.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Component/Animator/Animator.h"
#include "../Enemy.h"

EnemyRun::EnemyRun()
{
	id = ID::E_RUN;
	animId = ID::E_RUN;
	string = Function::GetClassNameC<EnemyRun>();
}

EnemyRun::~EnemyRun()
{
}

void EnemyRun::Update()
{
	Enemy* e = GetBase<Enemy>();
	VECTOR3 sub = EnemyInformation::BASE_POS - e->enemyBaseComponent.enemy->GetEnemyTransform()->position;
	float size = sub.Size();
	VECTOR3 base = EnemyInformation::BASE_POS;
	float size2 = base.Size();
	dir = EnemyInformation::BASE_POS - e->enemyBaseComponent.enemy->GetEnemyTransform()->position;
	dir = dir.Normalize();
	e->enemyBaseComponent.enemy->GetEnemyTransform()->rotation.y = atan2f(dir.x, dir.z);
	//最初にいた場所に戻ってきたらステートを変える
	if (size <= size2) {
		//強制的に止まらせたいので抵抗を強めている
		e->enemyBaseComponent.physics->SetFirction(VECTOR3(500000.0f, 500000.0f, 500000.0f));
		e->enemyBaseComponent.state->ChangeState(ID::E_ANIM_IDOL);
	}
	if (e->enemyBaseComponent.physics->GetVelocity().Size() <= 3000.0f) {
		e->enemyBaseComponent.physics->AddVelocity(dir * 150.0f, false);
	}
	
}

void EnemyRun::Draw()
{
}

void EnemyRun::Start()
{
	Enemy* e = GetBase<Enemy>();
	EnemyStateBase::Start();
	///基準としているポジションから距離をとって角度をとっている。
	dir = EnemyInformation::BASE_POS - e->enemyBaseComponent.enemy->GetEnemyTransform()->position;
	dir = dir.Normalize();
	e->enemyBaseComponent.enemy->GetEnemyTransform()->rotation.y = atan2f(dir.x,dir.z);
	
}

void EnemyRun::Finish()
{
	Enemy* e = GetBase<Enemy>();
	e->enemyBaseComponent.physics->SetFirction(VOne * 3000.0f);
}
