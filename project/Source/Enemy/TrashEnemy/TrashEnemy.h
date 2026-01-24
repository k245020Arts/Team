#pragma once
#include "../EnemyBase.h"
#include "../../Component/Object/Object3D.h"
#include "../EnemyInformation.h"

class StateManager;
class EnemyStateManager;
class CharaWeapon;
class T_EnemyStatus;

class TrashEnemy : public EnemyBase
{
public:
	//ステート関連
	friend class T_EnemyIdol;
	friend class T_EnemyRun;
	friend class T_EnemyAttack;
	friend class T_EnemyDead;
	friend class Standby;
	friend class T_EnemyJump;
	friend class CooperateAttack1;
	friend class T_EnemyDamage;

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
	void CreateTrashEnemy(VECTOR3 _pos,int kinds);

	void Trail();

	void PlayerHit()override;

	//ウェイポイントを取得して連携攻撃に切り替わる関数
	void GetWayPoint(VECTOR3 _pos, StateID::State_ID _id);
	//敵のステートを変える関数
	void ChangeState(StateID::State_ID _id);
	//void isStandbyF() { isStandby = false; }

	//攻撃命令
	void AttackCommand();
	//攻撃クールタイムリセット
	void AttackCoolTimeReset() { isAttack = true; }
	void CooperateAtkFinish();

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

	//セッター
	//加速移動
	void Move(float _speed, float _max);
	void AddPos(VECTOR3 _pos);

	template<typename T>
	T* CollsionStart(CollsionInformation::Shape _shape, Transform _trans)
	{
		if (attackColl == nullptr)
		{
			CollsionInfo info = CharaBase::CollsionInstant<T>(_shape, _trans);
			info.tag = CollsionInformation::Tag::E_ATTACK;
			collName = "e_attack";
			attackColl->CollsionAdd(info, _trans, collName);
		};
		return static_cast<T*>(attackColl);
	}
private:
	CharaWeapon* chara;
	T_EnemyStatus* eStatus;

	//targetPosに入ってる方向に向く
	void LookTarget();
	bool IsPlayerSpecialMove();
	bool active;
	//どこに向くか
	VECTOR3 targetPos;

	//通常攻撃をしてもよいか
	bool isAttack;

	//連携攻撃中
	bool isCooperateAtk;
	//プレイヤーのほうに向かうかどうか
	bool isMovingToPlayer;
	//連携攻撃の準備ができてるか
	bool isStandby;
	
	float speed;

	//強めの味方についていくか
	bool isEnemyFollow;
	VECTOR3 wayPoint;
	//自分の敵の種類
	int number;
	//必殺技を食らったときに敵をスローにする
	float slowCounter;
};