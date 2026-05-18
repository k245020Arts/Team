#include "T_EnemyStaySky.h"
#include "../TrashEnemy.h"
#include "../../../State/StateManager.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Camera/Camera.h"

T_EnemyStaySky::T_EnemyStaySky()
{
	animId = ID::TE_R_IDOL;
	string = Function::GetClassNameC<T_EnemyStaySky>();

	isLeader = false;
	setGravity = VZero;
}

T_EnemyStaySky::~T_EnemyStaySky()
{
}

void T_EnemyStaySky::Update()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	if (!isLeader)
		RangedMove(enemy);
	else
		LeaderMove(enemy);
}

void T_EnemyStaySky::Start()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	if (enemy->GetEnemyType() == EnemyType::RANGED_LEADER)
		isLeader = true;

	setGravity = enemy->enemyBaseComponent.physics->GetGravity();
	enemy->enemyBaseComponent.physics->SetGravity(VZero);

	EnemyStateBase::Start();
}

void T_EnemyStaySky::Finish()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	enemy->enemyBaseComponent.physics->SetGravity(setGravity);

	if(isLeader)
		enemy->CooperateAtkFinish();
}

void T_EnemyStaySky::LeaderMove(TrashEnemy* _enemy)
{
	const float Speed = 50.0f;
	const float MaxPos = 1800.0f;

	if (_enemy->GetPos().y <= MaxPos)
		_enemy->GetEnemyObj()->GetTransform()->position.y += Speed;
	else
	{
		_enemy->isStandby = true;
		_enemy->enemyBaseComponent.camera->ChangeStateCamera(StateID::R_ENEMY_CAMERA_S);
	}
}

void T_EnemyStaySky::RangedMove(TrashEnemy* _enemy)
{
	const VECTOR3 enePos = _enemy->GetPos();
	const VECTOR3 targetPos = _enemy->cooperateWayPoint;
	VECTOR3 dir = VZero;
	float speed = 0.0f;

	//ƒŠ[ƒ_[‚ÌŽü‚è‚ÉˆÚ“®
	if (VSize(targetPos - enePos) >= 30 && !_enemy->isStandby)
	{
		dir = VNorm(targetPos - enePos);
		speed = 30.0f;

		_enemy->GetEnemyObj()->GetTransform()->position += dir * speed;
	}
	else
		_enemy->isStandby = true;
}
