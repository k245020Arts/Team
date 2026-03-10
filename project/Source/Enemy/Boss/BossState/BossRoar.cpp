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
	animId	= ID::B_ROAR;
	//id = ID::B_IDOL;
	string	= Function::GetClassNameC<BossRoar>();
	first	= false;
}

BossRoar::~BossRoar()
{
}

void BossRoar::Update()
{
	Boss* b = GetBase<Boss>();
	SoundManager::GetInstance()->Play3DSound(Sound_ID::V_B_ROAR, b->obj, 15000.0f, 1000.0f);
	if (b->enemyBaseComponent.anim->AnimEventCan()) {
		if (first) {
			//b->trashEnemy->CreateEnemy(VZero, 4);
			first = false;
			EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(0.0f,100.0f,0.0f),VZero,VOne), b->GetBaseObject(), Effect_ID::BOSS_ROAR, 2.0f);
		}
		
	}
	if (b->enemyBaseComponent.anim->IsFinish()) {
		b->enemyBaseComponent.state->SetNoStateChange(false);
		b->enemyBaseComponent.state->ChangeState(StateID::BOSS_HALF_ATTACK_S);
	}
}

void BossRoar::Draw()
{
}

void BossRoar::Start()
{
	Boss* b	= GetBase<Boss>();
	SoundManager::GetInstance()->PlaySe(Sound_ID::V_B_ROAR);
	first	= true;
	EnemyStateBase::Start();
	b->enemyBaseComponent.state->SetNoStateChange(true);
}

void BossRoar::Finish()
{
}
