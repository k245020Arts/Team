#include "BossAppear.h"
#include "../Boss.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "BossStatus.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Camera/Camera.h"
#include "../../../GameManager/GameManager.h"
#include "../../../Common/Fead/Fead.h"

BossAppear::BossAppear()
{
	animId = ID::B_S_ATTACK1;
	//id = ID::B_IDOL;
	string = Function::GetClassNameC<BossAppear>();
	fead = FindGameObject<Fead>();
}

BossAppear::~BossAppear()
{
}

void BossAppear::Update()
{
	Boss* b = GetBase<Boss>();
	if (fead->GetFeadState() == Fead::FEAD_OUT) {
		if (!fead->IsFead()) {
			b->enemyBaseComponent.physics->GetBaseObject()->SetObjectTimeRate(1.0f);
		}
	}
	b->enemyBaseComponent.physics->AddGravity(VECTOR3(0, -10.0f, 0));
	if (b->enemyBaseComponent.physics->GetGround()) {
		b->enemyBaseComponent.gameManager->ChangeState("PLAY");
	}
}

void BossAppear::Draw()
{
}

void BossAppear::Start()
{
	Boss* b = GetBase<Boss>();
	EnemyStateBase::Start();
	obj->GetTransform()->position = VECTOR3(0, 10000, 0);
	b->enemyBaseComponent.physics->GetBaseObject()->SetObjectTimeRate(0.0f);
}

void BossAppear::Finish()
{
}
