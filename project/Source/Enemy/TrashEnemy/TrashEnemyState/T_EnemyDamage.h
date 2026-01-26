#pragma once
#include "../EnemyState/EnemyStateBase.h"

class T_EnemyDamage :public EnemyStateBase
{
public:
	T_EnemyDamage();
	~T_EnemyDamage();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;
private:

};