#pragma once
#include "../EnemyState/EnemyStateBase.h"

class T_EnemyWaitSee : public EnemyStateBase
{
public:
	T_EnemyWaitSee();
	~T_EnemyWaitSee();

	void Update()override;

	void Start()override;
	void Finish()override;

	bool IsLeader() { return isLeader; }
private:
	VECTOR3 targetPos;
	bool isLeader;
	float attackCounter;
};