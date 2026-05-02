#include "T_EnemyIdol.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "T_EnemyStatus.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Common/Random.h"

T_EnemyIdol::T_EnemyIdol()
{
	animId = ID::TE_IDOL;
	string = Function::GetClassNameC<T_EnemyIdol>();
	detectionRange = 0;

	setGravity = VZero;
}

T_EnemyIdol::~T_EnemyIdol()
{
}

void T_EnemyIdol::Update()
{
	NormalMove();
}

void T_EnemyIdol::Draw()
{
}

void T_EnemyIdol::Start()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	setGravity = enemy->enemyBaseComponent.physics->GetGravity();
	float addSpeed = (float)Random::GetInt(0, 500);
	enemy->enemyBaseComponent.physics->SetGravity(VECTOR3(0, -1000 - addSpeed, 0));

	EnemyStateBase::Start();
}

void T_EnemyIdol::Finish()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	enemy->enemyBaseComponent.physics->SetGravity(setGravity);
}

void T_EnemyIdol::NormalMove()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	if (enemy->GetPos().y >= 30)
		return;

	VECTOR3 targetVec = enemy->obj->GetTransform()->position - enemy->enemyBaseComponent.playerObj->GetTransform()->position;
	detectionRange += Time::DeltaTimeRate() * RANGESPEED;
	if (targetVec.Size() < enemy->eStatus->GetStatus().chaseRange + detectionRange || enemy->GetEnemyType()!=enemy->EnemyType::MELEE)
		enemy->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_RUN_S);
}
