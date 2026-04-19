#pragma once
#include "../EnemyState/EnemyStateBase.h"

class TrashEnemy;

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
	void ReaderMove(TrashEnemy* _enemy);
	void NormalMove(TrashEnemy* _enemy);

	VECTOR3 targetPos;
	bool isLeader;
	float attackCounter;
};