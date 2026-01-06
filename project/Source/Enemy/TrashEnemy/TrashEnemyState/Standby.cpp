#include "Standby.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "T_EnemyStatus.h"
#include "../../../Common/Random.h"

Standby::Standby()
{
	animId = ID::TE_IDOL;
	string = Function::GetClassNameC<Standby>();
	counter = 0;

	randomSpeed = 0;
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
		if (counter < BACKSPEED)
		{
			if (vec.Size() <= range)
			{
				float rotY = e->GetEnemyObj()->GetTransform()->rotation.y;

				e->GetEnemyObj()->GetTransform()->position.x -= range / BACKSPEED * cosf(rotY - 0.5f * DX_PI_F);
				e->GetEnemyObj()->GetTransform()->position.z -= range / BACKSPEED * sinf(rotY - 0.5f * DX_PI_F);
			}
			else
				counter++;

			angle = CalculateAngle();
		}
		else
		{
			//NormalMove();
			if (vec.Size() <= range / 2)
			{
				pPos = e->enemyBaseComponent.playerObj->GetTransform()->position;
				counter = 0;
			}
		}

		if (vec.Size() >= e->eStatus->GetStatus().chaseRange)
			e->ChangeState(StateID::T_ENEMY_RUN_S);
	}
	else
	{
		if (counter == 0)
			e->GetEnemyObj()->GetTransform()->position = e->wayPoint;

		counter++;
		
		if (counter == 20)
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

	range = e->eStatus->GetStatus().atkRang;
	
	if (e->isCooperateAtk)
		e->isMovingToPlayer = true;
	else 
		int a = 0;

	pPos = e->enemyBaseComponent.playerObj->GetTransform()->position;

	e->targetPos = pPos;
	aiMove = 0;

	randomSpeed = (float)Random::GetReal();
	EnemyStateBase::Start();
}

void Standby::Finish()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	counter = 0;
	e->isStandby = false;
	e->isAttack = false;
	/*if (!e->isCooperateAtk)
		e->nextId = StateID::T_ENEMY_STANDBY;*/

	e->isCooperateAtk = false;
}

void Standby::NormalMove()
{
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

	// ‰~‰^“®‚Ì‚½‚ß‚ÌŠp“x‚ði‚ß‚é
	angle += 0.5f * randomSpeed * DegToRad * rotDir;

	// ƒvƒŒƒCƒ„[’†S‚Ì‰~Žüã‚ÌˆÊ’u‚ðŒvŽZ
	VECTOR3 newPos;
	newPos.x = pPos.x + cosf(angle) * range;
	newPos.z = pPos.z + sinf(angle) * range;

	// ˆÚ“®
	e->GetEnemyObj()->GetTransform()->position = newPos;
}

float Standby::CalculateAngle()
{
	return -GetBase<TrashEnemy>()->GetEnemyObj()->GetTransform()->rotation.y + 0.5f * DX_PI_F;
}
