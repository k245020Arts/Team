#pragma once
#include "../EnemyState/EnemyStateBase.h"

class Standby :public EnemyStateBase
{
public:
	Standby();
	~Standby();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;
private:
	int counter;
};