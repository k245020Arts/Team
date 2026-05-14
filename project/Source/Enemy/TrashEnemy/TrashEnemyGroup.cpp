#include "TrashEnemyGroup.h"
#include "TrashEnemy.h"
#include "../../Common/Random.h"
#include "../../Camera/Camera.h"
#include "../../Common/Effect/EffectManager.h"

TrashEnemyGroup::TrashEnemyGroup()
{
	camera = FindGameObjectWithTag<Object3D>("CAMERA_OBJ")->Component()->GetComponent<Camera>();
	trashEnemyManager = FindGameObject<TrashEnemyManager>();

	hasLeader = false;
	attackCounter = 0;

	enemiesRunCounter = 0;

	leaderPos = VZero;

	rangedDamageMove = false;

	rangedAtkCounter = 0;

	rangedJoinCounter = 0;

	startRangedAtk = false;
}

TrashEnemyGroup::~TrashEnemyGroup()
{
}

void TrashEnemyGroup::Update()
{
	EnemyDead(meleeEnemies);
	EnemyDead(rangedEnemies);
	Separation();
	
	//近距離の敵関連
	for (auto melee : meleeEnemies)
	{
		EnemiesRun(melee);
		MeleeEnemyAttack(melee);
		CooperateAttackMove(melee);
	}
	//遠距離の敵関連
	for (auto ranged : rangedEnemies)
	{
		RangedEnemySetWaypoint(ranged);
	}

	if (rangedDamageMove)
		RangedDamageMove();
}

void TrashEnemyGroup::Draw()
{
	CooperateAttackLine();
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

void TrashEnemyGroup::EnemyDead(std::list<TrashEnemy*>& enemies)
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
	VECTOR pos1 = { 0,0,0 };
	VECTOR pos2 = { 0,0,0 };
	const float E_SIZE = 600;

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

int TrashEnemyGroup::GetActiveEnemy()const
{
	return GetMeleeActiveEnemy() + GetRangedActiveEnemy();
}

int TrashEnemyGroup::GetMeleeActiveEnemy()const
{
	int _counter = 0;
	float* _hp = 0;

	for (auto& itr : meleeEnemies)
	{
		if (itr->GetHp() > _hp)//Activeでやると死んでるモーション挟んでる敵もカウントされるため
			_counter++;
	}
	return _counter;
}

int TrashEnemyGroup::GetRangedActiveEnemy()const
{
	int _counter = 0;
	float* _hp = 0;

	for (auto& itr : rangedEnemies)
	{
		if (itr->GetHp() > _hp)
			_counter++;
	}

	return _counter;
}

void TrashEnemyGroup::MeleeEnemyAttack(TrashEnemy* _enemy)
{
	if (_enemy->IsCooperateAtk() || !_enemy->IsAttack() || startRangedAtk)
		return;

	attackCounter += Time::DeltaTimeRate();

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

	if (_enemy->GetCAttack())
		AllChangeMeleeState(StateID::COOPERATEATTACK1);
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

void TrashEnemyGroup::CooperateAttackLine()
{
	if (meleeEnemies.size() <= 0)
		return;

	std::vector<VECTOR3> copyPos;
	const float PosY = 300.0f;

	for (auto& itr : meleeEnemies)
	{
		if (!itr->IsCooperateAtk())
			return;

		copyPos.push_back(VECTOR3(itr->GetPos().x, PosY, itr->GetPos().z));
	}

	const float Counter = copyPos.size() - 1;

	for (int i = 0; i < Counter; i++)
	{
		DrawCapsule3D(copyPos[i], copyPos[i + 1] , 20, 0, GetColor(255, 0, 0), GetColor(255, 0, 0), true);
	}

	copyPos.clear();
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

void TrashEnemyGroup::RangedEnemyAttack()
{
	float pointCounter = 0.0f;
	const float MaxPoint = 8.0f;
	const float Range = 700.0f;
	const float MaxAttackCounter = 2.5f;
	bool leaderActiveEnd = false;

	/*
		リーダーが死んだ後の振り分け
		チャージ攻撃で打ち返し
		演出寄りで制作する
		細かいとこ調整して気持ちよさをだせるようにする
	*/

	for (auto& enemy : rangedEnemies)
	{
		if (enemy->GetDeadMove()|| enemy->IsPlayerSpecialMove())
			return;

		if (rangedJoinCounter == 0)//リーダー以外の敵を数える
			rangedJoinCounter = (int)rangedEnemies.size() - 1;//リーダーをのぞくため

		if (enemy->GetEnemyType() == enemy->EnemyType::RANGED_LEADER)
		{
			//リーダーが飛ぶ処理
			enemy->ChangeState(StateID::T_ENEMY_STAYSKY);

			startRangedAtk = true;

			if (enemy->GetStandby())//リーダーが他の奴に指示を出す
				leaderActiveEnd = true;

			leaderPos = enemy->GetPos();

			if (rangedJoinCounter <= rangedAtkCounter)//敵全員が攻撃を終えた後の処理
			{
				if (rangedAtkTime >= MaxAttackCounter)
				{
					rangedAtkTime = 0;
					AllChangeRangedState(StateID::T_ENEMY_WAITSEE);
					rangedJoinCounter = 0;
					rangedAtkCounter = 0;
					startRangedAtk = false;
					FindGameObject<TrashEnemyManager>()->SetStartRangedAttack(false);
				}
			}
		}
		else
		{
			if (!leaderActiveEnd)
				return;
			//リーダーの周りにポイント配置
			if (pointCounter < MaxPoint && !enemy->GetStandby() && !enemy->IsMovingToPlayer())
			{
				//均等に割って円形に配置
				float angle = (2.0f * DX_PI_F) * pointCounter / MaxPoint;

				//回転を反映した方向
				VECTOR3 rotatedDir = VECTOR3(cosf(angle), 0, sinf(angle));
				//リーダーからの絶対座標
				VECTOR3 target = leaderPos + rotatedDir * Range;
				//指定したポイントを渡す
				enemy->SetCooperateWayPoint(target);
				enemy->ChangeState(StateID::T_ENEMY_STAYSKY);

				pointCounter++;
			}

			//敵がダメージをくらった後の処理
			if (enemy->GetCooperateDamageMove())
			{
				const VECTOR3 enemyPos = enemy->GetPos();
				//リーダーとの距離で近くなったらの処理
				const float LeaderVecMax = 1200.0f;
				if (VSize(leaderPos - enemyPos) <= LeaderVecMax)
				{
					rangedDamageMove = true;
					enemy->ChangeHp(-enemy->MaxHp());
					EffectManager::GetInstance()
						->CreateEffekseer(*enemy->GetEnemyObj()->GetTransform(), nullptr, Effect_ID::ROCK_BLAST, 3.0f);
				}
				return;
			}
			//敵がプレイヤーに攻撃する処理
			if (rangedAtkTime >= MaxAttackCounter && !enemy->IsMovingToPlayer())
			{
				enemy->SetLeaderPos(leaderPos);
				enemy->RangedAttack();
				rangedAtkTime = 0;
				rangedAtkCounter++;
			}
		}
	}
	rangedAtkTime += Time::DeltaTimeRate();
}

void TrashEnemyGroup::DeadMeleeEnemy()
{
	for (auto& itr : meleeEnemies)
	{
		itr->ChangeHp(-itr->GetMaxHp());
	}
}

void TrashEnemyGroup::RangedEnemySetWaypoint(TrashEnemy* _enemy)
{
	if (_enemy->GetEnemyType() == _enemy->EnemyType::RANGED)
	{
		leaderPos.y = 0.0f;
		_enemy->SetWayPoint(leaderPos);
	}
	else
	{
		const VECTOR3 CameraPos = camera->GetCameraTransform()->position;
		const float CamRotY = camera->GetCameraTransform()->rotation.y;
		const VECTOR3 Forward = VECTOR3(sinf(CamRotY), 0, cosf(CamRotY));
		const float Distance = 5000;

		VECTOR3 _wayPointPos = CameraPos + Forward * Distance;

		_wayPointPos.y = 0;
		
		_enemy->SetWayPoint(_wayPointPos);

		leaderPos = _enemy->GetPos();
	}	
}

void TrashEnemyGroup::RangedDamageMove()
{
	for (auto& enemy : rangedEnemies)
	{
		const float Damage = -enemy->MaxHp();
		if (enemy->GetEnemyType() == enemy->EnemyType::RANGED_LEADER)
		{
			enemy->ChangeHp(Damage);
		}
		else
		{
			if(!enemy->IsMovingToPlayer())
				enemy->ChangeHp(Damage);
		}
	}
}

void TrashEnemyGroup::AllChangeRangedState(StateID::State_ID _id)
{
	for (auto& enemy : rangedEnemies)
		enemy->ChangeState(_id);
}
