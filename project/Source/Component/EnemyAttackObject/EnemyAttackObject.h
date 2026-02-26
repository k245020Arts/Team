#pragma once
#include "../Component.h"

class EnemyAttackObject : public Component
{
public:
	EnemyAttackObject();
	virtual ~EnemyAttackObject();

	void Update()override;
	void Draw()override;

private:

};
