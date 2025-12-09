#pragma once

class T_EnemyStatus
{
private:
	struct Status
	{
		const float normalAttack1 = 20.0f;	//攻撃１のダメージ数値
		
		const float maxHp = 100;			//最大HP
		const float coolTime = 30.0f;		//一段目の攻撃までの時間
		const float runSpeed = 6.0f;		//移動速度
		const float cooperateSoeed = 15.0f;	//連携攻撃時のrunスピード
		const float range = 1400.0f;		//idelとrunの切り替わり
		const float atkRang = 500.0f;		//runと攻撃の切り替わり
		const float chaseRange = 5000.0f;	//プレイヤーと離れたときに切り替わる
	};
	Status status;

public:
	T_EnemyStatus() { ; }
	Status GetStatus() { return status; }
};