#include "BossRun.h"
#include "Boss.h"
#include "stateManager.h"
#include "AttackSorting.h"

BossRun::BossRun()
{
	animId = ID::B_IDOL;
	id = ID::B_RUN;
	string = Function::GetClassNameC<BossRun>();
}

BossRun::~BossRun()
{
}

void BossRun::Update()
{
	Boss* b = GetBase<Boss>();
	b->LookPlayer();
	rotation = b->obj->GetTransform()->rotation;

	velocity.x = BOSS_SPEED * cosf(-rotation.y - 0.5f * DX_PI_F);
	velocity.z = BOSS_SPEED * sinf(-rotation.y - 0.5f * DX_PI_F);

	b->obj->GetTransform()->position += velocity;

	VECTOR3 targetVec = b->bossTransform->position - b->enemyBaseComponent.playerObj->GetTransform()->position;

	if (targetVec.Size() >= ChaseRange)
	{
		b->enemyBaseComponent.state->ChangeState(ID::B_IDOL);
	}
	if (targetVec.Size() <= 1000.0f)
	{
		/*int a = GetRand(2);
		if (a == 0) {
			b->enemyBaseComponent.state->ChangeState(ID::B_N_ATTACK1);
		}
		else if (a == 1) {
			b->enemyBaseComponent.state->ChangeState(ID::B_N_ATTACK2);
		}
		else {
			b->enemyBaseComponent.state->ChangeState(ID::B_N_ATTACK3);
		}		*/
		b->enemyBaseComponent.state->ChangeState(ID::B_AttackSorting); 
	}
	if (CheckHitKey(KEY_INPUT_M)) {
		b->enemyBaseComponent.state->ChangeState(ID::B_S_ATTACK1);
	}
	
}

void BossRun::Draw()
{
}

void BossRun::Start()
{
	EnemyStateBase::Start();
}

void BossRun::Finish()
{
}
