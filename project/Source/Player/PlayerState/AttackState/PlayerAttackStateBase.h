#pragma once
#include "../PlayerStateBase.h"
#include "../../../Common/JsonReader.h"

class PlayerAttackStateBase : public PlayerStateBase
{
public:

	PlayerAttackStateBase();
	virtual ~PlayerAttackStateBase();

	virtual void Update()override;

	/// <summary>
	/// Enemyの方向に向く
	/// </summary>
	virtual void EnemyRotation();
	virtual void Start()override;
	virtual void Finish() override;
	/// <summary>
	/// playerの攻撃力を取得
	/// </summary>
	/// <returns></returns>
	float GetHitDamage()const { return playerAttackData.hitDamage; }
	
	/// <summary>
	/// 現在攻撃中かを取得
	/// </summary>
	/// <returns></returns>
	bool IsAttack();

	/// <summary>
	/// 攻撃の当たり判定の生成をある程度統一化させた関数
	/// </summary>
	void AttackCollsion();
	/// <summary>
	/// 当たり判定が直前に生成されたかを取得
	/// </summary>
	/// <returns>攻撃が生成されていたらtrue</returns>
	bool GetCollsionCreate() { return collsionCreate; }

	/// <summary>
	/// 必殺技を発動
	/// </summary>
	void SpecialAttackStart();

	struct PlayerAttackData
	{
		StateID::State_ID state;
		StateID::State_ID normalAttackNextID;
		StateID::State_ID specialAttackNextID;

		float hitDamage;
		float attackAgainStartCounterMax;
		int attackNum;
		float collsionStartTime;
		float collsionFinishTime;

		float motionCancelStartTime;
		float runTimer;

		float frontSpeed;

		Transform collTrans;

		// デフォルトコンストラクタ
		PlayerAttackData()
			: state(StateID::State_ID::STATE_MAX)
			, normalAttackNextID(StateID::State_ID::STATE_MAX)
			, specialAttackNextID(StateID::State_ID::STATE_MAX)
			, hitDamage(0.0f)
			, attackAgainStartCounterMax(0.0f)
			, attackNum(0)
			, collsionStartTime(0.0f)
			, collsionFinishTime(0.0f)
			, motionCancelStartTime(0.0f)
			, runTimer(0.0f)
			, frontSpeed(0.0f)
			, collTrans()
		{
		}

		// 全引数コンストラクタ
		PlayerAttackData(
			StateID::State_ID state,
			StateID::State_ID normalAttackNextID,
			StateID::State_ID specialAttackNextID,
			float hitDamage,
			float attackAgainStartCounterMax,
			int attackNum,
			float collsionStartTime,
			float collsionFinishTime,
			float motionCancelStartTime,
			float runTimer,
			float frontSpeed,
			const Transform& collTrans
		)
			: state(state)
			, normalAttackNextID(normalAttackNextID)
			, specialAttackNextID(specialAttackNextID)
			, hitDamage(hitDamage)
			, attackAgainStartCounterMax(attackAgainStartCounterMax)
			, attackNum(attackNum)
			, collsionStartTime(collsionStartTime)
			, collsionFinishTime(collsionFinishTime)
			, motionCancelStartTime(motionCancelStartTime)
			, runTimer(runTimer)
			, frontSpeed(frontSpeed)
			, collTrans(collTrans)
		{
		}
	};

	const PlayerAttackData& GetAttackData()const { return playerAttackData; }

protected:
	float easingCount;
	float beforeAngle;
	float angle;
	bool nextAttack;
	bool nextAvoid;
	
	VECTOR3 dist;
	VECTOR3 norm;
	float distSize;
	float time;

	const float ATTACK_MOVE_DIST = 3000.0f;
	const float DISTANCE_MOVE = 800;

	bool beforeAttack;

	float runTimer;

	const float ATTACK_FINISH_ANIM_SPEED = 0.1f;
	bool rotation;

	
	bool defalutTrail;
	bool rockOn;

	int attackCount;

	float attackAgainStartCounter;
	//float attackAgainStartCounterMax;

	
	bool collsionCreate;

	void BaseAttackCollsion();
	void AttackMoveStart();

	void AgainAttackCollsion();
	void AgainTimerSet(float _time, int _attackNum);

	PlayerAttackData playerAttackData;
};

inline void to_json(nlohmann::json& j, const PlayerAttackStateBase::PlayerAttackData& p)
{
	j = {
		{"PlayerState", StateID::GetID(p.state)},
		{"normalAttackNextID", StateID::GetID(p.normalAttackNextID)},
		{"specialAttackNextID", StateID::GetID(p.specialAttackNextID)},

		{"hitDamage", p.hitDamage},
		{"attackAgainStartCounterMax", p.attackAgainStartCounterMax},
		{"attackNum", p.attackNum},
		{"collsionStartTime", p.collsionStartTime},
		{"collsionFinishTime", p.collsionFinishTime},

		{"motionCancelStartTime", p.motionCancelStartTime},
		{"runTimer", p.runTimer},

		{"frontSpeed", p.frontSpeed},

		{"collTrans", p.collTrans}
	};
}

inline void from_json(const nlohmann::json& j, PlayerAttackStateBase::PlayerAttackData& p)
{
	p.state = StateID::StringToID(j["PlayerState"].get<std::string>());
	p.normalAttackNextID = StateID::StringToID(j["normalAttackNextID"].get<std::string>());
	p.specialAttackNextID = StateID::StringToID(j["specialAttackNextID"].get<std::string>());

	j.at("hitDamage").get_to(p.hitDamage);
	j.at("attackAgainStartCounterMax").get_to(p.attackAgainStartCounterMax);
	j.at("attackNum").get_to(p.attackNum);
	j.at("collsionStartTime").get_to(p.collsionStartTime);
	j.at("collsionFinishTime").get_to(p.collsionFinishTime);

	j.at("motionCancelStartTime").get_to(p.motionCancelStartTime);
	j.at("runTimer").get_to(p.runTimer);

	j.at("frontSpeed").get_to(p.frontSpeed);

	j.at("collTrans").get_to(p.collTrans);
}