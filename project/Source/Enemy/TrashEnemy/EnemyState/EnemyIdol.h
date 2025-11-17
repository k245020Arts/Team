#pragma once
#include "EnemyStateBase.h"

class EnemyIdol : public EnemyStateBase
{
public:
	EnemyIdol();
	~EnemyIdol();
	void Update()override;
	void Draw()override;

	void Start()override;

private:
	float timer;
};