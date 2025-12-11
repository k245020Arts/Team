#include "Standby.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "T_EnemyStatus.h"

Standby::Standby()
{
	counter = 0;
	pPos = VZero;
}

Standby::~Standby()
{
}

void Standby::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	e->LookTarget();
	NormalMove();

	if (!e->isCooperateAtk)
		return;

	counter++;
	if (counter == 60)
		e->isStandby = true;
	else
		e->isStandby = false;
}

void Standby::Draw()
{
}

void Standby::Start()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	velocity = VZero;
	aiMove = 0;
	e->targetPos = e->enemyBaseComponent.playerObj->GetTransform()->position;
	acquisition = true;
	//連携攻撃の時の攻撃
	if (e->isCooperateAtk)
		e->attackId = StateID::COOPERATEATTACK1;
	else
		e->attackId = StateID::T_ENEMY_ATTACK_S;

	EnemyStateBase::Start();
}

void Standby::Finish()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	counter = 0;
	e->isCooperateAtk = false;
	e->isStandby = false;
}

void Standby::NormalMove()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	if(aiMove == 0)
		aiMove = 1 + GetRand(1);

	if (aiMove == 1)
		RotateMove(1);
	else if (aiMove == 2)
		RotateMove(-1);
	else
		int a = 0;

	//e->GetEnemyObj()->GetTransform()->position = velocity;
}

void Standby::RotateMove(float index)
{
	float RANGE = 1000;
	TrashEnemy* e = GetBase<TrashEnemy>();
	if (acquisition)
	{
		pPos = e->enemyBaseComponent.playerObj->GetTransform()->position;
		//ベクトル計算
		vec = pPos - e->GetPos();
		//角度をだす
		targetDirection = atan2f(vec.z, vec.x) + DX_PI_F;
		acquisition = false;
	}
	
	//プレイヤーとの距離を測る
	if (vec.Size()<= e->eStatus->GetStatus().atkRang)
	{
		//近すぎたら離れる
		e->GetEnemyObj()->GetTransform()->position.x = -5 * cosf(-e->GetEnemyObj()->GetTransform()->rotation.y - 0.5f * DX_PI_F);
		e->GetEnemyObj()->GetTransform()->position.z = -5 * sinf(-e->GetEnemyObj()->GetTransform()->rotation.y - 0.5f * DX_PI_F);
	}
	else
	{
		//ある程度離れたら動く
		e->GetEnemyObj()->GetTransform()->position.x = pPos.x + cos(targetDirection) * RANGE;
		e->GetEnemyObj()->GetTransform()->position.z = pPos.z + sin(targetDirection) * RANGE;

		targetDirection = targetDirection + 0.2 * index;
	}
}
