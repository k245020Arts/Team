#include "BossRoar.h"
#include "../Boss.h"
#include "../../../State/StateManager.h"
#include "AttackSorting.h"
#include "BossStatus.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../Common/Sound/SoundManager.h"
#include "../../TrashEnemy/TrashEnemyManager.h"//

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
	EnemyStateBase::Start();

}

void BossRoar::Finish()
{
}
