#include "Standby.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "T_EnemyStatus.h"
#include "../../../Common/Random.h"
#include "../../../Camera/Camera.h"
#include "../../../Common/Effect/EffectManager.h"

Standby::Standby()
{
	animId = ID::TE_STANCE;
	string = Function::GetClassNameC<Standby>();
	counter = 0;

	range = 0.0f;
	randomSpeed = 0.0f;
	redefinitionCounter = 0.0f;
	isRedefinition = true;

	runTime = 0.0f;
}

Standby::~Standby()
{
}

void Standby::Update()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	pPos = enemy->enemyBaseComponent.playerObj->GetTransform()->position;
	enemy->LookTarget(pPos);

	vec = enemy->enemyBaseComponent.playerObj->GetTransform()->position - enemy->GetPos();
	if (!enemy->isCooperateAtk)
	{
		InCameraView();

		if (vec.Size() <= range)//プレイヤーとの距離を見て後ろに下がる
		{
			isRedefinition = true;
			float rotY = enemy->GetEnemyObj()->GetTransform()->rotation.y;

			enemy->GetEnemyObj()->GetTransform()->position.x -= range / BACKSPEED * cosf(rotY - 0.5f * DX_PI_F);
			enemy->GetEnemyObj()->GetTransform()->position.z -= range / BACKSPEED * sinf(rotY - 0.5f * DX_PI_F);
		}
		else
		{
			RotateMove();
			if (vec.Size() <= range / 2)
			{
				pPos = enemy->enemyBaseComponent.playerObj->GetTransform()->position;
				counter = 0;
			}
		}

		if (vec.Size() >= enemy->eStatus->GetStatus().chaseRange)
			runTime += Time::DeltaTimeRate();
		else
			runTime = 0.0f;

		if (runTime >= 0.5f)
			enemy->ChangeState(StateID::T_ENEMY_RUN_S);
	}
	else
	{
		if (counter <= 1)
			counter += Time::DeltaTimeRate();
		else if (counter != 2)
		{
			enemy->isStandby = true;
			EffectManager::GetInstance()->CreateEffekseer(Transform(enemy->GetPos(), VZero, VOne), nullptr, Effect_ID::COOPERATEATTACK, 1.0f);
			counter = 2;
		}
		else
			enemy->isStandby = false;
	}
}

void Standby::Draw()
{
}

void Standby::Start()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	range = enemy->eStatus->GetStatus().atkRang;
	
	if (enemy->isCooperateAtk)
		enemy->isMovingToPlayer = true;
		
	pPos = enemy->enemyBaseComponent.playerObj->GetTransform()->position;

	randomSpeed = (float)Random::GetReal();

	runTime = 0.0f;

	EnemyStateBase::Start();
}

void Standby::Finish()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	counter = 0;
	enemy->isStandby = false;
	enemy->isAttack = false;
	isRedefinition = true;
}

void Standby::RotateMove()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	const VECTOR3 EPos = enemy->GetPos();
	const float vecMax = 200.0f;

	if (isRedefinition)
	{
		float MaxPos = 1500;
		enemy->enemyBaseComponent.anim->Play(ID::TE_STANCE);
		float _rangeX = MaxPos * (float)Random::GetReal() - MaxPos * (float)Random::GetReal();
		float _rangeZ = MaxPos * (float)Random::GetReal() - MaxPos * (float)Random::GetReal();
		newPos = VECTOR3(_rangeX, 0, _rangeZ) + EPos;
		isRedefinition = false;
	}
	else if (VSize(newPos - EPos) >= vecMax)
	{
		// 移動
		VECTOR3 dir = VNorm(newPos - EPos);
		enemy->GetEnemyObj()->GetTransform()->position += dir * 10;
	}
	else
	{
		redefinitionCounter += Time::DeltaTimeRate();
		enemy->enemyBaseComponent.anim->Play(ID::TE_IDOL);
		const float MaxTime = 1.0f;

		if (redefinitionCounter >= MaxTime)
		{
			isRedefinition = true;
			redefinitionCounter = 0;
		}
	}
}

void Standby::InCameraView()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	VECTOR3 cameraPos = enemy->enemyBaseComponent.camera->GetCameraTransform()->position;
	cameraPos.y = 0;
	VECTOR3 frontVec = VECTOR3(0, 0, 1) * MGetRotY(enemy->enemyBaseComponent.camera->GetCameraTransform()->rotation.y);
	VECTOR3 vec = enemy->GetEnemyObj()->GetTransform()->position - enemy->enemyBaseComponent.playerObj->GetTransform()->position;
	
	//内積
	float dotProduct = VDot(frontVec, vec.Normalize());
	
	if (dotProduct > cosf(45 * DegToRad) )//カメラに写っているかつプレイヤーの前
		enemy->isAttack = true;
	else
		enemy->isAttack = false;
}
