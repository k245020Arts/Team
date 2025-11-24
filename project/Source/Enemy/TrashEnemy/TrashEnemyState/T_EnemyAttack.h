#pragma once
#include "../EnemyState/EnemyStateBase.h"

class T_EnemyAttack :public EnemyStateBase
{
public:
	T_EnemyAttack();
	~T_EnemyAttack();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:

};