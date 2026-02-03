#pragma once
#include "T_EnemyAttack.h"

class CooperateAttack2 :public T_EnemyAttack
{
public:
	CooperateAttack2();
	~CooperateAttack2();

	void Update()override;

	void Start()override;
	void Finish()override;
private:

};