#include "T_EnemyWaitSee.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "../../../Common/Random/Random.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Common/Easing/Easing.h"

T_EnemyWaitSee::T_EnemyWaitSee()
{
	animId = ID::TE_R_IDOL_SKY;

	attackCounter = 0.0f;
	targetPos = VZero;
	isLeader = false;

	moveSpeed = 0;

	pointRange = 0.0f;
	setGravity = 0;

	floatingTimeCounter = 1.0f;
	copyPositionY = 0;
	isUpMove = false;
}

T_EnemyWaitSee::~T_EnemyWaitSee()
{
}

void T_EnemyWaitSee::Update()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	if (enemy->IsPlayerSpecialMove())
		return;
	
	enemy->LookTarget(enemy->enemyBaseComponent.playerObj->GetTransform()->position);

	Move(enemy);
}

void T_EnemyWaitSee::Start()
{
	const TrashEnemy* enemy = GetBase<TrashEnemy>();
	setGravity = enemy->enemyBaseComponent.physics->GetGravity();
	enemy->enemyBaseComponent.physics->SetGravity(VZero);

	targetPos = enemy->TargetPoint();
	isUpMove = true;

	switch (enemy->enemyType)
	{
	case EnemyType::RANGED:
		moveSpeed = NormalMoveSpeed;
		pointRange = LeaderRange + RangedRange * (float)Random::GetReal();
		break;
	case EnemyType::RANGED_LEADER:
		pointRange = LeaderRange;
		moveSpeed = LeaderMoveSpeed;
		isLeader = true;
		break;
	default:
		break;
	}

	EnemyStateBase::Start();
}

void T_EnemyWaitSee::Finish()
{
}

void T_EnemyWaitSee::Move(TrashEnemy* _enemy)
{
	const VECTOR3 enePos = _enemy->GetPos();
	targetPos = _enemy->TargetPoint();

	if (!_enemy->rangedGravity)
	{
		if (isUpMove)
		{
			targetPos.y = AveragePositionY;
			const bool IsUpMove = enePos.y >= targetPos.y - 50;
			float moveDelta = AveragePositionY - enePos.y;
			_enemy->GetEnemyObj()->GetTransform()->position.y += moveDelta * 0.01f;
			if (IsUpMove)
			{
				copyPositionY = enePos.y;
				isUpMove = false;
			}
		}
		else
			FloatingMove(_enemy);
	}
	else
	{
		_enemy->enemyBaseComponent.physics->SetGravity(setGravity);
		if(enePos.y <= 0.0f)
			_enemy->enemyBaseComponent.anim->Play(ID::TE_R_IDOL);
	}

	if (VSize(targetPos - enePos) < pointRange)
		return;

	VECTOR3 dir = VNorm(targetPos - enePos);
	_enemy->GetEnemyObj()->GetTransform()->position+= dir * moveSpeed;
}

void T_EnemyWaitSee::FloatingMove(TrashEnemy* _enemy)
{
	floatingTimeCounter += Time::DeltaTimeRate();
	// ã‰º‚É—h‚ê‚é—Ê‚ðŒvŽZ
	const float offsetY = copyPositionY + Easing::SinCube(-DeviationPositionY, DeviationPositionY, sinf(floatingTimeCounter));
	_enemy->GetEnemyObj()->GetTransform()->position.y = offsetY;
}
