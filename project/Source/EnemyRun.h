#pragma once
#include "EnemyStateBase.h"

class EnemyRun : public EnemyStateBase
{
public:
	EnemyRun();
	~EnemyRun();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:

	VECTOR3 dir;
};