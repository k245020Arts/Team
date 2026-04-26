#include "CooperateAttack2.h"
#include "../TrashEnemy.h"
#include "T_EnemyStatus.h"

CooperateAttack2::CooperateAttack2()
{
	string = Function::GetClassNameC<CooperateAttack2>();
	attackParam.animID = ID::TE_ATTACK2;
	collTrans = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	attackParam.damagePattern = BossAttackBase::BACK;

	attackParam.hitDamage = 30;
	attackParam.useFlash = true;
	attackParam.attackFlashStartTime = 0.7f;
	attackParam.slowAmout = 0.1f;
	attackParam.slowTime = 0.3f;
	attackParam.speedUpMotionSpeed = 0.3f;

	isLeader = false;
}

CooperateAttack2::~CooperateAttack2()
{
}

void CooperateAttack2::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();

	if (!isLeader)
		RangedMove(e);
	else 
		LeaderMove(e);
}

void CooperateAttack2::Start()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	/*if (enemy->GetEnemyType() == enemy->EnemyType::RANGED)
	else */
	if (enemy->GetEnemyType() == enemy->EnemyType::RANGED_LEADER)
		isLeader = true;;

	EnemyStateBase::Start();
}

void CooperateAttack2::Finish()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	enemy->CooperateAtkFinish();
}

void CooperateAttack2::RangedMove(TrashEnemy* _enemy)
{
	const VECTOR3 enePos = _enemy->GetPos();
	const VECTOR3 targetPos = _enemy->cooperateWayPoint;
	VECTOR3 dir = VZero;
	float speed = 0.0f;

	if (VSize(targetPos - enePos) && !_enemy->isStandby)
	{
		dir = VNorm(targetPos - enePos);
		speed = 10.0f;

		_enemy->GetEnemyObj()->GetTransform()->position += dir * speed;
	}
	else
		_enemy->isStandby = true;

	if (!_enemy->isStandby)
		return;

	const VECTOR3 pPos = _enemy->enemyBaseComponent.playerObj->GetTransform()->position;
	_enemy->LookTarget(pPos);

	const float ROTY = -_enemy->enemyBaseComponent.playerObj->GetTransform()->rotation.y - 0.5f * DX_PI_F;

	/*_enemy->GetEnemyObj()->GetTransform()->position.x += _enemy->eStatus->GetStatus().runSpeed * cosf(ROTY);
	_enemy->GetEnemyObj()->GetTransform()->position.z += _enemy->eStatus->GetStatus().runSpeed * sinf(ROTY);*/
	speed = 30.0f;
	
	_enemy->GetEnemyObj()->GetTransform()->position += dir * speed;
}

void CooperateAttack2::LeaderMove(TrashEnemy* _enemy)
{
	const float Speed = 20.0f;
	const float MaxPos = 100.0f;

	if (_enemy->GetEnemyObj()->GetTransform()->position.y >= MaxPos)
		_enemy->GetEnemyObj()->GetTransform()->position.y += Speed;
}
