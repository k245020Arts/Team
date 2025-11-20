#pragma once
#include "../EnemyAttackObject.h"

class EffectManager;

class ShockWave : public EnemyAttackObject
{
public:
	ShockWave();
	~ShockWave();

	void Update()override;
	void Draw()override;

private:

};
