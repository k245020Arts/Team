#pragma once
#include "EnemyStateBase.h"

class SphereCollider;

class EnemyAttack1 : public EnemyStateBase
{
public:
	EnemyAttack1();
	~EnemyAttack1();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

	float GetHitDamage() { return hitDamage; }

private:
	float animStopCounter;
	bool sound;
	float effectAverageSpeed;
	float hitDamage;
};