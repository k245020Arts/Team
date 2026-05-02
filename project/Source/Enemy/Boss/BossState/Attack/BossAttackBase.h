#pragma once
#include "../../../TrashEnemy/EnemyState/EnemyStateBase.h"
#include "../../../../Common/ID/EffectID.h"
//#define DataSave
#include "../../../../Component/Collider/CollsionInformation.h"

class SphereCollider;

class BossAttackBase: public EnemyStateBase
{
public:

	struct RayColliderInfo
	{
		float rayStartPos; //Rayの判定の開始の座標
		float rayFinishPos; //Rayの判定の終了の座標
	};

	struct DountColliderInfo
	{
		float inRadius; //内側の半径
		float outRadius; //外側の円の半径
	};

	struct ThrowObjectAttackData
	{
		//基本データ
		std::string throwObjectID;
		VECTOR3 baseGravity;
		VECTOR3 baseFirction;

		//押し返し当たり判定
		bool pushCollCan;
		Transform pushCollTransform;

		//地面に着地するか
		bool randCan;
		RayColliderInfo randCollInfo;
		//消える時間
		float randTime;

		//投擲物がプレイヤーに当たるか
		bool playerHit;
		bool playerHitNoReaction;
		float playerHitCollRadius;
		float playerHitJustAvoidCollRadius;
		bool capsule;
		VECTOR3 capselEndPos;
		VECTOR3 capselStartPos;
		bool capsuleColliderAdd;
		VECTOR3 causuleColliderAddPos;
		bool capsuleColliderAddStartThrow;
		float capsuleColliderAddStartThrowAnimFrame;
		//多段階当たり判定
		bool alotCollsionHit;
		float alotHitColliderCreateCounter;

		//地面についた後にプレイヤーに当たり判定がつくか
		bool playerGroundHit;
		bool playerGroundOneHit;
		bool playerGroundNoDamageReactionHit;
		float playerGroundCollRadius;

		//ボスにダメージを食らうか
		bool bossHit;
		float bossHitCollRadius;

		//残留した投擲物にプレイヤーの攻撃ではじき返せるか
		bool playerAttackFlying;
		float playerAttackFlyingCollRadius;
		float flyingSpeed;
		float flyingHeight;
		
		//ボスが突進攻撃をしてきたときに当たったらひるむ当たり判定
		bool bossRushHit;
		float bossRushHitCollRadius;

		//爆発するか
		bool blastCan;
		DountColliderInfo blastColliderInfo;
		DountColliderInfo blastJustAvoidColliderInfo;
		float blastBlinkMaxCounter;
		bool randomBlast;
		float randomBlastRate;
		float maxRadius;
		float waveSpeed;;
		//ランダムに出現する高さを変えるか
		bool randomHeight;
		float minHeight;
		float maxHeight;
		//ランダムに出現するスピードを変えるか
		bool randomSpeed;
		float minSpeed;
		float maxSpeed;
		

		//予測線を出すか
		bool predictionCicleCan;
		RayColliderInfo predictionCicleColliderInfo;
		//腕で投げるか
		bool armThrow;
		bool armSwordHand;
		int armFrameNum;
		float throwStartTime;
		float throwObjectApperaTime;
		VECTOR3 armAddPos;
		bool throwToPlayer;
		bool thorwToFront;
		float throwSpeed;
		float upSpeed;
		float throwFirstSpeed;
		VECTOR3 diffusionAngle;
		//落ちるか
		bool throwToFall;
		float throwHeight;
		float throwFallGravity;
		bool throwToFallToPlayer;
		bool freeDir;
		VECTOR3 thorwStartPos;
		VECTOR3 thorwVelocity;
		//地面についた後に消えるか
		bool groundDelete;
		//ダメージを受けた時にオブジェクトを落とすかどうか
		bool playerAttackObjectDrop;
		bool attackFinishDelete;
		

		ThrowObjectAttackData()
		{
			throwObjectID = "";

			baseGravity = VECTOR3();
			baseFirction = VECTOR3();

			pushCollCan = false;
			pushCollTransform = Transform();

			randCan = false;
			randCollInfo = RayColliderInfo();
			randTime = 0.0f;

			playerHit = false;
			playerHitCollRadius = 0.0f;
			playerHitJustAvoidCollRadius = 0.0f;
			capsule = false;
			capselEndPos = VZero;
			capselStartPos = VZero;


			playerGroundHit = false;
			playerGroundCollRadius = 0.0f;

			bossHit = false;
			bossHitCollRadius = 0.0f;

			playerAttackFlying = false;
			playerAttackFlyingCollRadius = 0.0f;
			flyingSpeed = 0.0f;
			flyingHeight = 0.0f;
			maxRadius = 0.0f;
			waveSpeed = 0.0f;

			bossRushHit = false;
			bossRushHitCollRadius = 0.0f;

			blastCan = false;
			blastColliderInfo = DountColliderInfo();
			blastJustAvoidColliderInfo = DountColliderInfo();
			blastBlinkMaxCounter = 0.0f;
			randomBlast = false;
			randomBlastRate = 0.0f;
			randomHeight = false;
			minHeight = 0.0f;
			maxHeight = 0.0f;
			randomSpeed = false;
			minSpeed = 0.0f;
			maxSpeed = 0.0f;

			predictionCicleCan = false;
			predictionCicleColliderInfo = RayColliderInfo();

			armThrow = false;
			armFrameNum = 0;
			throwStartTime = 0.0f;
			throwObjectApperaTime = 0.0f;
			armAddPos = VECTOR3();
			throwToPlayer = false;
			thorwToFront = false;
			throwSpeed = 0.0f;
			upSpeed = 0.0f;
			throwFirstSpeed = 0.0f;
			diffusionAngle = VECTOR3();
			throwToFall = false;
			throwHeight = 0.0f;
			throwFallGravity = 0.0f;
			throwToFallToPlayer = false;
			freeDir = false;
			thorwStartPos = VECTOR3();
			thorwVelocity = VECTOR3();

			groundDelete = false;

			playerAttackObjectDrop = false;
			armSwordHand = false;

			capsuleColliderAdd = false;
			causuleColliderAddPos = VZero;
			capsuleColliderAddStartThrow = false;
			capsuleColliderAddStartThrowAnimFrame = 0.0f;

			attackFinishDelete = false;
			playerHitNoReaction = false;
			alotCollsionHit = false;
			alotHitColliderCreateCounter = 0.0f;
		}
	};

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
			animNum = 0;
			animFileName = "";

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
			attackBeforeAnimID = ID::ID_MAX;

			hitDamage = 0.0f;
			damagePattern = NONE;

			voiceName = "";

			// 移動イベント
			frontMove = false;
			moveSpeed = 0.0f;

			// プレイヤー追従イベント
			playerAloowMove = false;
			baseFirstSpeed = 0.0f;
			baseSpeed = 0.0f;
			playerNearStop = false;
			playerNearAloowStop = false;
			addVelocity = false;
			playerBaseNear = 0.0f;
			maxMoveSpeed = 0.0f;
			minMoveSpeed = 0.0f;
			moveStartTime = 0.0f;
			moveFinishTime = 0.0f;

			// 突進イベント
			rushMove = false;
			rushAfterAnimID = ID::ID_MAX;
			rushAfterSpeed = 0.0f;
			rushSoundRightFoot = 0.0f;
			rushSoundLeftFoot = 0.0f;
			rushTime = 0.0f;
			rushColl = false;
			addRushCollScale = 0.0f;

			// 回転イベント
			rotateMove = false;
			angleMoveAmout = 0.0f;

			// ジャンプイベント
			jump = false;
			addGravity = 0.0f;
			jumpSpeed = 0.0f;
			jumpStartTime = 0.0f;
			groundEffectStartTime = 0.0f;
			jumpGroundEffect = Effect_ID::EFFECT_ID();
			groundShakeCamera = 0.0f;
			groundShakeTime = 0.0f;

			// 衝撃波イベント
			shockWave = false;
			shockMoveEffect = Effect_ID::EFFECT_ID();
			shockWaveSpeed = 0.0f;
			startRange = 0.0f;

			// プレイヤー見るイベント
			lookPlayer = false;
			lookNum = 0;
			lookMaxCounter = 0.0f;

			// 投擲イベント
			throwObject = false;
			throwAttackData.clear();
			armThrow = false;
			

			throwObjectNum = 0;
			objectApperaPosition = VECTOR3();
			intervalTime = 0.0f;
			intervalTimeSub = false;
			maxIntervalTime = 0.0f;
			minIntervalTime = 0.0f;

			attackCameraBossLook = false;
			cameraChangeSpeed = 0.0f;

			useTrail = false;
			trailRightHand = false;
		}

		
		//基本データ
		std::string bossID;
		std::string attackID;
		std::string animFileName;
		int animNum;
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
		ID::IDType attackBeforeAnimID;
		float hitDamage;
		PlayerDamagePattern damagePattern;
		std::string voiceName;

		//移動イベント
		bool frontMove;
		float moveSpeed;
		

		//プレイヤー追従イベント
		bool playerAloowMove;
		float baseSpeed;
		float baseFirstSpeed;
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
		ID::IDType rushAfterAnimID;
		float rushAfterSpeed;
		float rushSoundRightFoot;
		float rushSoundLeftFoot;
		float rushTime;
		bool rushColl;
		float addRushCollScale;

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

		//投擲イベント
		bool throwObject;
		std::vector<ThrowObjectAttackData> throwAttackData;
		bool armThrow;
		int throwObjectNum;
		VECTOR3 objectApperaPosition;
		float intervalTime;
		bool intervalTimeSub;
		float maxIntervalTime;
		float minIntervalTime;
		//カメラ見るイベント
		bool attackCameraBossLook;
		float cameraChangeSpeed;
		//トレイルイベント
		bool useTrail;
		bool trailRightHand;

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
	/// <summary>
	/// データのセーブをする関数
	/// </summary>
	void DataSaveAll();
	/// <summary>
	/// 攻撃の当たり判定の生成
	/// </summary>
	void AttackCollsion();
	/// <summary>
	/// ボスの攻撃の当たり判定の生成
	/// </summary>
	void BossAttackCollsion();
	/// <summary>
	/// 攻撃時の音の生成
	/// </summary>
	void AttackSound();
	/// <summary>
	/// 攻撃をするときに手を光らせる前段階の処理
	/// </summary>
	/// <param name="_modelId">モデルのEnum</param>
	/// <param name="_modelFrame">光らせる場所のフレーム</param>
	/// <param name="_voice">ボイスの名前</param>
	void AttackFlash(ID::IDType _modelId, int _modelFrame, std::string _voice);
	/// <summary>
	/// 攻撃をするときに手を光らせる処理
	/// </summary>
	/// <param name="_modelId">モデルのEnum</param>
	/// <param name="_modelFrame">光らせる場所のフレーム</param>
	/// <param name="_voice">ボイスの名前</param>
	void AttackBeforeFrash(ID::IDType _modelId, int _modelFrame, std::string _voice);
	/// <summary>
	/// 軌跡の表示
	/// </summary>
	void Trail();
	/// <summary>
	/// 軌跡の表示
	/// </summary>
	/// <param name="_right">右手から出るか左手から出るか</param>
	void BossTrail(bool _right);
	/// <summary>
	/// ジャスト回避当たり判定の生成
	/// </summary>
	void BossJustAvoidCollsion();
	/// <summary>
	/// ジャスト回避当たり判定の生成
	/// </summary>
	void EnemyJustAvoidCollsion();
	/// <summary>
	/// 攻撃のデータのロード
	/// </summary>
	void LoadAttackParam();
	/// <summary>
	/// 攻撃のデータの受け渡し
	/// </summary>
	/// <param name="_param"></param>
	void SetAttackParam(BossAttackParam _param);

	/// <summary>
	/// 回転イベント
	/// </summary>
	void RotateEvent();
	/// <summary>
	/// プレイヤーを見るイベント
	/// </summary>
	void LookEvent();
	/// <summary>
	/// 移動イベント
	/// </summary>
	void MoveEvent();
	/// <summary>
	/// ジャンプイベント
	/// </summary>
	void JumpEvent();
	/// <summary>
	/// 衝撃波イベント
	/// </summary>
	void ShackWaveEvent();
	/// <summary>
	/// 衝撃波の生成
	/// </summary>
	void CreateWave();
	/// <summary>
	/// 投擲物イベント
	/// </summary>
	void ThrowObjectsEvent();
	/// <summary>
	/// 突進イベント
	/// </summary>
	void RushEvent();
	/// <summary>
	/// 今再生されてるアニメーションが攻撃アニメーションかどうかを判定する
	/// </summary>
	/// <returns></returns>
	bool CurrentAttackAnim();
	/// <summary>
	/// 攻撃後の終了判定
	/// </summary>
	void AttackFinishFrame();

protected:
	BossAttackParam attackParam;
private:
	float averageSpeed;
	bool aloowStop;
	VECTOR3 normal;
	bool firstJump;
	float gravitySpeed;
	bool groundEffect;
	bool throwRock;
	bool rockGet;
	float throwObjectAppearTime;
	int throwObjectNumNow;
	float rushAttackCount;
	bool rushSound;
	bool firstOnes;
	bool secondOnes;
	bool firstMove;

	SphereCollider* rockColl;

	void AttackStart();
	void BossDushSound();
	void BossUpdate();
};
