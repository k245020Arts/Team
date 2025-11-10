#pragma once
#include "EnemyStateBase.h"

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
