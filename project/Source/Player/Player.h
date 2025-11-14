#pragma once
#include "../CharaBase/CharaBase.h"
#include "playerInformation.h"
#include "../Component/Transform/Transform.h"
#include "../Common/ID/ID.h"
#include  "../Common/ID/EffectID.h"
#include "../Common/ID/SoundID.h"

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
	/// <summary>
	/// 移動の関数
	/// </summary>
	/// <param name="_speed"></param>
	/// <param name="_speedMax"></param>
	void Move(float _speed,float _speedMax);
	/// <summary>
	/// プレイヤーの回転を滑らかに行うために行っている関数
	/// 上は自由指定ver、下はデフォルト値ver
	/// </summary>
	/// <param name="_angle"></param>
	/// <param name="_speed"></param>
	void RotationChange(VECTOR3 _angle,float _speed);
	void RotationChange();
	/// <summary>
	/// 回避の向き指定とか移動とかの処理を行っている関数
	/// </summary>
	/// <param name="_speed"></param>
	/// <param name="_speedMax"></param>
	/// <param name="cameraAngle"></param>
	/// <param name="_upSpeed"></param>
	void Avoid(float _speed, float _speedMax,float cameraAngle,float _upSpeed);
	void ImguiDraw()override;
	/// <summary>
	/// プレイヤーの移動のスティックの値を得られる関数
	/// </summary>
	void PlayerStickInput();

	VECTOR3 GetWalkAngle() { return walkAngle; }
	bool GetAvoidStart() { return avoidStart; }
	void SetAvoidStart(bool _set) { avoidStart = _set; }
	/// <summary>
	/// 回避が始まるときのちょっとした隙をつくるかんすう
	/// </summary>
	void AvoidReady();
	/// <summary>
	/// 回避した時の回転のチェンジをしている関数
	/// </summary>
	void AvoidRotationChange();
	
	/// <summary>
	/// 敵の攻撃を食らった時の処理
	/// </summary>
	/// <param name="_attackId"></param>
	/// <param name="_obj"></param>
	/// <returns></returns>
	bool EnemyHit(ID::IDType _attackId, BaseObject* _obj);
	Transform* GetPlayerTransform(){ return playerTransform; }
	BaseObject* GetPlayerObj() { return obj; }
	void JustAvoidCan();
	/// <summary>
	/// カメラのターゲットをセットする関数
	/// </summary>
	/// <param name="_base"></param>
	void TargetObjSet(BaseObject* _base);
	/// <summary>
	/// 攻撃を食らったりしたときとか、ジャスト回避した敵の種類をエル関数
	/// </summary>
	/// <param name="_base"></param>
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

	/// <summary>
	/// 軌跡表示の関数で引数なしはdefalutのものが呼び出される。
	/// 特別仕様にカスタマイズしたいなら下の関数を使用する
	/// </summary>
	void DrawTrail();
	void DrawTrail(VECTOR3 _nPos, VECTOR3 _fPos, float _r, float _g, float _b, float _a, float index, float _time);

	void DeleteCollision() override;

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

	const float MAX_HP = 1000;
};