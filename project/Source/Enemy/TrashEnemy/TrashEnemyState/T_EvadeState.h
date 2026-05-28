#pragma once
#include "../EnemyState/EnemyStateBase.h"

class T_EvadeState :public EnemyStateBase
{
public:
	T_EvadeState();
	~T_EvadeState();

	void Update()override;
	void Start()override;
	void Finish()override;
private:
};