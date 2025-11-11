#pragma once
#include "stateBase.h"
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

	void AttackSound();
	void AttackFlash(ID::IDType _modelId, int _modelFrame, std::string _voice);
	void Trail();
	void BossTrail(bool _right);

protected:
	ID::IDType animId;
	bool firstColl;
	Transform collTrans;
	float attackTime;
	bool sound;

	float a;
};