#pragma once
#include "../EnemyState/EnemyStateBase.h"

class T_EnemyRun :public EnemyStateBase
{
public:
	T_EnemyRun();
	~T_EnemyRun();
	void Update()override;
	
	void Start()override;
	void Finish()override;

private:
	void Look();
	void Move(float _speed, float _max);
	VECTOR3 rotation;
	VECTOR3 velocity;
	VECTOR3 targetPos;
};
