#pragma once
#include "../../EnemyState/EnemyStateBase.h"
#include "../Boss.h"
#include "Attack/BossAttackJsonParse.h"
#include "Attack/BossAttack.h"

struct AttackSelectData
{
	float priority;
	float normalParam;
	float comboParam;
	AttackSelectData()
	{
		priority = 0;
		normalParam = 0;
		comboParam = 0;
	}
};

struct ActionParam
{
	std::string id;
	bool attackState; // 攻撃のStateか
	int priority;     // プライオリティ
	int weight;       // 重さ
	int maxAction;    // 連続で何回行動できるか
	float distance = 0; // 距離によってその技が出やすいかどうか

	int addWeight = 0;  // 数字変動

	ActionParam() 
	{
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

inline void from_json(const JSON& j, AttackSelectData& data)
{
	j.at("priority").get_to(data.priority);
	j.at("normal").get_to(data.normalParam);
	j.at("combo").get_to(data.comboParam);
}

class AttackSorting :public EnemyStateBase
{
public:

	AttackSorting();
	~AttackSorting();
	void Update()override;

	void Start()override;
	void Finish()override;
	/// <summary>
	/// 強制的に次の攻撃のStateの決定をする
	/// </summary>
	/// <param name="_attackID"></param>
	void ForcedAttackStart(const std::string& _attackID);

	/// <summary>
	/// 攻撃情報のロードをする
	/// </summary>
	/// <param name="_bossName">ボスのID</param>
	/// <param name="_boss">ボスのポインタ</param>
	void Load(const std::string& _bossName, Boss* _boss);
	/// <summary>
	/// 攻撃の終了
	/// </summary>
	void AttackFinish();
	/// <summary>
	/// 今の攻撃状態を記録
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<EnemyAttackBase> GetNowAttackState();
	/// <summary>
	/// 攻撃抽選情報のセーブ
	/// </summary>
	/// <param name="_bossName">ボスのID</param>
	void SaveSorthing(const std::string& _bossName);
	/// <summary>
	/// 攻撃抽選情報のロード
	/// </summary>
	/// <param name="_bossName">ボスのID</param>
	void LoadSorting(const std::string& _bossName);
	/// <summary>
	/// 攻撃抽選情報の取得
	/// </summary>
	/// <returns></returns>
	const std::vector<ActionParam> GetActionParam()const;
	/// <summary>
	/// 攻撃情報の取得
	/// </summary>
	/// <returns></returns>
	const std::unordered_map<std::string, EnemyAttackBase::BossAttackParam> GetAttackParam()const;
	/// <summary>
	/// 攻撃の追加
	/// </summary>
	/// <param name="_param">攻撃情報</param>
	/// <param name="_boss">ボスのポインタ</param>
	void AddAttack(const EnemyAttackBase::BossAttackParam& _param, Boss* _boss);
	/// <summary>
	/// 攻撃の追加
	/// </summary>
	/// <param name="_param">攻撃情報</param>
	/// <param name="_boss">ボスのポインタ</param>
	/// <param name="_attackID">攻撃のID</param>
	void AddAttack(const EnemyAttackBase::BossAttackParam& _param,Boss* _boss, const std::string& _attackID);
	/// <summary>
	/// 攻撃情報のリロードをする
	/// </summary>
	/// <param name="_param"></param>
	/// <param name="_reLoadID"></param>
	void ReloadParam(const EnemyAttackBase::BossAttackParam& _param, const std::string& _reLoadID);

	//void StateImguiDraw()override;

private:
	const float COOLTIME = 0.5f;
	void LoodAttackSelect(const std::string& _fileName);

	void NormalAttackSelect();
	void AttackStart();
		
	/// <summary>
	/// 行動を決める
	/// </summary>
	/// <param name="_priority">今のボスの状況を入れる</param>
	void BuildTable(int _priority);
	//コンボ関数
	void SelectNextComboAction(int _priority);
	//攻撃を選択する関数
	void SelectNextAction(int _priority);

	//変動させた値をすべてゼロにする
	void AllAddWeightZero();

	//攻撃振り分けパラメーターの設定
	std::vector<ActionParam> actions;
	//コンボか通常の数値のデータを保存する
	std::vector<AttackSelectData> selectData;

	float coolTime;

	Boss::HP_RATE hp;

	bool jump;
	int kind;
	int attackNum;
	bool nextAttack;

	int bossPriority;//
	std::string nextState;
	std::string copyState;

	int copyPriority;
	int moveCounter;
	bool forceAttack;

	VECTOR3 pVec;
	std::unordered_map<std::string, std::shared_ptr<BossAttack>> attacks; //攻撃のポインターの保持
	std::unordered_map<std::string, EnemyAttackBase::BossAttackParam> attackParam; ///攻撃のパラメーターの保持
};