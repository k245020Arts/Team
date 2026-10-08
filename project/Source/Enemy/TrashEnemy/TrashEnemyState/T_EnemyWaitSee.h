#pragma once
#include "../../EnemyState/EnemyStateBase.h"

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
	void Move(TrashEnemy* _enemy);
	void FloatingMove(TrashEnemy* _enemy);

	const float LeaderRange = 400.0f;
	const float RangedRange = 200.0f;
	//敵の浮いている位置の平均値
	const float AveragePositionY = 1200.0f;
	//浮いてるように見せるための誤差の数値
	const float DeviationPositionY = 200.0f;


	VECTOR3 targetPos;
	bool isLeader;
	float attackCounter;

	float moveSpeed;
	const float LeaderMoveSpeed = 10.0f;
	const float NormalMoveSpeed = 6.0f;

	float pointRange;
	VECTOR3 setGravity;
	//浮くときに使うカウンター
	float floatingTimeCounter;
};