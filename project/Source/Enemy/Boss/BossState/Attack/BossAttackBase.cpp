#include "BossAttackBase.h"
#include "../../Boss.h"
#include "../../../../Component/Animator/Animator.h"

BossAttackBase::BossAttackBase()
{
	damage = DamagePattern();
}

BossAttackBase::~BossAttackBase()
{
}

void BossAttackBase::BossStart()
{
	Boss* boss = GetBase<Boss>();
	boss->threat = false;
	damage.flash = false;
}

void BossAttackBase::BossFinish()
{
	Boss* boss = GetBase<Boss>();
	if (boss->maxAttack < 0) {
		boss->threat = true;
	}
	boss->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
}
