#include "T_EnemyRUN.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "T_EnemyStatus.h"

T_EnemyRun::T_EnemyRun()
{
	animId = ID::TE_RUN;
	string = Function::GetClassNameC<T_EnemyRun>();

	targetPos = VZero;
}

T_EnemyRun::~T_EnemyRun()
{
}

void T_EnemyRun::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	if (!e->isCooperateAtk)
		targetPos = e->enemyBaseComponent.playerObj->GetTransform()->position;

	rotation = e->obj->GetTransform()->rotation;
	Look();

	const float ROTY = -rotation.y - 0.5f * DX_PI_F;

	//スピードをポジションに足す
	//e->Move(e->speed, 1500);
	
	e->GetEnemyObj()->GetTransform()->position.x += e->eStatus->GetStatus().runSpeed * cosf(ROTY);
	e->GetEnemyObj()->GetTransform()->position.z += e->eStatus->GetStatus().runSpeed * sinf(ROTY);

	VECTOR3 targetVec = targetPos - e->obj->GetTransform()->position;
	
	if (targetVec.Size() <= e->eStatus->GetStatus().atkRang)
	{
		if (!e->IsMovingToPlayer())
			e->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_STANDBY);
		else 
			e->enemyBaseComponent.state->ChangeState(StateID::COOPERATEATTACK1);
	}
}

void T_EnemyRun::Start()
{
	EnemyStateBase::Start();
	TrashEnemy* e = GetBase<TrashEnemy>();
	if (!e->isCooperateAtk)
		targetPos = e->enemyBaseComponent.playerObj->GetTransform()->position;
	else
		targetPos = e->wayPoint;
}

void T_EnemyRun::Finish()
{
}

void T_EnemyRun::Look()
{
	VECTOR3 distance = targetPos - obj->GetTransform()->position;
	//向くべき角度
	float direction = -atan2f(distance.z, distance.x) - 0.5f * DX_PI_F;
	//その角度とどれだけ差があるか
	float sign = direction - obj->GetTransform()->rotation.y;
	//切り捨てして180の境界線を無くす
	sign -= floorf(sign / DX_PI_F / 2) * DX_PI_F * 2.0f;
	if (sign > DX_PI_F)
		sign -= 2 * DX_PI_F;
	//向くスピード(ラジアン)
	const float LOOK_SPEED = 0.5;
	//Playerの方をゆっくり向く
	if (sign > LOOK_SPEED)
		obj->GetTransform()->rotation.y += LOOK_SPEED;
	else if (sign < -LOOK_SPEED)
		obj->GetTransform()->rotation.y -= LOOK_SPEED;
	else
		obj->GetTransform()->rotation.y = direction;
}

//void T_EnemyRun::Move(float _speed, float _max)
//{
//	TrashEnemy* e = GetBase<TrashEnemy>();
//
//	VECTOR3 pos = e->GetBaseObject()->GetTransform()->position;
//
//	// ターゲット座標へ向かうベクトル
//	VECTOR3 dir = VECTOR3(targetPosition - pos).Normalize();
//	dir.y = 0;
//
//	// スピードをかける
//	VECTOR3 velocity = dir * _speed;
//
//	e->enemyBaseComponent.physics->AddVelocity(velocity, false);
//
//	VECTOR3 moveVelo = e->enemyBaseComponent.physics->GetVelocity() * VECTOR3(1, 0, 1);
//
//	// 最大速度制限
//	if (moveVelo.SquareSize() >= _max * _max)
//	{
//		moveVelo = moveVelo.Normalize() * _max;
//		moveVelo.y = enemyBaseComponent.physics->GetVelocity().y;
//		enemyBaseComponent.physics->SetVelocity(moveVelo);
//	}
//
//	//VECTOR3 dir = VZero;
//	//dir.y *= 0.0f;
//	////b->bossTransform->GetRotationMatrix();
//	//dir = GetBaseObject()->GetTransform()->Forward() * -_speed;
//	//enemyBaseComponent.physics->AddVelocity(dir, false);
//	//VECTOR3 moveVelo;
//	//moveVelo = enemyBaseComponent.physics->GetVelocity() * VECTOR3(1.0f, 0.0f, 1.0f);
//
//	//float max = _max;
//	////最大速度までいったらスピードマックスに補正
//	//if (moveVelo.SquareSize() >= max * max) {
//	//	moveVelo = moveVelo.Normalize() * max;
//	//	moveVelo.y = enemyBaseComponent.physics->GetVelocity().y;
//	//	enemyBaseComponent.physics->SetVelocity(moveVelo);
//	//}
//}