#pragma once
#include "../EnemyState/EnemyStateBase.h"

class TrashEnemy;

class T_EnemyStaySky : public EnemyStateBase
{
public:
	T_EnemyStaySky();
	~T_EnemyStaySky();

	void Update()override;

	void Start()override;
	void Finish()override;

private:
	//ƒŠ[ƒ_[‚Ì“®‚«
	void LeaderMove(TrashEnemy* _enemy);
	//•’Ê‚Ì“G‚Ìˆ—
	void RangedMove(TrashEnemy* _enemy);

	bool isLeader;

	VECTOR3 setGravity;
};