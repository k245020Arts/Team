#pragma once

class T_EnemyStatus
{
private:
	struct Status
	{
		const float normalAttack1 = 20.0f;	//攻撃１のダメージ数値
		
		const float maxHp = 100;			//最大HP
		const float coolTime = 30.0f;		//一段目の攻撃までの時間
		const float runSpeed = 5.0f;		//移動速度
		//float fallFrame = 50.0f;			//次の攻撃の間隔を短くする
		const float range = 1400.0f;		//敵が攻撃するまでのプレイヤーとの距離
		const float chaseRange = 5000.0f;	//プレイヤーと離れたときに切り替わる
	};
	Status status;

public:
	T_EnemyStatus() { ; }
	Status GetStatus() { return status; }
};