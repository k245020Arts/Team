#pragma once
#include "CharaBase.h"
#include "enemyInformation.h"

class EnemyBase : public CharaBase
{
public:
	friend class EnemyStateBase;
	EnemyBase();
	~EnemyBase();

	void Update()override;
	void Draw()override;


protected:

	EnemyInformation::EnemyComponent enemyBaseComponent;
	float damageFlash;

};
