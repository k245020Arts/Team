#pragma once
#include "../EnemyBase.h"
#include "../../Component/Object/Object3D.h"
#include "../EnemyInformation.h"

class StateManager;
class EnemyStateManager;
class CharaWeapon;
class T_EnemyStatus;

struct DeadData
{
	float maxVelocity;	//速度
	float gravity;		//落下の強さ（重力）
	float timeToPeak;	//最高点までの時間
	float maxHeight;	//吹き飛びの高さ
};

class TrashEnemy : public EnemyBase
{
public:
	//ステート関連
	friend class T_EnemyIdol;
	friend class T_EnemyRun;
	friend class T_EnemyAttack;
	friend class T_EnemyDead;
	friend class Standby;
	friend class T_EnemyWaitSee;
	friend class T_EnemyJump;
	friend class CooperateAttack1;
	friend class CooperateAttack2;
	friend class T_EnemyDamage;
	friend class EnemyStateBase;
	friend class BossAttackBase;

	TrashEnemy();
	~TrashEnemy();

	void Update()override;
	void Draw()override;

	void Start(Object3D* _obj);
	/// <summary>
	/// 敵の生成
	/// </summary>
	/// <param name="_pos">ポジションの指定</param>
	/// <param name="kinds">敵の強さの種類(０なら軽い,1ならデフォルト,それ以外は重い敵)</param>
	void CreateTrashEnemy(VECTOR3 _pos,int kinds,int _number);

	void Trail();

	void PlayerHit()override;

	//攻撃命令
	void AttackCommand();
	//攻撃クールタイムリセット
	void AttackCoolTimeReset() { isAttack = true; }
	//連携攻撃を終わらせた後の処理
	void CooperateAtkFinish();

	enum EnemyType
	{
		MELEE,			//近距離
		RANGED_LEADER,	//遠距離のリーダ
		RANGED,			//遠距離

		MAX,
	};

	//ゲッター
	VECTOR3 GetPos() { return obj->GetTransform()->position; }
	float Speed() { return speed; }
	//通常攻撃をしてもよいか
	bool IsAttack() { return isAttack; }
	//連携攻撃かどうか
	bool IsCooperateAtk() { return isCooperateAtk; }
	//連携攻撃でプレイヤーのほうに向かうかどうか
	bool IsMovingToPlayer() { return isMovingToPlayer; }
	//連携攻撃の準備ができてるか
	bool GetStandby() { return isStandby; }
	//生きてるかどうか
	bool GetActive() { return active; }
	//連携攻撃を取得する
	const StateID::State_ID GetNextCooperateID() { return nextCooperateID; };
	//近距離か遠距離の敵か分かる関数
	EnemyType GetEnemyType()const { return enemyType; }
	//runステートか分かる関数
	bool GetIsRunState() { return isRunState; }
	//追いかける時にどのポイントを追いかけるか決める
	int GetPointNumber() { return pointNumber; }

	VECTOR3 TargetPoint()const { return targetPoint; }

	//セッター
	void SetWayPoint(VECTOR3 _wayPoint);
	void SetCooperateWayPoint(VECTOR3 _wayPoint) { cooperateWayPoint = _wayPoint; }
	// //ウェイポイントを取得して連携攻撃に切り替わる関数
	void SetCooperateWayPoint(VECTOR3 _pos, StateID::State_ID _id);
	//敵のステートを変える関数
	void ChangeState(StateID::State_ID _id);
	//加速移動
	void Move(float _speed, float _max);
	void AddPos(VECTOR3 _pos);
	//近距離か遠距離の敵かを指定する関数
	void SetEnemyType(EnemyType type);
	//遠距離の敵の攻撃準備
	void ReadyCooperteAtk2(VECTOR3 _pos);

	//遠距離の敵が攻撃する
	void RangedAttack();

	template<typename T>
	T* CollsionStart(CollsionSet* _set, const Transform& _trans)
	{
		if (_set->instance == nullptr) {
			CollsionInfo info = CharaBase::CollsionInstant<T>(_set, _trans);
			info.tag = _set->tag;
			//collName = _set->collName;
			_set->instance->CollsionAdd(info, _trans, _set->collName);
		};
		return static_cast<T*>(_set->instance);
	}

private:
	CharaWeapon* chara;
	T_EnemyStatus* eStatus;

	DeadData deadPreset;
	std::vector<DeadData> deadPresets
	{
		{0,0,0,0},
		{10.0f, -800.0f ,0.5f,600.0f},//通常
		{30.0f, -1000.0f,0.5f,800.0f},//ジャスト回避
		{40.0f, -1200.0f,0.5f,1000.0f},//必殺
	};

	//_posの方向に向く
	void LookTarget(VECTOR3 _pos);
	bool IsPlayerSpecialMove();
	
	bool active;
	
	//通常攻撃をしてもよいか
	bool isAttack;
	//連携攻撃中
	bool isCooperateAtk;
	//プレイヤーのほうに向かうかどうか
	bool isMovingToPlayer;
	//連携攻撃の準備ができてるか
	bool isStandby;
	
	//移動速度
	float speed;
	//防御力
	float defense;

	//強めの味方についていくか
	//bool isEnemyFollow;
	
	VECTOR3 targetPoint;
	//連携攻撃時のポジション指定
	VECTOR3 cooperateWayPoint;

	//必殺技を食らったときに敵をスローにする
	float slowCounter;
	//モーションが止まる速度
	float mStopCounter;

	StateID::State_ID nextCooperateID;

	EnemyType enemyType;

	bool isRunState;

	int pointNumber;
};