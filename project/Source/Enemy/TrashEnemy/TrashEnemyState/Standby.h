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
	void RotateMove(int _index);
	float CalculateAngle();
	const float BACKSPEED = 60.0f;

	VECTOR3 vec;
	VECTOR3 pPos;

	int aiMove;
	int counter;
	float angle;
	float range;
	float randomSpeed;
	bool isBach;

};