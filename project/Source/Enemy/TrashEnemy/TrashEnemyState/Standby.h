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
	void RotateMove();
	void InCameraView();
	//float CalculateAngle();
	const float BACKSPEED = 60.0f;

	VECTOR3 vec;
	VECTOR3 pPos;
	//どこに移動するかわかる
	VECTOR3 newPos;

	float counter;
	float range;
	float randomSpeed;
	float redefinitionCounter;
	//次のポジションを再定義する
	bool isRedefinition;

	float runTime;
};