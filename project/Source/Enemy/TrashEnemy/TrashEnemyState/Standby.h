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
	void NormalMove();
	void RotateMove(float index);
	VECTOR3 vec;
	VECTOR3 velocity;
	VECTOR3 pPos;
	float targetDirection;

	int aiMove;
	bool acquisition;
	int counter;
};