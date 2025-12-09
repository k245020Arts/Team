#pragma once
#include "../EnemyState/EnemyStateBase.h"

class T_EnemyDead :public EnemyStateBase
{
public:
	T_EnemyDead();
	~T_EnemyDead();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;
private:
	int counter;
};