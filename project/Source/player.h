#pragma once
#include "CharaBase.h"
#include "playerInformation.h"
#include "transform.h"
#include "ID.h"
#include "EffectID.h"
#include "SoundID.h"

class Player : public CharaBase
{
public:
	friend class PlayerAvoid;
	friend class PlayerBlowAway;
	friend class PlayerDamage;
	friend class PlayerDie;
	friend class PlayerJustAvoid;
	friend class PlayerWait;
	friend class PlayerWalk;
	friend class PlayerAttack1;
	friend class PlayerJustAvoidAttack1;
	friend class PlayerAttack2;
	friend class PlayerJustAvoidAttack2;
	friend class PlayerAttack3;
	friend class PlayerJustAvoidAttack3;
	friend class PlayerAttack4;
	friend class PlayerJustAvoidAttack4;
	friend class PlayerAttack5;
	friend class PlayerJustAvoidAttack5;
	friend class PlayerAttackStateBase;
	friend class PlayerStateBase;

	Player();
	~Player();

	void Update()override;
	void Draw()override;
	void Start(Object3D* _obj);

	void Move(float _speed,float _speedMax);
	void RotationChange(VECTOR3 _angle,float _speed);
	void RotationChange();
	void Avoid(float _speed, float _speedMax,float cameraAngle,float _upSpeed);
	void ImguiDraw()override;
	void PlayerStickInput();

	VECTOR3 GetWalkAngle() { return walkAngle; }
	bool GetAvoidStart() { return avoidStart; }
	void SetAvoidStart(bool _set) { avoidStart = _set; }
	void AvoidReady();
	void AvoidRotationChange();

	bool EnemyHit(ID::IDType _attackId, BaseObject* _obj);
	Transform* GetPlayerTransform(){ return playerTransform; }
	BaseObject* GetPlayerObj() { return obj; }
	void JustAvoidCan();

	void TargetObjSet(BaseObject* _base);
	void HitObjectSet(BaseObject* _base);

	template<typename T>
	T* CollsionStart(CollsionInformation::Shape _shape, Transform _trans) {
		if (attackColl == nullptr) {
			CollsionInfo info = CharaBase::CollsionInstant<T>(_shape, _trans);
			info.tag = CollsionInformation::Tag::P_ATTACK;
			info.oneColl = false;
			//collName = "p_attack"
			attackColl->CollsionAdd(info, _trans, "p_attack");
		};
		return static_cast<T*>(attackColl);
	}
	void PlayerAttackHit();
	bool IsShake();
	bool GetJustAvoid() { return justAvoid; }
	StateManager* GetPlayerStateManager();

	void AvoidFinishState();
	void DeleteCollision();

	/// <summary>
	/// 軌跡表示の関数で引数なしはdefalutのものが呼び出される。
	/// 特別仕様にカスタマイズしたいなら下の関数を使用する
	/// </summary>
	void DrawTrail();
	void DrawTrail(VECTOR3 _nPos, VECTOR3 _fPos, float _r, float _g, float _b, float _a, float index, float _time);

private:
	PlayerInformation::CharaComponent playerCom;
	float size;
	Transform* playerTransform;
	VECTOR3 walkAngle;
	bool avoidStart;
	bool avoidReady;
	bool enemyHit;
	float justAvoidCanCounter;
	bool justAvoid;
	int avoidCounter;
	float noAvoidCounter;
	float avoidReadyCounter;

	const float MAX_HP = 100;
};