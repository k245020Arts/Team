#pragma once
#include "stateBase.h"
#include "enemyInformation.h"
#include "ID.h"
#include "transform.h"

class EnemyStateBase : public StateBase
{
public:
	EnemyStateBase();
	~EnemyStateBase();

	void Start()override;
	void AttackCollsion();
	void BossAttackCollsion();

	ID::IDType GetAnimId() { return animId; }

protected:
	ID::IDType animId;
	bool firstColl;
	Transform collTrans;
};