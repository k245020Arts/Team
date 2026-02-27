#include "CollsionEvent.h"
#include "../ComponentManager.h"
#include "../../Player/Player.h"
#include "../../Enemy/TrashEnemy/TrashEnemy.h"
#include "../../Common/Debug/Debug.h"
#include "../../Enemy/TrashEnemy/EnemyState/EnemyStateManager.h"
#include "../../Enemy/TrashEnemy/EnemyState/EnemyStateBase.h"
#include "../../Enemy/Boss/Boss.h"
#include "../../Common/Effect/EffectBase.h"
#include "../../Camera/Camera.h"
#include "../EnemyAttackObject/BossRock/BossRock.h"
#include "../EnemyAttackObject/BossRock/BossThrowRock.h"
#include "../Physics/Physics.h"

CollsionEvent::CollsionEvent()
{
}

CollsionEvent::~CollsionEvent()
{
}

void CollsionEvent::Event(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	using namespace CollsionInformation;
	Tag tag1 = _coll1->GetCollTag();
	Tag tag2 = _coll2->GetCollTag();
	if (tag1 == PLAYER && tag2 == E_ATTACK) {
		/*Debug::DebugLog("EnemyAttackHit");*/
		PlayerDamageEvent(_coll1, _coll2,_hitPos);
	}
	if (tag1 == ENEMY && (tag2 == P_ATTACK || tag2 == P_SPECIAL_ATTACK)) {
		/*Debug::DebugLog("PlayerAttackHit");*/
		EnemyDamageEvent(_coll1, _coll2, _hitPos);
	}
	if (tag1 == PLAYER && tag2 == B_ATTACK) {
		/*Debug::DebugLog("EnemyAttackHit");*/
		PlayerDamageBossEvent(_coll1, _coll2, _hitPos);
	}
	if (tag1 == PLAYER && tag2 == B_E_ATTACK) {
		/*Debug::DebugLog("EnemyAttackHit");*/
		PlayerDamageBossEffectEvent(_coll1, _coll2, _hitPos);
	}
	if (tag1 == BOSS && (tag2 == P_ATTACK || tag2 == P_SPECIAL_ATTACK)) {
		/*Debug::DebugLog("PlayerAttackHit");*/
		BossDamageEvent(_coll1, _coll2, _hitPos);
	}

	if (tag1 == FLOOR && tag2 == C_FLOOR) {
		CameraPushEvent(_coll1, _coll2, resolver, _hitPos);
	}

	if (tag1 == PLAYER && tag2 == BOSS_ROCK_ATTACK) {
		PlayerDamageBossChildEvent(_coll1, _coll2,resolver, _hitPos);
	}

	if (tag1 == BOSS_ROCK_UI && tag2 == FLOOR_AABB) {
		BossRockPrePosition(_coll1, _coll2, resolver, _hitPos);
	}

	if (tag1 == FLOOR && tag2 == BOSS_ROCK_F) {
		BossRockGround(_coll1, _coll2, resolver, _hitPos);
	}

	if (tag1 == BOSS_ROCK_PLAYER_ATTACK && tag2 == P_ATTACK) {
		PlayerAttackRock(_coll1, _coll2, resolver, _hitPos);
	}

	if (tag1 == BOSS && tag2 == BOSS_ROCK_DAMAGE) {
		BossRockDamage(_coll1, _coll2, resolver, _hitPos);
	}

	if (tag1 == BOSS_ROCK_RUSH && tag2 == BOSS_RUSH) {
		BossRockRush(_coll1, _coll2, resolver, _hitPos);
	}

	if (tag1 == PLAYER && tag2 == ROCK_BLAST_DAMAGE) {
		/*Debug::DebugLog("EnemyAttackHit");*/
		BossRockBlastDamagePlayer(_coll1, _coll2, resolver, _hitPos);
	}

	if (tag1 == BOSS && tag2 == ROCK_BLAST_DAMAGE) {
		/*Debug::DebugLog("EnemyAttackHit");*/
		BossRockBlastDamageBoss(_coll1, _coll2, resolver, _hitPos);
	}

	if (tag1 == PLAYER && tag2 == JUST_AVOID) {
		JustAvoid(_coll1, _coll2, resolver, _hitPos);
	}

	if (tag1 == PLAYER && tag2 == JUST_AVOID_ENEMY) {
		JustAvoidEnemy(_coll1, _coll2, resolver, _hitPos);
	}

	if (tag1 == PLAYER && tag2 == JUST_AVOID_BOSS) {
		JustAvoidBoss(_coll1, _coll2, resolver, _hitPos);
	}
}

void CollsionEvent::PlayerDamageEvent(ColliderBase* _coll1, ColliderBase* _coll2, const VECTOR3& _hitPos)
{
	Player* player =  _coll1->GetObj()->Component()->GetComponent<Player>();
	TrashEnemy* enemy = _coll2->GetObj()->Component()->GetComponent<TrashEnemy>();
	
	bool damage =  player->EnemyHit(enemy->GetStateManager()->GetState<EnemyStateBase>()->GetAnimId(),enemy->GetEnemyObj());
	if (!damage) {
		_coll2->CollsionRespown();
	}
}

void CollsionEvent::EnemyDamageEvent(ColliderBase* _coll1, ColliderBase* _coll2, const VECTOR3& _hitPos)
{
	Player* player = _coll2->GetObj()->Component()->GetComponent<Player>();
	TrashEnemy* enemy = _coll1->GetObj()->Component()->GetComponent<TrashEnemy>();
	//当たり判定に当たっていたら通さない、OneCollをtrueにすると一人の敵にしか当たらなくなるため使用しない
	if (enemy->GetHit()) {
		return;
	}
	enemy->PlayerHit();
	player->PlayerAttackHit();
}

void CollsionEvent::PlayerDamageBossEvent(ColliderBase* _coll1, ColliderBase* _coll2, const VECTOR3& _hitPos)
{
	Player* player = _coll1->GetObj()->Component()->GetComponent<Player>();
	Boss* b = _coll2->GetObj()->Component()->GetComponent<Boss>();

	bool damage = player->EnemyHit(b->GetStateManager()->GetState<EnemyStateBase>()->GetAnimId(), b->GetEnemyObj());
	if (!damage) {
		_coll2->CollsionRespown();
	}
}

void CollsionEvent::PlayerDamageBossEffectEvent(ColliderBase* _coll1, ColliderBase* _coll2, const VECTOR3& _hitPos)
{
	Player* player = _coll1->GetObj()->Component()->GetComponent<Player>();
	EffectBase* effect = _coll2->GetObj()->Component()->GetComponent<EffectBase>();
	Boss* b = effect->GetBaseObject()->GetParent()->Component()->GetComponent<Boss>();

	bool damage = player->EnemyAttackObjectHitIsPlayer(b->GetBaseObject(), _coll2->GetCollTag());
	if (!damage) {
		_coll2->CollsionRespown();
	}
}

void CollsionEvent::BossDamageEvent(ColliderBase* _coll1, ColliderBase* _coll2, const VECTOR3& _hitPos)
{
	Player* player = _coll2->GetObj()->Component()->GetComponent<Player>();
	Boss* boss = _coll1->GetObj()->Component()->GetComponent<Boss>();
	//当たり判定に当たっていたら通さない、OneCollをtrueにすると一人の敵にしか当たらなくなるため使用しない
	if (boss->GetHit()) {
		return;
	}
	boss->PlayerHit();
	player->PlayerAttackHit();
}

void CollsionEvent::CameraPushEvent(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	Camera* camera = _coll2->GetBaseObject()->Component()->GetComponent<Camera>();
	PushInfo info = resolver.GetPushInfo()[0];
	camera->PushCamera(info.normal, info.penetration, info.targetPos);
}

void CollsionEvent::PlayerDamageBossChildEvent(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	Player* player = _coll1->GetObj()->Component()->GetComponent<Player>();
	Boss* b = _coll2->GetObj()->GetParent()->Component()->GetComponent<Boss>();

	bool damage = player->EnemyAttackObjectHitIsPlayer(b->GetBaseObject(), _coll2->GetCollTag());
	if (!damage) {
		_coll2->CollsionRespown();
	}
}

void CollsionEvent::BossRockPrePosition(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	BossRock* rock = _coll1->GetObj()->Component()->GetComponent<BossRock>();

	rock->SetPreInfo(_hitPos);
	
}

void CollsionEvent::BossRockGround(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	BossRock* rock = _coll2->GetObj()->Component()->GetComponent<BossRock>();
	BossThrowRock* throwRock = _coll2->GetObj()->Component()->GetComponent<BossThrowRock>();
	if (rock != nullptr) {
		rock->Ground();
	}
	else {
		throwRock->Ground();
	}
	
}

void CollsionEvent::PlayerAttackRock(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	Player* player = _coll2->GetObj()->Component()->GetComponent<Player>();
	BossRock* rock = _coll1->GetObj()->Component()->GetComponent<BossRock>();
	BossThrowRock* throwRock = _coll1->GetObj()->Component()->GetComponent<BossThrowRock>();
	if (rock != nullptr) {
		rock->PlayerAttackRockFlyAway(*player->GetPlayerTransform());
	}
	else {
		throwRock->PlayerAttackRockFlyAway(*player->GetPlayerTransform());
	}
	player->AttackRockHit();
}

void CollsionEvent::BossRockDamage(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	Boss* boss = _coll1->GetObj()->Component()->GetComponent<Boss>();
	BossRock* rock = _coll2->GetObj()->Component()->GetComponent<BossRock>();
	Physics* physics = _coll2->GetObj()->Component()->GetComponent<Physics>();

	boss->RockHitDamage(physics);
	

	BossThrowRock* throwRock = _coll2->GetObj()->Component()->GetComponent<BossThrowRock>();
	if (rock != nullptr) {
		rock->RockBossHit();
	}
	else {
		throwRock->RockBossHit();
	}
}

void CollsionEvent::BossRockRush(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	Boss* boss = _coll2->GetObj()->Component()->GetComponent<Boss>();
	BossRock* rock = _coll1->GetObj()->Component()->GetComponent<BossRock>();
	

	boss->RockHitRushDamage();
	BossThrowRock* throwRock = _coll1->GetObj()->Component()->GetComponent<BossThrowRock>();
	if (rock != nullptr) {
		rock->GetBaseObject()->DestroyMe();
	}
	else {
		throwRock->GetBaseObject()->DestroyMe();
	}
}

void CollsionEvent::BossRockBlastDamagePlayer(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	BossRock* rock = _coll2->GetObj()->Component()->GetComponent<BossRock>();
	Player* player = _coll1->GetObj()->Component()->GetComponent<Player>();
	BossThrowRock* throwRock = _coll2->GetObj()->Component()->GetComponent<BossThrowRock>();

	if (rock != nullptr) {
		if (rock->HitObjects(player->GetPlayerObj())) {
			return;
		}
	}
	else {
		if (throwRock->HitObjects(player->GetPlayerObj())) {
			return;
		}
	}

	bool damage = player->EnemyAttackObjectHitIsPlayer(_coll2->GetBaseObject(),_coll2->GetCollTag());
	if (!damage) {
		_coll2->CollsionRespown();
	}

	if (rock != nullptr) {
		rock->AddHitObj(player->GetPlayerObj());
	}
	else {
		throwRock->AddHitObj(player->GetPlayerObj());
	}

}

void CollsionEvent::BossRockBlastDamageBoss(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	BossRock* rock = _coll2->GetObj()->Component()->GetComponent<BossRock>();
	BossThrowRock* throwRock = _coll2->GetObj()->Component()->GetComponent<BossThrowRock>();
	Boss* boss = _coll1->GetObj()->Component()->GetComponent<Boss>();
	if (rock != nullptr) {
		if (rock->HitObjects(boss->GetEnemyObj())) {
			return;
		}
	}
	else {
		if (throwRock->HitObjects(boss->GetEnemyObj())) {
			return;
		}
	}
	
	Physics* physics = _coll2->GetObj()->Component()->GetComponent<Physics>();

	boss->RockHitDamage(physics);
	
	if (rock != nullptr) {
		rock->AddHitObj(boss->GetEnemyObj());
	}
	else {
		throwRock->AddHitObj(boss->GetEnemyObj());
	}
}

void CollsionEvent::JustAvoidEnemy(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	Player* player = _coll1->GetObj()->Component()->GetComponent<Player>();
	TrashEnemy* enemy = _coll2->GetObj()->Component()->GetComponent<TrashEnemy>();

	player->JustAvoidCollsionHit(enemy->GetBaseObject());
}

void CollsionEvent::JustAvoidBoss(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	Player* player = _coll1->GetObj()->Component()->GetComponent<Player>();
	Boss* b = _coll2->GetObj()->Component()->GetComponent<Boss>();

	player->JustAvoidCollsionHit(b->GetBaseObject());
}

void CollsionEvent::JustAvoid(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	Player* player = _coll1->GetObj()->Component()->GetComponent<Player>();
	//EnemyBase* enemy = _coll2->GetObj()->Component()->GetComponent<TrashEnemy>();
	BaseObject* _obj = _coll2->GetObj();
	if (_obj->GetParent() == nullptr) {
		_obj = _obj->GetParent();
	}

	player->JustAvoidCollsionHit(_obj);
}
