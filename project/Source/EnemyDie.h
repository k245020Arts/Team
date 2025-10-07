#pragma once
#include "EnemyStateBase.h"

class EnemyDie : public EnemyStateBase
{
public:
	EnemyDie();
	~EnemyDie();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;


private:
	float slowTime;
};