#pragma once
#include "../../../State/StateBase.h"
#include "../../../Common/ID/ID.h"
#include "../../../Component/Transform/Transform.h"

class EnemyStateBase : public StateBase
{
public:
	EnemyStateBase();
	~EnemyStateBase();

	void Update()override;

	void Start()override;
	void AttackCollsion();
	void BossAttackCollsion();

	ID::IDType GetAnimId() { return animId; }

	void AttackSound();
	void AttackFlash(ID::IDType _modelId, int _modelFrame, std::string _voice);
	void AttackBeforeFrash(ID::IDType _modelId, int _modelFrame, std::string _voice);
	void Trail();
	void BossTrail(bool _right);

protected:
	ID::IDType animId;
	bool firstColl;
	Transform collTrans;
	float attackTime;
	bool sound;

	float fallFrame;
	float animSlowCounter;
	float keepAnimSpeed;
	bool slowMode;
};