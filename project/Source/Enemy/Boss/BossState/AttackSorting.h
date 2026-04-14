#pragma once
#include "../../TrashEnemy/EnemyState/EnemyStateBase.h"
#include "../Boss.h"

class BossAttackBase;

class AttackSorting :public EnemyStateBase
{
public:

	AttackSorting();
	~AttackSorting();
	void Update()override;

	void Start()override;
	void Finish()override;

	void ForcedAttackStart(std::string _attackID);

	void Load(std::string _bossName, Boss* _boss);
	void AttackFinish();
	BossAttackBase* GetNowAttackState();

private:
	const float COOLTIME = 0.5f;

	void NormalAttackSelect();
	void AttackStart();

	void Save(std::string _bossName);
	void LoadSorting(std::string _bossName);
	
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
	bool nextAttack;

	/*struct ActionRange
	{
		int min;
		int max;
		StateID::State_ID id;
		ActionRange();
	};
	std::vector<ActionRange> table;*/

	int bossPriority;//
	std::string nextState;
	std::string copyState;

	int copyPriority;
	int moveCounter;
	bool forceAttack;

	VECTOR3 vec;
	std::unordered_map<std::string, BossAttackBase*> attacks;
};