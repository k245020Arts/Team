#pragma once
#include "colliderBase.h"

class CollsionEvent
{
public:
	CollsionEvent();
	~CollsionEvent();

	//イベント発生
	void Event(ColliderBase* _coll1,ColliderBase* _coll2, Pushback& resolver,const VECTOR3& _hitPos);

private:
	//プレイヤーがダメージを食らった時
	void PlayerDamageEvent(ColliderBase* _coll1, ColliderBase* _coll2, const VECTOR3& _hitPos);
	//敵がダメージを食らった時
	void EnemyDamageEvent(ColliderBase* _coll1, ColliderBase* _coll2, const VECTOR3& _hitPos);

	//プレイヤーがダメージを食らった時
	void PlayerDamageBossEvent(ColliderBase* _coll1,ColliderBase* _coll2, const VECTOR3& _hitPos);
	void PlayerDamageBossEffectEvent(ColliderBase* _coll1,ColliderBase* _coll2, const VECTOR3& _hitPos);
	//ボスがダメージを食らった時
	void BossDamageEvent(ColliderBase* _coll1, ColliderBase* _coll2, const VECTOR3& _hitPos);
	void CameraPushEvent(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos);
	//ボスの子供で攻撃をするのが当たった時
	void PlayerDamageBossChildEvent(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos);
	//ボスの岩の予測線を出す。
	void BossRockPrePosition(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos);
	void BossRockGround(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos);
	void PlayerAttackRock(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos);

	void BossRockDamage(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos);
	void BossRockRush(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos);
	void BossRockBlastDamagePlayer(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos);
	void BossRockBlastDamageBoss(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos);
	void JustAvoidEnemy(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos);
	void JustAvoidBoss(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos);

	void JustAvoid(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos);
	//void DamageEvent(ColliderBase* _coll1, ColliderBase* _coll2);

	using EventFunc = void (CollsionEvent::*)(ColliderBase*, ColliderBase*, Pushback&, const VECTOR3&);

	std::unordered_map<int, EventFunc> eventTable;

	int MakeKey(CollsionInformation::Tag t1, CollsionInformation::Tag t2);

	CollsionEventData SetEventData(ColliderBase* _myCollObj, ColliderBase* _targetCollObj, Pushback& _pushInfo, const VECTOR3& _hitPos);

};