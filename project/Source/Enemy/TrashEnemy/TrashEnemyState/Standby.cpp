#include "Standby.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "T_EnemyStatus.h"

Standby::Standby()
{
	animId = ID::TE_IDOL;
	string = Function::GetClassNameC<Standby>();
	counter = 0;
}

Standby::~Standby()
{
}

void Standby::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	e->LookTarget();

	vec = e->enemyBaseComponent.playerObj->GetTransform()->position - e->GetPos();
	if (!e->isCooperateAtk)
	{
		if (vec.Size() <= e->eStatus->GetStatus().atkRang && counter == 0)
		{
			float rotY = e->GetEnemyObj()->GetTransform()->rotation.y;

			e->GetEnemyObj()->GetTransform()->position.x += -10 * cosf(rotY - 0.5f * DX_PI_F) / 2;
			e->GetEnemyObj()->GetTransform()->position.z += -10 * sinf(rotY - 0.5f * DX_PI_F) / 2;

			//RANGE = v.Size();
		}
		else
			counter++;

		if(counter > 20)
			NormalMove();

		if (e->isAttack)
			e->ChangeState(e->NextId);
	}
	else
	{
		counter++;
		if (counter == 40)
			e->isStandby = true;
		else
			e->isStandby = false;
	}

	if (vec.Size() >= e->eStatus->GetStatus().chaseRange)
		e->ChangeState(StateID::T_ENEMY_RUN_S);
}

void Standby::Draw()
{
}

void Standby::Start()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	vec = e->enemyBaseComponent.playerObj->GetTransform()->position - e->GetPos();
	
	angle = -atan2f(vec.z, vec.x);

	range = e->eStatus->GetStatus().range;

	if (e->isCooperateAtk) 
		e->NextId = StateID::COOPERATEATTACK1; 
	else 
		e->NextId = StateID::T_ENEMY_ATTACK_S;

	e->targetPos = e->enemyBaseComponent.playerObj->GetTransform()->position;
	pPos = e->enemyBaseComponent.playerObj->GetTransform()->position;
	aiMove = 0;
	e->isAttack = false;
	EnemyStateBase::Start();
}

void Standby::Finish()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	counter = 0;
	e->isStandby = false;
	e->isAttack = false;
	if (!e->isCooperateAtk)
		e->NextId = StateID::T_ENEMY_STANDBY;
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

void Standby::RotateMove(int rotDir)
{
	TrashEnemy* e = GetBase<TrashEnemy>();

	//pPos = e->enemyBaseComponent.playerObj->GetTransform()->position;
	// “G‚ÌŒ»ÝˆÊ’u
	VECTOR3 enemyPos = e->GetPos();

	// ‰~‰^“®‚Ì‚½‚ß‚ÌŠp“x‚ði‚ß‚é
	angle += 0.5f * DegToRad * rotDir;

	// ƒvƒŒƒCƒ„[’†S‚Ì‰~Žüã‚ÌˆÊ’u‚ðŒvŽZ
	VECTOR3 newPos;
	newPos.x = pPos.x + cosf(angle) * range;
	newPos.z = pPos.z + sinf(angle) * range;
	//newPos.y = enemyPos.y;

	// ˆÚ“®
	e->GetEnemyObj()->GetTransform()->position = newPos;
}
