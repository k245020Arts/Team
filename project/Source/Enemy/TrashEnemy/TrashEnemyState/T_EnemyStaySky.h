#pragma once
#include "../EnemyState/EnemyStateBase.h"

class T_EnemyStaySky : public EnemyStateBase
{
public:
	T_EnemyStaySky();
	~T_EnemyStaySky();

	void Update()override;

	void Start()override;
	void Finish()override;

private:
	void LeaderMove(TrashEnemy* _enemy);
	void RangedMove(TrashEnemy* _enemy);

	bool isLeader;
};