#pragma once
#include "../../TrashEnemy/EnemyState/EnemyStateBase.h"
#include "../Boss.h"

class AttackSorting :public EnemyStateBase
{
public:

	AttackSorting();
	~AttackSorting();
	void Update()override;

	void Start()override;
	void Finish()override;

private:
	const float COOLTIME = 0.5f;

	void NormalAttackSelect();
	//int AttackPriority();
	
	/// <summary>
	/// 行動を決める
	/// </summary>
	/// <param name="_priority">今のボスの状況を入れる</param>
	void BuildTable(int _priority);
	//変動させた値をすべてゼロにする
	void AllAddWeightZero();

	float coolTime;

	Boss::HP_RATE hp;

	bool jump;
	int kind;
	int attackNum;

	/*struct ActionRange
	{
		int min;
		int max;
		StateID::State_ID id;
		ActionRange();
	};
	std::vector<ActionRange> table;*/

	int bossPriority;//
	StateID::State_ID nextState;
	StateID::State_ID copyState;

	int copyPriority;
	int moveCounter;

	VECTOR3 vec;
};