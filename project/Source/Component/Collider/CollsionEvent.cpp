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
	using namespace CollsionInformation;

	// Player ← Enemy attack
	eventTable[MakeKey(PLAYER, E_ATTACK)] = (EventFunc)&CollsionEvent::PlayerDamageEvent;

	// Enemy ← Player attack
	eventTable[MakeKey(ENEMY, P_ATTACK)] = (EventFunc)&CollsionEvent::EnemyDamageEvent;

	eventTable[MakeKey(ENEMY, P_SPECIAL_ATTACK)] = (EventFunc)&CollsionEvent::EnemyDamageEvent;


	// Player ← Boss attack
	eventTable[MakeKey(PLAYER, B_ATTACK)] = (EventFunc)&CollsionEvent::PlayerDamageBossEvent;


	// Player ← Boss effect attack
	eventTable[MakeKey(PLAYER, B_E_ATTACK)] = (EventFunc)&CollsionEvent::PlayerDamageBossEffectEvent;


	// Boss ← Player attack
	eventTable[MakeKey(BOSS, P_ATTACK)] = (EventFunc)&CollsionEvent::BossDamageEvent;

	eventTable[MakeKey(BOSS, P_SPECIAL_ATTACK)] = (EventFunc)&CollsionEvent::BossDamageEvent;


	// Camera push
	eventTable[MakeKey(FLOOR, C_FLOOR)] = (EventFunc)&CollsionEvent::CameraPushEvent;


	// Player ← BossRock attack
	eventTable[MakeKey(PLAYER, BOSS_ROCK_ATTACK)] = (EventFunc)&CollsionEvent::PlayerDamageBossChildEvent;
	eventTable[MakeKey(PLAYER, THROW_OBJECT_GROUND)] = (EventFunc)&CollsionEvent::PlayerDamageBossChildEvent;
	eventTable[MakeKey(PLAYER, THROW_OBJECT_GROUND_ONE_HIT)] = (EventFunc)&CollsionEvent::PlayerDamageBossChildEvent;
	eventTable[MakeKey(PLAYER, THROW_OBJECT_GROUND_NO_DAMAGE_REACTION)] = (EventFunc)&CollsionEvent::PlayerDamageBossChildEvent;

	// BossRock UI → floor
	eventTable[MakeKey(BOSS_ROCK_UI, FLOOR_AABB)] = (EventFunc)&CollsionEvent::BossRockPrePosition;


	// Rock landing
	eventTable[MakeKey(FLOOR, BOSS_ROCK_F)] = (EventFunc)&CollsionEvent::BossRockGround;


	// Player attack rock
	eventTable[MakeKey(BOSS_ROCK_PLAYER_ATTACK, P_ATTACK)] = (EventFunc)&CollsionEvent::PlayerAttackRock;


	// Rock → Boss damage
	eventTable[MakeKey(BOSS, BOSS_ROCK_DAMAGE)] = (EventFunc)&CollsionEvent::BossRockDamage;


	// Rock rush
	eventTable[MakeKey(BOSS_ROCK_RUSH, BOSS_RUSH)] = (EventFunc)&CollsionEvent::BossRockRush;


	// Rock blast damage player
	eventTable[MakeKey(PLAYER, ROCK_BLAST_DAMAGE)] = (EventFunc)&CollsionEvent::BossRockBlastDamagePlayer;


	// Rock blast damage boss
	eventTable[MakeKey(BOSS, ROCK_BLAST_DAMAGE)] = (EventFunc)&CollsionEvent::BossRockBlastDamageBoss;


	// Just avoid
	eventTable[MakeKey(PLAYER, JUST_AVOID)] = (EventFunc)&CollsionEvent::JustAvoid;


	// Just avoid enemy
	eventTable[MakeKey(PLAYER, JUST_AVOID_ENEMY)] = (EventFunc)&CollsionEvent::JustAvoidEnemy;


	// Just avoid boss
	eventTable[MakeKey(PLAYER, JUST_AVOID_BOSS)] = (EventFunc)&CollsionEvent::JustAvoidBoss;
}

CollsionEvent::~CollsionEvent()
{
}

void CollsionEvent::Event(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	using namespace CollsionInformation;
	using namespace CollsionInformation;

	Tag tag1 = _coll1->GetCollTag();
	Tag tag2 = _coll2->GetCollTag();

	int key = MakeKey(tag1, tag2);

	auto it = eventTable.find(key);

	if (it != eventTable.end())
	{
		(this->*(it->second))(_coll1, _coll2, resolver, _hitPos);
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
	if (boss->GetStateState() != nullptr) {
		int a = 0;
	}
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
	BossRockBase* rock = _coll1->GetObj()->Component()->GetComponent<BossRockBase>();

	rock->SetPreInfo(_hitPos);
	
}

void CollsionEvent::BossRockGround(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	BossRockBase* throwRock = _coll2->GetObj()->Component()->GetComponent<BossRockBase>();
	
	throwRock->Ground();
	
}

void CollsionEvent::PlayerAttackRock(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	Player* player = _coll2->GetObj()->Component()->GetComponent<Player>();
	BossRockBase* throwRock = _coll1->GetObj()->Component()->GetComponent<BossRockBase>();
	throwRock->PlayerAttackRockFlyAway(*player->GetPlayerTransform());

	player->AttackRockHit();
}

void CollsionEvent::BossRockDamage(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	Boss* boss = _coll1->GetObj()->Component()->GetComponent<Boss>();
	Physics* physics = _coll2->GetObj()->Component()->GetComponent<Physics>();

	boss->RockHitDamage(physics);
	

	BossRockBase* throwRock = _coll2->GetObj()->Component()->GetComponent<BossRockBase>();
	throwRock->RockBossHit();
	
}

void CollsionEvent::BossRockRush(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	Boss* boss = _coll2->GetObj()->Component()->GetComponent<Boss>();

	boss->RockHitRushDamage();
	BossRockBase* throwRock = _coll1->GetObj()->Component()->GetComponent<BossRockBase>();
	throwRock->GetBaseObject()->DestroyMe();
}

void CollsionEvent::BossRockBlastDamagePlayer(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	Player* player = _coll1->GetObj()->Component()->GetComponent<Player>();
	BossRockBase* throwRock = _coll2->GetObj()->Component()->GetComponent<BossRockBase>();


	if (throwRock->HitObjects(player->GetPlayerObj())) {
		return;
	}

	bool damage = player->EnemyAttackObjectHitIsPlayer(_coll2->GetBaseObject(),_coll2->GetCollTag());
	if (!damage) {
		_coll2->CollsionRespown();
	}
	throwRock->AddHitObj(player->GetPlayerObj());
}

void CollsionEvent::BossRockBlastDamageBoss(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	BossRockBase* throwRock = _coll2->GetObj()->Component()->GetComponent<BossRockBase>();
	Boss* boss = _coll1->GetObj()->Component()->GetComponent<Boss>();
	if (throwRock->HitObjects(boss->GetEnemyObj())) {
		return;
	}
	
	Physics* physics = _coll2->GetObj()->Component()->GetComponent<Physics>();

	boss->RockHitDamage(physics);
	throwRock->AddHitObj(boss->GetEnemyObj());
}

void CollsionEvent::JustAvoidEnemy(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	Player* player = _coll1->GetObj()->Component()->GetComponent<Player>();
	TrashEnemy* enemy = _coll2->GetObj()->Component()->GetComponent<TrashEnemy>();

	player->JustAvoidCollsionHit(enemy->GetBaseObject(), _coll2->GetCollTag());
}

void CollsionEvent::JustAvoidBoss(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	Player* player = _coll1->GetObj()->Component()->GetComponent<Player>();
	Boss* b = _coll2->GetObj()->Component()->GetComponent<Boss>();

	player->JustAvoidCollsionHit(b->GetBaseObject(), _coll2->GetCollTag());
}

void CollsionEvent::JustAvoid(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	Player* player = _coll1->GetObj()->Component()->GetComponent<Player>();
	//EnemyBase* enemy = _coll2->GetObj()->Component()->GetComponent<TrashEnemy>();
	BaseObject* _obj = _coll2->GetObj();
	if (_coll2->GetCollTag() == CollsionInformation::B_E_ATTACK) {
		if (_obj->GetParent() != nullptr) {
			_obj = _obj->GetParent();
		}
	}
	player->JustAvoidCollsionHit(_obj,_coll2->GetCollTag());
}

int CollsionEvent::MakeKey(CollsionInformation::Tag t1, CollsionInformation::Tag t2)
{
	return Function::EnumTag(t1, t2, CollsionInformation::TAG_MAX);
}
