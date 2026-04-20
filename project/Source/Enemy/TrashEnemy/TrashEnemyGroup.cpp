#include "TrashEnemyGroup.h"
#include "TrashEnemy.h"
#include "../../Common/Random.h"
#include "../../Camera/Camera.h"

TrashEnemyGroup::TrashEnemyGroup()
{
	camera = FindGameObjectWithTag<Object3D>("CAMERA_OBJ")->Component()->GetComponent<Camera>();
	trashEnemyManager = FindGameObject<TrashEnemyManager>();

	hasLeader = false;
	attackCounter = 0;

	enemiesRunCounter = 0;

	separationTime = 0.0f;

	leaderPos = VZero;
}

TrashEnemyGroup::~TrashEnemyGroup()
{
}

void TrashEnemyGroup::Update()
{
	Separation();
	EnemyDeaad(meleeEnemies);
	EnemyDeaad(rangedEnemies);
	
	//近距離の敵関連
	for (auto melee : meleeEnemies)
	{
		EnemiesRun(melee);
		MeleeEnemyAttack(melee);
		CooperateAttackMove(melee);
	}

	for (auto ranged : rangedEnemies)
	{
		RangedEnemyAttack(ranged);
	}
}

void TrashEnemyGroup::SettingGroup(TrashEnemy* _enemy, int _index)
{
	if (_index % 2 == 0)
	{
		_enemy->SetEnemyType(_enemy->EnemyType::MELEE);
		meleeEnemies.push_back(_enemy);
	}
	else if (hasLeader)
	{
		_enemy->SetEnemyType(_enemy->EnemyType::RANGED);
		rangedEnemies.push_back(_enemy);
	}
	else
	{
		_enemy->SetEnemyType(_enemy->EnemyType::RANGED_LEADER);
		rangedEnemies.push_back(_enemy);
		hasLeader = true;
	}
}

void TrashEnemyGroup::SetMeleeEnemy(TrashEnemy* _enemy)
{
	_enemy->SetEnemyType(_enemy->EnemyType::MELEE);
	meleeEnemies.push_back(_enemy);
}

void TrashEnemyGroup::SetRangedEnemy(TrashEnemy* _enemy)
{
	if (hasLeader)
	{
		_enemy->SetEnemyType(_enemy->EnemyType::RANGED);
		rangedEnemies.push_back(_enemy);
	}
	else
	{
		_enemy->SetEnemyType(_enemy->EnemyType::RANGED_LEADER);
		rangedEnemies.push_back(_enemy);
		hasLeader = true;
	}
}

int TrashEnemyGroup::GetEnemySize()const
{
	return meleeEnemies.size() + rangedEnemies.size();
}

void TrashEnemyGroup::EnemyDeaad(std::list<TrashEnemy*>& enemies)
{
	for (auto itr = enemies.begin(); itr != enemies.end(); )
	{
		//雑魚敵が死んでたらlistから削除する
		if (!(*itr)->GetActive())
		{
			(*itr)->GetEnemyObj()->DestroyMe();
			//次の要素のイテレータが返る
			itr = enemies.erase(itr);
		}
		else
			++itr;
	}
}

void TrashEnemyGroup::Separation()
{
	separationTime += Time::DeltaTimeRate();

	if (separationTime <= 0.5f)
		return;

	separationTime = 0.0f;

	VECTOR pos1 = { 0,0,0 };
	VECTOR pos2 = { 0,0,0 };
	const float E_SIZE = 500;

	allEnemy.clear();

	for (auto& melee : meleeEnemies)
	{
		allEnemy.push_back(melee);
	}
	for (auto& ranged : rangedEnemies)
	{
		allEnemy.push_back(ranged);
	}

	for (auto& itr1 : allEnemy)
	{
		for (auto& itr2 : allEnemy)
		{
			if (itr1 == itr2)
				continue;

			pos1 = itr1->GetPos();
			pos2 = itr2->GetPos();
			VECTOR3 vec = pos1 - pos2;
			VECTOR3 vec2 = pos2 - pos1;

			vec.y = 0;
			vec2.y = 0;

			//エネミーの分散
			if (vec.Size() <= E_SIZE)
			{
				itr1->AddPos(vec.Normalize());
				itr2->AddPos(vec2.Normalize());
			}
		}
	}
}

void TrashEnemyGroup::InCameraWayPoint(WayPoint& _wayPoint)
{
	VECTOR3 position = camera->GetCameraTransform()->position;
	position.y = 0;
	//正面べく
	VECTOR3 frontVec = VECTOR3(0, 0, 1) * MGetRotY(camera->GetCameraTransform()->rotation.y);
	VECTOR3 vec = _wayPoint.position - position;

	//内積
	float dotProduct = VDot(frontVec, vec.Normalize());
	//カメラに写ってるか
	if (dotProduct > cosf(45 * DegToRad))
		_wayPoint.active = true;
	//カメラに写ってなかったら
	else
		_wayPoint.active = false;
}

int TrashEnemyGroup::GetActiveEnemy()
{
	int _counter = 0;
	float* _hp = 0;
	for (auto& itr : meleeEnemies)
	{
		if (itr->GetHp() > _hp)//Activeでやると死んでるモーション挟んでる敵もカウントされるため
			_counter++;
	}
	for (auto& itr : rangedEnemies)
	{
		if (itr->GetHp() > _hp)
			_counter++;
	}

	return _counter;
}

void TrashEnemyGroup::MeleeEnemyAttack(TrashEnemy* _enemy)
{
	if (_enemy->IsCooperateAtk() || !_enemy->IsAttack())
		return;

	attackCounter+=Time::DeltaTimeRate();

	if (attackCounter >= ATK_COUNTER_MIN + maxAttackCounter)
	{
		if (_enemy->IsAttack())
		{
			_enemy->AttackCommand();
			attackCounter = 0;
			maxAttackCounter = ATK_COUNTER_MAX * (float)Random::GetReal();
		}
	}
	else if (attackCounter >= ATK_COUNTER_MAX)
		_enemy->AttackCoolTimeReset();
}

void TrashEnemyGroup::EnemiesRun(TrashEnemy* _enemy)
{
	if (!_enemy->GetIsRunState())
		return;
	
	VECTOR3 _pos = VZero;

	if (_enemy->GetPointNumber() != 0)
		_pos = trashEnemyManager->GetWayPointPosition()[_enemy->GetPointNumber() - 1];
	else
		_pos = trashEnemyManager->GetPlayerPos();

	_enemy->SetWayPoint(_pos);
}

void TrashEnemyGroup::CooperateAttackMove(TrashEnemy* _enemy)
{
	if (!_enemy->IsCooperateAtk())
		return;
	int enemiesMax = (int)meleeEnemies.size();

	//連携攻撃のときにその敵が準備完了したかどうか
	if (_enemy->GetStandby())
		standbyCounter++;
	if (standbyCounter >= 1)
		cooperateCounter += Time::DeltaTimeRate();

	//敵全員が準備完了するか時間経過で攻撃に移る
	if (standbyCounter == enemiesMax || cooperateCounter >= 3)
	{
		AllChangeMeleeState(StateID::T_ENEMY_RUN_S);
		standbyCounter = 0;
		cooperateCounter = 0;
	}
}

void TrashEnemyGroup::AllChangeMeleeState(StateID::State_ID _id)
{
	if (GetActiveEnemy() == 0)
		return;

	for (auto& itr : meleeEnemies)
	{
		if (itr->IsMovingToPlayer())
			itr->ChangeState(_id);
		else
			itr->CooperateAtkFinish();
	}
}

void TrashEnemyGroup::CloseWayPoint(std::vector<WayPoint> wayPoint)
{
	VECTOR3 position = camera->GetCameraTransform()->position;
	position.y = 0;
	//正面べく
	VECTOR3 frontVec = VECTOR3(0, 0, 1) * MGetRotY(camera->GetCameraTransform()->rotation.y);
	float counter = 0;
	VECTOR3 savePos = INFINITY;
	for (auto enemy : meleeEnemies)
	{
		for (auto& itr : wayPoint)
		{
			//使っていいウェイポイントを一回だけ探す
			if (counter == 0)
			{
				InCameraWayPoint(itr);
				//VECTOR3 vec = itr.position - position;
				////内積
				//float dotProduct = VDot(frontVec, vec.Normalize());
				////カメラに写ってるか
				//if (dotProduct > cosf(45 * DegToRad))
				//	itr.active = true;
				////カメラに写ってなかったら
				//else
				//	itr.active = false;
			}
			//一番近いウェイポイントを探す
			if (itr.active)
			{
				VECTOR3 vec = itr.position - enemy->GetPos();
				if (savePos.Size() > vec.Size())
					savePos = itr.position;
			}
		}
		enemy->SetCooperateWayPoint(savePos, StateID::COOPERATEATTACK1);
		counter = 1;
	}
}

void TrashEnemyGroup::RangedEnemyAttack(TrashEnemy* _enemy)
{
	if (_enemy->GetEnemyType() == _enemy->EnemyType::RANGED)
	{

	}
	else
	{
		const VECTOR3 cameraPos = camera->GetCameraTransform()->position;
		const float camRotY = camera->GetCameraTransform()->rotation.y;
		const VECTOR3 forward = VECTOR3(sinf(camRotY), 0, cosf(camRotY));
		const float distance = 5000;

		VECTOR3 _wayPointPos = cameraPos + forward * distance;

		_wayPointPos.y = 0;
		
		_enemy->SetWayPoint(_wayPointPos);

		leaderPos = _enemy->GetPos();
	}
	
	
}
