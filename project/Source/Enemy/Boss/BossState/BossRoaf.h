#pragma once
#include "../../TrashEnemy/EnemyState/EnemyStateBase.h"

class BossRoaf : public EnemyStateBase
{
public:
	BossRoaf();
	~BossRoaf();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:

};
