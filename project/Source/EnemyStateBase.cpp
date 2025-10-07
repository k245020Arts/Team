#include "EnemyStateBase.h"
#include "Animator.h"
#include "enemy.h"
#include "transform.h"
#include "sphereCollider.h"

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
	Enemy* e = GetBase<Enemy>();
	e->eCom.anim->Play(animId);
}

void EnemyStateBase::AttackCollsion()
{
	Enemy* e = GetBase<Enemy>();
	//“G‚ÌUŒ‚”»’è‚Ì¶¬ƒNƒ‰ƒX
	if (e->eCom.anim->AnimEventCan()) {
		if (firstColl) {
			firstColl = false;
			e->eCom.enemy->CollsionStart<SphereCollider>(CollsionInformation::SPHERE,collTrans);
			e->eCom.enemy->SetShape(CollsionInformation::SPHERE);
		}
	}
}
