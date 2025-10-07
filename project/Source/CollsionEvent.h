#pragma once
#include "colliderBase.h"

class CollsionEvent
{
public:
	CollsionEvent();
	~CollsionEvent();

	//イベント発生
	void Event(ColliderBase* _coll1,ColliderBase* _coll2);

private:
	//プレイヤーがダメージを食らった時
	void PlayerDamageEvent(ColliderBase* _coll1, ColliderBase* _coll2);
	//敵がダメージを食らった時
	void EnemyDamageEvent(ColliderBase* _coll1, ColliderBase* _coll2);
	//void DamageEvent(ColliderBase* _coll1, ColliderBase* _coll2);
};