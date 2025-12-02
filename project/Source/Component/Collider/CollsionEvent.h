#pragma once
#include "colliderBase.h"

class CollsionEvent
{
public:
	CollsionEvent();
	~CollsionEvent();

	//イベント発生
	void Event(ColliderBase* _coll1,ColliderBase* _coll2, Pushback& resolver);

private:
	//プレイヤーがダメージを食らった時
	void PlayerDamageEvent(ColliderBase* _coll1, ColliderBase* _coll2);
	//敵がダメージを食らった時
	void EnemyDamageEvent(ColliderBase* _coll1, ColliderBase* _coll2);

	//プレイヤーがダメージを食らった時
	void PlayerDamageBossEvent(ColliderBase* _coll1,ColliderBase* _coll2);
	void PlayerDamageBossEffectEvent(ColliderBase* _coll1,ColliderBase* _coll2);
	//ボスがダメージを食らった時
	void BossDamageEvent(ColliderBase* _coll1, ColliderBase* _coll2);
	void CameraPushEvent(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver);
	//void DamageEvent(ColliderBase* _coll1, ColliderBase* _coll2);
};