#pragma once
#include "../../TrashEnemy/EnemyState/EnemyStateBase.h"
#include "../Boss.h"
#include "Attack/BossAttackBase.h"

struct ActionParam
{
	std::string id;
	bool attackState; // 攻撃のStateか
	int priority;     // プライオリティ
	int weight;       // 重さ
	int maxAction;    // 連続で何回行動できるか

	float distance = 0; // 距離によってその技が出やすいかどうか
	int addWeight = 0;  // 数字変動

	ActionParam() {
		id = "";
		attackState = false;
		priority = 1;
		weight = 0;
		maxAction = 1;
		distance = 0;
		addWeight = 0;
	}
};

// to_json
inline void to_json(JSON& j, const ActionParam& p)
{
	j = JSON{
		{"id", p.id},
		{"attackState", p.attackState},
		{"priority", p.priority},
		{"weight", p.weight},
		{"maxAction", p.maxAction},
		{"distance", p.distance},
		{"addWeight", p.addWeight}
	};
}

// from_json
inline void from_json(const JSON& j, ActionParam& p)
{
	j.at("id").get_to(p.id);
	j.at("attackState").get_to(p.attackState);
	j.at("priority").get_to(p.priority);
	j.at("weight").get_to(p.weight);
	j.at("maxAction").get_to(p.maxAction);

	// optional扱い（デフォルト値あり）
	if (j.contains("distance"))
		j.at("distance").get_to(p.distance);

	if (j.contains("addWeight"))
		j.at("addWeight").get_to(p.addWeight);
}


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
	std::shared_ptr<BossAttackBase> GetNowAttackState();

	void SaveSorthing(std::string _bossName);
	void LoadSorting(std::string _bossName);

	std::vector<ActionParam> GetActionParam();
	std::unordered_map<std::string, BossAttackBase::BossAttackParam> GetAttackParam();

	void AddAttack(BossAttackBase::BossAttackParam _param, Boss* _boss);
	void AddAttack(BossAttackBase::BossAttackParam _param,Boss* _boss,std::string _attackID);

	void ReloadParam(BossAttackBase::BossAttackParam _param,std::string _reLoadID);

	//void StateImguiDraw()override;

private:
	const float COOLTIME = 0.5f;

	void NormalAttackSelect();
	void AttackStart();

	
	
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
	std::unordered_map<std::string, std::shared_ptr<BossAttackBase>> attacks;
	std::unordered_map<std::string, BossAttackBase::BossAttackParam> attackParam;
};