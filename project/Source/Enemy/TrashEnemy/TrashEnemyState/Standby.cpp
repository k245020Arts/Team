#include "Standby.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "T_EnemyStatus.h"
#include "../../../Common/Random.h"
#include "../../../Camera/Camera.h"

Standby::Standby()
{
	animId = ID::TE_IDOL;
	string = Function::GetClassNameC<Standby>();
	counter = 0;

	range = 0;
	randomSpeed = 0;
	redefinitionCounter = 0;
	isRedefinition = true;
}

Standby::~Standby()
{
}

void Standby::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	pPos = e->enemyBaseComponent.playerObj->GetTransform()->position;
	e->LookTarget(pPos);
	
	vec = e->enemyBaseComponent.playerObj->GetTransform()->position - e->GetPos();
	if (!e->isCooperateAtk)
	{
		InCameraView();
		//if (counter < BACKSPEED)
		//{
			if (vec.Size() <= range)//プレイヤーとの距離を見て後ろに下がる
			{
				isRedefinition = true;
				float rotY = e->GetEnemyObj()->GetTransform()->rotation.y;

				e->GetEnemyObj()->GetTransform()->position.x -= range / BACKSPEED * cosf(rotY - 0.5f * DX_PI_F);
				e->GetEnemyObj()->GetTransform()->position.z -= range / BACKSPEED * sinf(rotY - 0.5f * DX_PI_F);
			}
			/*else
				counter++;*/
		//}
		else
		{
			RotateMove();
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
		if (counter <= 1)
			counter += Time::DeltaTimeRate();
		else if (counter != 2)
		{
			e->isStandby = true;
			counter = 2;
		}
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
	/*else
		e->isAttack = true;*/

	pPos = e->enemyBaseComponent.playerObj->GetTransform()->position;

	randomSpeed = (float)Random::GetReal();
	EnemyStateBase::Start();
}

void Standby::Finish()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	counter = 0;
	e->isStandby = false;
	e->isAttack = false;
	isRedefinition = true;

}

void Standby::RotateMove()
{
	TrashEnemy* e = GetBase<TrashEnemy>();

	VECTOR3 pPos = e->enemyBaseComponent.playerObj->GetTransform()->position;

	float MAX = 50;
	if (isRedefinition)
	{
		float _rangeX = MAX * (float)Random::GetReal() - MAX * (float)Random::GetReal();
		float _rangeZ = MAX * (float)Random::GetReal() - MAX * (float)Random::GetReal();
		newPos = /*enemyPos +*/ VECTOR3(_rangeX, 0, _rangeZ);
		isRedefinition = false;
	}
	else if (newPos.Size() <= e->eStatus->GetStatus().atkRang)
	{
		redefinitionCounter += Time::DeltaTimeRate();

		if (redefinitionCounter >= 5)
		{
			isRedefinition = true;
			redefinitionCounter = 0;
		}
	}

	// 移動
	e->GetEnemyObj()->GetTransform()->position += 10 * newPos.Normalize();
}

void Standby::InCameraView()
{
	TrashEnemy* _e = GetBase<TrashEnemy>();
	VECTOR3 cameraPos = _e->enemyBaseComponent.camera->GetCameraTransform()->position;
	cameraPos.y = 0;
	VECTOR3 frontVec = VECTOR3(0, 0, 1) * MGetRotY(_e->enemyBaseComponent.camera->GetCameraTransform()->rotation.y);
	VECTOR3 vec = _e->GetEnemyObj()->GetTransform()->position - _e->enemyBaseComponent.playerObj->GetTransform()->position;
	
	//内積
	float dotProduct = VDot(frontVec, vec.Normalize());
	
	if (dotProduct > cosf(45 * DegToRad) )//カメラに写っているかつプレイヤーの前
		_e->isAttack = true;
	else
		_e->isAttack = false;
}

//float Standby::CalculateAngle()
//{
//	return -GetBase<TrashEnemy>()->GetEnemyObj()->GetTransform()->rotation.y + 0.5f * DX_PI_F;
//}
