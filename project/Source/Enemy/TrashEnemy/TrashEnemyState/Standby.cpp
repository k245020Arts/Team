#include "Standby.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"

Standby::Standby()
{
	counter = 0;
}

Standby::~Standby()
{
}

void Standby::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	e->LookTarget();
	
	counter++;

	if (!e->isCooperateAtk)
	{
		if (counter > 20)
			NormalMove();
		if (e->isAttack)
			e->ChangeState(e->NextId);
	}
	else
	{
		if (counter == 60)
			e->isStandby = true;
		else
			e->isStandby = false;
	}
}

void Standby::Draw()
{
}

void Standby::Start()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	VECTOR3 v = e->enemyBaseComponent.playerObj->GetTransform()->position - e->GetPos();
	angle = -atan2f(v.z, v.x);
	if (e->isCooperateAtk) 
		e->NextId = StateID::COOPERATEATTACK1; 
	else 
		e->NextId = StateID::T_ENEMY_ATTACK_S;

	e->targetPos = e->enemyBaseComponent.playerObj->GetTransform()->position;
	aiMove = 0;
	EnemyStateBase::Start();
}

void Standby::Finish()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	counter = 0;
	e->isStandby = false;
	e->isAttack = false;
}

void Standby::NormalMove()
{
	//TrashEnemy* e = GetBase<TrashEnemy>(); 
	if (aiMove == 0) 
		aiMove = 1 + GetRand(1); 
	if (aiMove == 1) 
		RotateMove(1); 
	else if (aiMove == 2) 
		RotateMove(-1); 
}

void Standby::RotateMove(int index)
{
	TrashEnemy* e = GetBase<TrashEnemy>();

	// プレイヤー位置（毎フレーム更新）
	VECTOR3 pPos = e->enemyBaseComponent.playerObj->GetTransform()->position;

	// 敵の現在位置
	VECTOR3 enemyPos = e->GetPos();

	// 円運動のための角度を進める
	angle += 0.02f * index;

	float RANGE = 800.0f;

	// プレイヤー中心の円周上の位置を計算
	VECTOR3 newPos;
	newPos.x = pPos.x + cosf(angle) * RANGE;
	newPos.z = pPos.z + sinf(angle) * RANGE;
	newPos.y = enemyPos.y;

	// 移動
	e->GetEnemyObj()->GetTransform()->position = newPos;
}

