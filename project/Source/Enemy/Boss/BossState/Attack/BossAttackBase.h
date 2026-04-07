#pragma once
#include "../../../TrashEnemy/EnemyState/EnemyStateBase.h"
#include "../../../../Common/ID/EffectID.h"
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
		BossAttackParam()
		{
			bossID = "";
			attackID = "";

			useFlash = false;
			attackFlashStartTime = 0.0f;

			slowTime = 0.0f;
			slowAmout = 0.0f;

			speedUpMotionSpeed = 0.0f;

			attackCollsionStartTime = 0.0f;
			attackCollsionEndTime = 0.0f;

			justAvoidCollsionStartTime = 0.0f;
			justAvoidCollsionEndTime = 0.0f;

			attackPositionFrameNum = 0;

			attackSoundStartTime = 0.0f;

			attackCollTransform = Transform();
			justAvoidCollTransform = Transform();

			animID = ID::IDType();

			hitDamage = 0.0f;
			damagePattern = NONE;

			voiceName = "";

			// 移動イベント
			frontMove = false;
			moveSpeed = 0.0f;

			// プレイヤー追従イベント
			playerAloowMove = false;
			baseSpeed = 0.0f;
			playerNearStop = false;
			playerNearAloowStop = false;
			playerBaseNear = 0.0f;
			maxMoveSpeed = 0.0f;
			minMoveSpeed = 0.0f;

			// 突進イベント
			rushMove = false;
			rushBeforeAnimID = ID::IDType();
			rushAfterAnimID = ID::IDType();
			rushSoundRightFoot = 0.0f;
			rushSoundLeftFoot = 0.0f;

			// 回転イベント
			rotateMove = false;
			angleMoveAmout = 0.0f;

			// ジャンプイベント
			jump = false;
			addGravity = 0.0f;

			// 衝撃波イベント
			shockWave = false;
			shockMoveEffect = Effect_ID::EFFECT_ID(); // デフォルト
			shockWaveSpeed = 0.0f;
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

		//移動イベント
		bool frontMove;
		float moveSpeed;
		

		//プレイヤー追従イベント
		bool playerAloowMove;
		float baseSpeed;
		bool playerNearStop;
		bool playerNearAloowStop;
		bool addVelocity;
		float playerBaseNear;
		float maxMoveSpeed;
		float minMoveSpeed;
		float moveStartTime;
		float moveFinishTime;

		//突進イベント
		bool rushMove;
		ID::IDType rushBeforeAnimID;
		ID::IDType rushAfterAnimID;
		float rushSoundRightFoot;
		float rushSoundLeftFoot;

		//回転イベント
		bool rotateMove;
		float angleMoveAmout;

		//ジャンプイベント
		bool jump;
		float addGravity;
		float jumpSpeed;
		float jumpStartTime;
		float groundEffectStartTime;
		Effect_ID::EFFECT_ID jumpGroundEffect;
		float groundShakeCamera;
		float groundShakeTime;

		//衝撃波イベント
		bool shockWave;
		Effect_ID::EFFECT_ID shockMoveEffect;
		float shockWaveSpeed;
		float startRange;

		//プレイヤー見るイベント
		bool lookPlayer;
		int lookNum;
		float lookMaxCounter;
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

	void RotateEvent();
	void LookEvent();
	void MoveEvent();
	void JumpEvent();
	void ShackWaveEvent();
	void CreateWave();

protected:
	BossAttackParam attackParam;
private:
	float averageSpeed;
	bool aloowStop;
	VECTOR3 normal;
	bool firstJump;
	float gravitySpeed;
	bool groundEffect;
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
		{"voiceName", p.voiceName},

		// 移動イベント
		{"frontMove", p.frontMove},
		{"moveSpeed", p.moveSpeed},

		// プレイヤー追従イベント
		{"playerAloowMove", p.playerAloowMove},
		{"baseSpeed", p.baseSpeed},
		{"playerNearStop", p.playerNearStop},
		{"playerNearAloowStop", p.playerNearAloowStop},
		{"playerBaseNear", p.playerBaseNear},
		{"maxMoveSpeed", p.maxMoveSpeed},
		{"minMoveSpeed", p.minMoveSpeed},

		// 突進イベント
		{"rushMove", p.rushMove},
		{"rushBeforeAnimID", ID::GetID(p.rushBeforeAnimID)},
		{"rushAfterAnimID", ID::GetID(p.rushAfterAnimID)},
		{"rushSoundRightFoot", p.rushSoundRightFoot},
		{"rushSoundLeftFoot", p.rushSoundLeftFoot},

		// 回転イベント
		{"rotateMove", p.rotateMove},
		{"angleMoveAmout", p.angleMoveAmout},

		// ジャンプイベント
		{"jamp", p.jump},
		{"addGravity", p.addGravity},

		// 衝撃波イベント
		{"shockWave", p.shockWave},
		{"shockMoveEffect", static_cast<int>(p.shockMoveEffect)},
		{"shockWaveSpeed", p.shockWaveSpeed}
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

	//// 移動イベント
	//j.at("frontMove").get_to(p.frontMove);
	//j.at("moveSpeed").get_to(p.moveSpeed);

	//// プレイヤー追従イベント
	//j.at("playerAloowMove").get_to(p.playerAloowMove);
	//j.at("baseSpeed").get_to(p.baseSpeed);
	//j.at("playerNearStop").get_to(p.playerNearStop);
	//j.at("playerNearAloowStop").get_to(p.playerNearAloowStop);
	//j.at("playerBaseNear").get_to(p.playerBaseNear);
	//j.at("maxMoveSpeed").get_to(p.maxMoveSpeed);
	//j.at("minMoveSpeed").get_to(p.minMoveSpeed);

	//// 突進イベント
	//j.at("rushMove").get_to(p.rushMove);

	//std::string rushBefore, rushAfter;
	//j.at("rushBeforeAnimID").get_to(rushBefore);
	//j.at("rushAfterAnimID").get_to(rushAfter);
	//p.rushBeforeAnimID = ID::StringToID(rushBefore);
	//p.rushAfterAnimID = ID::StringToID(rushAfter);

	//j.at("rushSoundRightFoot").get_to(p.rushSoundRightFoot);
	//j.at("rushSoundLeftFoot").get_to(p.rushSoundLeftFoot);

	//// 回転イベント
	//j.at("rotateMove").get_to(p.rotateMove);
	//j.at("angleMoveAmout").get_to(p.angleMoveAmout);

	//// ジャンプイベント
	//j.at("jamp").get_to(p.jamp);
	//j.at("addGravity").get_to(p.addGravity);

	//// 衝撃波イベント
	//j.at("shockWave").get_to(p.shockWave);

	//std::string effect;
	//j.at("shockMoveEffect").get_to(effect);
	//p.shockMoveEffect = Effect_ID::StringToID(effect);

	//j.at("shockWaveSpeed").get_to(p.shockWaveSpeed);
}