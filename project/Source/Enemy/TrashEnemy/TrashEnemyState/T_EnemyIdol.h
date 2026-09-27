#pragma once
#include "../../EnemyState/EnemyStateBase.h"

class TrashEnemy;

class T_EnemyIdol :public EnemyStateBase
{
public:
	T_EnemyIdol();
	~T_EnemyIdol();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;
private:
	void NormalMove();
	//プレイヤーに少しずつ近づく処理
	void PlayerCloser(TrashEnemy* _enemy);

	const float RangeSpeed = 60.0f;
	const float Gravity = -1000.0f;
	const int RandMax = 500;

	//近づく時のモーション速度の最大値
	const float WalkAnimSpeedMax = 2.0f;
	//近づく時のモーション速度の最小値
	const float WalkAnimSpeedMin = 0.1f;
	//プレイヤーの探知範囲を少しずつあげる
	float detectionRange;

	VECTOR3 setGravity;

	int counter;

	int animCounter;
};