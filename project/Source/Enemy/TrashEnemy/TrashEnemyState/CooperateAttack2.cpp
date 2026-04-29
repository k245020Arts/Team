#include "CooperateAttack2.h"
#include "../TrashEnemy.h"
#include "T_EnemyStatus.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../State/StateManager.h"

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

	damageMove = false;

	speedDownCounter = 0;

	hitStopCounter = 0.0f;
}

CooperateAttack2::~CooperateAttack2()
{
}

void CooperateAttack2::Update()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	if (!damageMove)
			RangedMove(enemy);
		else
			DamageMove(enemy);
}

void CooperateAttack2::Start()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	enemy->isMovingToPlayer = true;
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

	const VECTOR3 pPos = _enemy->enemyBaseComponent.playerObj->GetTransform()->position;
	_enemy->LookTarget(pPos);

	const float ROTY = -_enemy->enemyBaseComponent.playerObj->GetTransform()->rotation.y - 0.5f * DX_PI_F;
	dir = VNorm(pPos - enePos);
	speed = 50.0f;
	
	_enemy->GetEnemyObj()->GetTransform()->position += dir * speed;
	
	AttackCollsion();
	AttackSound();
	AttackFlash(ID::E_MODEL, 35, "E_AttackV");
	Trail();
	EnemyJustAvoidCollsion();

	if (_enemy->cooperateDamageMove)
		damageMove = true;
}

void CooperateAttack2::DamageMove(TrashEnemy* _enemy)
{
	hitStopCounter += Time::DeltaTimeRate();
	const float CounterMax = 0.5f;
	if (hitStopCounter < CounterMax)
		return;

	const VECTOR3 enePos = _enemy->GetPos();
	const VECTOR3 targetPos = _enemy->cooperateWayPoint;
	VECTOR3 dir = VNorm(targetPos - enePos);
	const float Speed = 100.0f;
	
	speedDownCounter += Time::DeltaTimeRate();

	_enemy->GetEnemyObj()->GetTransform()->position += dir * (Speed - speedDownCounter);
}

//void CooperateAttack2::LeaderMove(TrashEnemy* _enemy)
//{
//	const float Speed = 50.0f;
//	const float MaxPos = 1800.0f;
//
//	float a = _enemy->GetPos().y;
//	if (_enemy->GetPos().y <= MaxPos)
//		_enemy->GetEnemyObj()->GetTransform()->position.y += Speed;
//	else
//		_enemy->isStandby = true;
//}
