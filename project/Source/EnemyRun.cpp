#include "EnemyRun.h"
#include "function.h"
#include "EnemyStateManager.h"
#include "Physics.h"
#include "Animator.h"
#include "enemy.h"

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
	VECTOR3 sub = EnemyInformation::BASE_POS - e->eCom.enemy->GetEnemyTransform()->position;
	float size = sub.Size();
	VECTOR3 base = EnemyInformation::BASE_POS;
	float size2 = base.Size();
	dir = EnemyInformation::BASE_POS - e->eCom.enemy->GetEnemyTransform()->position;
	dir = dir.Normalize();
	e->eCom.enemy->GetEnemyTransform()->rotation.y = atan2f(dir.x, dir.z);
	//最初にいた場所に戻ってきたらステートを変える
	if (size <= size2) {
		//強制的に止まらせたいので抵抗を強めている
		e->eCom.physics->SetFirction(VECTOR3(500000.0f, 500000.0f, 500000.0f));
		e->eCom.state->ChangeState(ID::E_ANIM_IDOL);
	}
	if (e->eCom.physics->GetVelocity().Size() <= 3000.0f) {
		e->eCom.physics->AddVelocity(dir * 150.0f, false);
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
	dir = EnemyInformation::BASE_POS - e->eCom.enemy->GetEnemyTransform()->position;
	dir = dir.Normalize();
	e->eCom.enemy->GetEnemyTransform()->rotation.y = atan2f(dir.x,dir.z);
	
}

void EnemyRun::Finish()
{
	Enemy* e = GetBase<Enemy>();
	e->eCom.physics->SetFirction(VOne * 3000.0f);
}
