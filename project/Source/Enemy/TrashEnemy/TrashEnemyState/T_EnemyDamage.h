#pragma once
#include "../EnemyState/EnemyStateBase.h"

class TrashEnemy;

class T_EnemyDamage :public EnemyStateBase
{
public:
	T_EnemyDamage();
	~T_EnemyDamage();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;
protected:
	float counter;
	//吹き飛ぶときの処理
	void KnockbackMove(TrashEnemy* _e, float a, float h, float k);
	/// <summary>
	/// 吹き飛びの軌道
	/// </summary>
	/// <param name="a">落下の強さ（重力）</param>
	/// <param name="h">最高点までの時間</param>
	/// <param name="k">吹っ飛びの高さ</param>
	/// <returns></returns>
	float Orbit(float a, float h, float k);
	float motionSpeed;
private:
	
};