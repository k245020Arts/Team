#include "EnemyStateBase.h"
#include "Animator.h"
#include "enemy.h"
#include "transform.h"
#include "sphereCollider.h"
#include "EnemyBase.h"
#include "Boss.h"

EnemyStateBase::EnemyStateBase()
{
	firstColl = false;
	animId = ID::E_GETUP;
}

EnemyStateBase::~EnemyStateBase()
{
}

void EnemyStateBase::Start()
{
	EnemyBase* e = GetBase<EnemyBase>();
	e->enemyBaseComponent.anim->Play(animId);
}

void EnemyStateBase::AttackCollsion()
{
	Enemy* e = GetBase<Enemy>();
	//“G‚ÌUŒ‚”»’è‚Ì¶¬ƒNƒ‰ƒX
	if (e->enemyBaseComponent.anim->AnimEventCan()) {
		if (firstColl) {
			firstColl = false;
			e->CollsionStart<SphereCollider>(CollsionInformation::SPHERE,collTrans);
			e->SetShape(CollsionInformation::SPHERE);
		}
	}
}

void EnemyStateBase::BossAttackCollsion()
{
	Boss* b = GetBase<Boss>();
	//“G‚ÌUŒ‚”»’è‚Ì¶¬ƒNƒ‰ƒX
	if (b->enemyBaseComponent.anim->AnimEventCan()) {
		if (firstColl) {
			firstColl = false;
			b->CollsionStart<SphereCollider>(CollsionInformation::SPHERE, collTrans);
			b->SetShape(CollsionInformation::SPHERE);
		}
	}
}
