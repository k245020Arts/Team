#pragma once
#include "../EnemyState/EnemyStateBase.h"

class T_EnemyJump :public EnemyStateBase
{
public:
	T_EnemyJump();
	~T_EnemyJump();

	void Update()override;

	void Start()override;
	void Finish()override;

private:
	VECTOR3 targetPos;
};