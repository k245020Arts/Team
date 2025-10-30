#pragma once

class BossStatus
{
private:
	struct Status
	{
		float normalAttack1 = 50.0f;	//攻撃１のダメージ数値
		float normalAttack2 = 50.0f;	//攻撃２のダメージ数値
		float normalAttack3 = 50.0f;	//攻撃３のダメージ数値

		int maxAttack = 3;				//最大連続攻撃の数
		float maxHp = 1000;				//最大HP
		float coolTime = 60.0f;			//一段目の攻撃までの時間
	};
	Status status;

public:
	BossStatus() { ; }
	Status GetStatus() { return status; }
};