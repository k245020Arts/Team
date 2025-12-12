#include "T_EnemyAttack.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "T_EnemyStatus.h"

T_EnemyAttack::T_EnemyAttack()
{
	string = Function::GetClassNameC<T_EnemyAttack>();
	animId = ID::TE_ATTACK;
	collTrans = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));

	damage.damagePattern = BossAttackBase::NO_BACK;
}

T_EnemyAttack::~T_EnemyAttack()
{
}

void T_EnemyAttack::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	e->LookTarget();

	if (!e->isCooperateAtk)
	{
		counter++;

		if (counter >= 5&&counter<=10)
		{
			e->GetEnemyObj()->GetTransform()->position.x += 40 * cosf(-e->GetEnemyObj()->GetTransform()->rotation.y - 0.5f * DX_PI_F);
			e->GetEnemyObj()->GetTransform()->position.z += 40 * sinf(-e->GetEnemyObj()->GetTransform()->rotation.y - 0.5f * DX_PI_F);
		}				
	}

	if (e->enemyBaseComponent.anim->IsFinish())
		e->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_RUN_S);

	AttackCollsion();
	AttackSound();
	//AttackFlash(ID::E_MODEL, 35, "E_AttackV");
	Trail();
}

void T_EnemyAttack::Draw()
{
}

void T_EnemyAttack::Start()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	EnemyStateBase::Start();
	firstColl = true;
	e->enemyBaseComponent.anim->SetFrame(5.0f);
	damage.hitDamage = e->eStatus->GetStatus().normalAttack1;
	counter = 0;
}

void T_EnemyAttack::Finish()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	e->isAttack = false;
	e->NextId = StateID::T_ENEMY_STANDBY;
	e->DeleteCollision();
}
