#pragma once
#include "../../TrashEnemy/EnemyState/EnemyStateBase.h"

class Fead;
class UIManager;

class BossAppear : public EnemyStateBase
{
public:
	BossAppear();
	~BossAppear();

	void Update()override;
	void Draw() override;

	void Start()override;
	void Finish()override;

private:

	Fead* fead;
	bool feedInFinish;
	bool roar;
	bool first;
	UIManager* uiManager;
};