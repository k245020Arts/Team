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
	/// <summary>
	/// 吹き飛ぶときの処理
	/// </summary>
	/// <param name="speed">吹っ飛びの速度</param>
	/// <param name="a">落下の強さ（重力）</param>
	/// <param name="h">最高点までの時間</param>
	/// <param name="k">吹っ飛びの高さ</param>
	/// <returns></returns>
	void KnockbackMove(TrashEnemy* _e, float _speed, float a, float h, float k);
	//吹き飛びの軌道
	float Orbit(float a, float h, float k);
	float motionSpeed;
	//情報を一回だけ取得する
	bool isGetInformation;
	//右ベクトル
	VECTOR3 rightVec;
	//左右判定
	float side;
private:
	
};