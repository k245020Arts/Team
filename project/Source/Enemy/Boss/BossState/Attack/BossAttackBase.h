#pragma once
#include "../../../TrashEnemy/EnemyState/EnemyStateBase.h"

//#define DataSave

class BossAttackBase:public EnemyStateBase
{
public:

	enum PlayerDamagePattern
	{
		NONE = -1,
		NO_BACK = 0,
		BACK,
		BLOW_AWAY,
	};

	static std::string ToString(PlayerDamagePattern p)
	{
		switch (p)
		{
		case NO_BACK:    return "NO_BACK";
		case BACK:       return "BACK";
		case BLOW_AWAY:  return "BLOW_AWAY";
		case NONE:       return "NONE";
		}
		return "NONE";
	}

	static PlayerDamagePattern FromString(const std::string& s)
	{
		if (s == "NO_BACK")    return NO_BACK;
		if (s == "BACK")       return BACK;
		if (s == "BLOW_AWAY")  return BLOW_AWAY;
		if (s == "NONE")       return NONE;

		// 不正データ対策
		return NONE;
	}

	struct BossAttackParam
	{
		BossAttackParam() {
			hitDamage = 0.0f;
			damagePattern = NONE;
		}

		std::string bossID;
		std::string attackID;

		bool useFlash;
		float attackFlashStartTime;

		float slowTime;
		float slowAmout;

		float speedUpMotionSpeed;

		float attackCollsionStartTime;
		float attackCollsionEndTime;

		float justAvoidCollsionStartTime;
		float justAvoidCollsionEndTime;

		int attackPositionFrameNum;

		float attackSoundStartTime;
		
		Transform attackCollTransform;
		Transform justAvoidCollTransform;

		ID::IDType animID;

		float hitDamage;

		PlayerDamagePattern damagePattern;
		std::string voiceName;
	};

	
	
	BossAttackBase();
	~BossAttackBase();

	void Update()override;

	void BossStart();
	void BossFinish();

	/// <summary>
	/// ダメージを受けているダメージ
	/// </summary>
	/// <returns></returns>
	float GetHitDamage()const  { return attackParam.hitDamage; }
	/// <summary>
	/// ダメージのパラメーターを取得
	/// </summary>
	/// <returns>ダメージのパラメーター</returns>
	const BossAttackParam& GetDamageParam() { return attackParam; }

	void DataSaveAll();

	void AttackCollsion();
	void BossAttackCollsion();

	void AttackSound();
	void AttackFlash(ID::IDType _modelId, int _modelFrame, std::string _voice);
	void AttackBeforeFrash(ID::IDType _modelId, int _modelFrame, std::string _voice);
	void Trail();
	void BossTrail(bool _right);

	void BossJustAvoidCollsion();
	void EnemyJustAvoidCollsion();

	void LoadAttackParam();

protected:
	BossAttackParam attackParam;
};

inline void to_json(JSON& j, const BossAttackBase::BossAttackParam& p)
{
	j = JSON{
		{"bossID", p.bossID},
		{"attackID", p.attackID},
		{"useFlash", p.useFlash},
		{"attackFlashStartTime", p.attackFlashStartTime},
		{"slowTime", p.slowTime},
		{"slowAmout", p.slowAmout},
		{"speedUpMotionSpeed", p.speedUpMotionSpeed},
		{"attackCollsionStartTime", p.attackCollsionStartTime},
		{"attackCollsionEndTime", p.attackCollsionEndTime},
		{"justAvoidCollsionStartTime", p.justAvoidCollsionStartTime},
		{"justAvoidCollsionEndTime", p.justAvoidCollsionEndTime},
		{"attackPositionFrameNum", p.attackPositionFrameNum},
		{"attackSoundStartTime", p.attackSoundStartTime},
		{"attackCollTransform", p.attackCollTransform},
		{"justAvoidCollTransform", p.justAvoidCollTransform},

		{"animID", ID::GetID(p.animID)},

		{"hitDamage", p.hitDamage},

		{"damagePattern", BossAttackBase::ToString(p.damagePattern)},

		{"voiceName", p.voiceName}
	};
}
// from_json
inline void from_json(const JSON& j, BossAttackBase::BossAttackParam& p)
{
	j.at("bossID").get_to(p.bossID);
	j.at("attackID").get_to(p.attackID);
	j.at("useFlash").get_to(p.useFlash);
	j.at("attackFlashStartTime").get_to(p.attackFlashStartTime);
	j.at("slowTime").get_to(p.slowTime);
	j.at("slowAmout").get_to(p.slowAmout);
	j.at("speedUpMotionSpeed").get_to(p.speedUpMotionSpeed);
	j.at("attackCollsionStartTime").get_to(p.attackCollsionStartTime);
	j.at("attackCollsionEndTime").get_to(p.attackCollsionEndTime);
	j.at("justAvoidCollsionStartTime").get_to(p.justAvoidCollsionStartTime);
	j.at("justAvoidCollsionEndTime").get_to(p.justAvoidCollsionEndTime);
	j.at("attackPositionFrameNum").get_to(p.attackPositionFrameNum);
	j.at("attackSoundStartTime").get_to(p.attackSoundStartTime);
	j.at("attackCollTransform").get_to(p.attackCollTransform);
	j.at("justAvoidCollTransform").get_to(p.justAvoidCollTransform);

	std::string animStr;
	j.at("animID").get_to(animStr);
	p.animID = ID::StringToID(animStr);

	j.at("hitDamage").get_to(p.hitDamage);

	std::string pattern;
	j.at("damagePattern").get_to(pattern);
	p.damagePattern = BossAttackBase::FromString(pattern);

	j.at("voiceName").get_to(p.voiceName);
}