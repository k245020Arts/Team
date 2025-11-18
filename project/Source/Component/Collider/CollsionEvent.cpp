#include "CollsionEvent.h"
#include "../ComponentManager.h"
#include "../../Player/Player.h"
#include "../../Enemy/TrashEnemy/Enemy.h"
#include "../../Common/Debug/Debug.h"
#include "../../Enemy/TrashEnemy/EnemyState/EnemyStateManager.h"
#include "../../Enemy/TrashEnemy/EnemyState/EnemyStateBase.h"
#include "../../Enemy/Boss/Boss.h"
#include "../../Common/Effect/EffectBase.h"

CollsionEvent::CollsionEvent()
{
}

CollsionEvent::~CollsionEvent()
{
}

void CollsionEvent::Event(ColliderBase* _coll1, ColliderBase* _coll2)
{
	using namespace CollsionInformation;
	Tag tag1 = _coll1->GetCollTag();
	Tag tag2 = _coll2->GetCollTag();
	if (tag1 == PLAYER && tag2 == E_ATTACK) {
		/*Debug::DebugLog("EnemyAttackHit");*/
		PlayerDamageEvent(_coll1, _coll2);
	}
	if (tag1 == ENEMY && tag2 == P_ATTACK) {
		/*Debug::DebugLog("PlayerAttackHit");*/
		EnemyDamageEvent(_coll1, _coll2);
	}
	if (tag1 == PLAYER && tag2 == B_ATTACK) {
		/*Debug::DebugLog("EnemyAttackHit");*/
		PlayerDamageBossEvent(_coll1, _coll2);
	}
	if (tag1 == PLAYER && tag2 == B_E_ATTACK) {
		/*Debug::DebugLog("EnemyAttackHit");*/
		PlayerDamageBossEffectEvent(_coll1, _coll2);
	}
	if (tag1 == BOSS && tag2 == P_ATTACK) {
		/*Debug::DebugLog("PlayerAttackHit");*/
		BossDamageEvent(_coll1, _coll2);
	}
}

void CollsionEvent::PlayerDamageEvent(ColliderBase* _coll1, ColliderBase* _coll2)
{
	Player* player =  _coll1->GetObj()->Component()->GetComponent<Player>();
	Enemy* enemy = _coll2->GetObj()->Component()->GetComponent<Enemy>();
	
	bool damage =  player->EnemyHit(enemy->GetStateManager()->GetState<EnemyStateBase>()->GetAnimId(),enemy->GetEnemyObj());
	if (!damage) {
		_coll2->CollsionRespown();
	}
}

void CollsionEvent::EnemyDamageEvent(ColliderBase* _coll1, ColliderBase* _coll2)
{
	Player* player = _coll2->GetObj()->Component()->GetComponent<Player>();
	Enemy* enemy = _coll1->GetObj()->Component()->GetComponent<Enemy>();
	if (enemy->GetHit()) {
		return;
	}
	enemy->PlayerHit();
	player->PlayerAttackHit();
}

void CollsionEvent::PlayerDamageBossEvent(ColliderBase* _coll1, ColliderBase* _coll2)
{
	Player* player = _coll1->GetObj()->Component()->GetComponent<Player>();
	Boss* b = _coll2->GetObj()->Component()->GetComponent<Boss>();

	bool damage = player->EnemyHit(b->GetStateManager()->GetState<EnemyStateBase>()->GetAnimId(), b->GetEnemyObj());
	if (!damage) {
		_coll2->CollsionRespown();
	}
}

void CollsionEvent::PlayerDamageBossEffectEvent(ColliderBase* _coll1, ColliderBase* _coll2)
{
	Player* player = _coll1->GetObj()->Component()->GetComponent<Player>();
	EffectBase* effect = _coll2->GetObj()->Component()->GetComponent<EffectBase>();
	Boss* b = effect->GetBaseObject()->GetParent()->Component()->GetComponent<Boss>();

	bool damage = player->EnemyHit(b->GetStateManager()->GetState<EnemyStateBase>()->GetAnimId(), b->GetEnemyObj());
	if (!damage) {
		_coll2->CollsionRespown();
	}
}

void CollsionEvent::BossDamageEvent(ColliderBase* _coll1, ColliderBase* _coll2)
{
	Player* player = _coll2->GetObj()->Component()->GetComponent<Player>();
	Boss* boss = _coll1->GetObj()->Component()->GetComponent<Boss>();
	if (boss->GetHit()) {
		return;
	}
	boss->PlayerHit();
	player->PlayerAttackHit();
}
