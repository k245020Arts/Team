#include "BossRoar.h"
#include "../Boss.h"
#include "../../../State/StateManager.h"
#include "AttackSorting.h"
#include "BossStatus.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../Common/Sound/SoundManager.h"
#include "../../TrashEnemy/TrashEnemyManager.h"//
#include "../../../Common/Effect/EffectManager.h"

BossRoar::BossRoar()
{
	animId = ID::B_ROAR;
	//id = ID::B_IDOL;
	string = Function::GetClassNameC<BossRoar>();
}

BossRoar::~BossRoar()
{
}

void BossRoar::Update()
{
	Boss* b = GetBase<Boss>();
	if (b->enemyBaseComponent.anim->AnimEventCan()) {
		if (first) {
			b->trashEnemy->CreateEnemy(VECTOR3(0, 0, 0), 1);//
			first = false;
			b->enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(0.0f,100.0f,0.0f),VZero,VOne), b->GetBaseObject(), Effect_ID::BOSS_ROAR, 2.0f);
		}
		
	}
	if (b->enemyBaseComponent.anim->IsFinish()) {
		b->enemyBaseComponent.state->ChangeState(StateID::BOSS_SPECIAL_ATTACK1_S);
	}
}

void BossRoar::Draw()
{
}

void BossRoar::Start()
{
	Boss* b = GetBase<Boss>();
	b->enemyBaseComponent.sound->PlaySe(Sound_ID::V_B_ROAR);
	first = true;
	EnemyStateBase::Start();

}

void BossRoar::Finish()
{
}
