#pragma once
#include "../../Enemy/EnemyState/EnemyStateBase.h"

class BossIdol : public EnemyStateBase
{
public:
	BossIdol();
	~BossIdol();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:

};
