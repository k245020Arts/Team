#pragma once
#include "stateBase.h"
#include "Debug.h"
#include "ID.h"
#include "transform.h"
#include "function.h"

class PlayerStateBase : public StateBase
{
public:
	PlayerStateBase();
	~PlayerStateBase();
	void Update() override;
	void Draw()override;

	virtual void Start()override;

	void AvoidStart()override;
	void AttackCollsion();

	void DefalutWalk();

protected:
	ID::IDType animId;
	bool noStateChange;
	bool firstColl;
	Transform collTrans;
	bool nextAttack;
};