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
	const float COOLTIME = 30.0f;

	void NormalAttackSelect();
	//int AttackPriority();
	
	/// <summary>
	/// s“®‚ğŒˆ‚ß‚é
	/// </summary>
	/// <param name="_priority">¡‚Ìƒ{ƒX‚Ìó‹µ‚ğ“ü‚ê‚é</param>
	void BuildTable(int _priority);

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
};