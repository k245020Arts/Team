#include "CooperateAttack2.h"
#include "../TrashEnemy.h"
#include "T_EnemyStatus.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../State/StateManager.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../Component/Collider/ColliderBase.h"

CooperateAttack2::CooperateAttack2()
{
	string = Function::GetClassNameC<CooperateAttack2>();
	animId = ID::TE_C_ATTACK2;
	attackParam.animID = ID::TE_C_ATTACK2;
	collTrans = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	attackParam.damagePattern = BossAttackBase::BACK;

	attackParam.hitDamage = 50;
	attackParam.useFlash = true;
	attackParam.attackFlashStartTime = 0.7f;
	attackParam.slowAmout = 0.1f;
	attackParam.slowTime = 0.3f;
	attackParam.speedUpMotionSpeed = 0.3f;

	damageMove = false;

	speedDownCounter = 0;

	hitStopCounter = 0.0f;

	pPos = VZero;

	isDamageMove = true;

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

	firstColl = true;

	BossAttackBase::collTrans.position.z = 0.0f;
	BossAttackBase::collTrans.scale = VECTOR3(100, 0, 0);

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
	const float SearchPosMax = 100;
	const float Max = 50;

	if (VSize(pPos - _enemy->GetPos()) > SearchPosMax)
		pPos = _enemy->enemyBaseComponent.playerObj->GetTransform()->position;

	_enemy->LookTarget(pPos);

	if (VSize(pPos - _enemy->GetPos()) < Max)
	{
		_enemy->enemyBaseComponent.anim->Play(ID::TE_IDOL);
		_enemy->enemyBaseComponent.anim->SetPlaySpeed(1.0f);

		_enemy->isStandby = false;
		isDamageMove = false;
			
		return;
	}

	const float ROTY = -_enemy->enemyBaseComponent.playerObj->GetTransform()->rotation.y - 0.5f * DX_PI_F;
	dir = VNorm(pPos - enePos);
	speed = 50.0f;
	
	_enemy->GetEnemyObj()->GetTransform()->position += dir * speed;
	
	AttackCollsion();
	AttackSound();
	AttackFlash(ID::E_MODEL, 35, "E_AttackV");
	Trail();
	EnemyJustAvoidCollsion();

	if (_enemy->cooperateDamageMove && isDamageMove)
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
