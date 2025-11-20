#pragma once
#include "../Component.h"

class EnemyAttackObject : public Component
{
public:
	EnemyAttackObject();
	~EnemyAttackObject();

	void Update()override;
	void Draw()override;

private:

};
