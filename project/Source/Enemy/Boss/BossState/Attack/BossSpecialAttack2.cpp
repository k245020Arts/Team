#include "BossSpecialAttack2.h"
#include "../../Boss.h"
#include "../../../../State/StateManager.h"
#include "../../../../Component/Animator/Animator.h"
#include "../../../../Component/Physics/Physics.h"

BossSpecialAttack2::BossSpecialAttack2()
{
	//id = ID::B_S_ATTACK2;
	string = Function::GetClassNameC<BossSpecialAttack2>();
	animId = ID::B_S_ATTACK2_BEFORE;
	collTrans = Transform(VECTOR3(0, 0, -50), VZero, VECTOR3(500.0f, 0.0f, 0.0f));
}

BossSpecialAttack2::~BossSpecialAttack2()
{
}

void BossSpecialAttack2::Update()
{
	Boss* b = GetBase<Boss>();
	if (b->enemyBaseComponent.anim->IsFinish()) {
		if (b->enemyBaseComponent.anim->GetCurrentID() == ID::GetID(ID::B_S_ATTACK2_STOP)) {
			if (b->maxAttack != -1)
				b->enemyBaseComponent.state->ChangeState(StateID::ATTACK_SORTING_S);
			else
				b->enemyBaseComponent.state->ChangeState(StateID::BOSS_RUN_S);
		}
		else {
			b->enemyBaseComponent.anim->Play(ID::B_S_ATTACK2);
			AttackStart();
		}
	}
	if (b->enemyBaseComponent.anim->GetCurrentID() == ID::GetID(ID::B_S_ATTACK2_BEFORE)) {
		b->LookPlayer(0.09f);
		return;
	}
	attackCount -= Time::DeltaTimeRate();
	if (attackCount <= 0.0f) {
		b->enemyBaseComponent.anim->Play(ID::B_S_ATTACK2_STOP);
		VECTOR3 p = b->enemyBaseComponent.physics->GetVelocity().Normalize();
		b->enemyBaseComponent.physics->AddVelocity(p * -5000.0f, true);
		return;
	}
	//b->enemyBaseComponent.physics->AddVelocity(rotation * 10000.0f, true);
	BossAttackCollsion();
	VECTOR3 pos = b->enemyBaseComponent.playerObj->GetTransform()->position;
	VECTOR3 angle = pos - b->GetBaseObject()->GetTransform()->position;
	distance = angle.Size();
	b->enemyBaseComponent.physics->AddVelocity(VECTOR3(0, 0, -11000) * b->GetEnemyObj()->GetTransform()->GetRotationMatrix(), true);
	if (!look) {
		return;
	}
	if (distance > 2000.0f) {
		b->LookPlayer(0.2f);
	}
	else {
		look = false;
	}

}

void BossSpecialAttack2::Draw()
{

}

void BossSpecialAttack2::Start()
{
	EnemyStateBase::Start();
	
}

void BossSpecialAttack2::Finish()
{
	EnemyStateBase::Start();
	Boss* b = GetBase<Boss>();
	b->enemyBaseComponent.physics->SetVelocity(VECTOR3(0, 0, -1000) * b->GetEnemyObj()->GetTransform()->GetRotationMatrix());
	b->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
}

void BossSpecialAttack2::AttackStart()
{
	Boss* b = GetBase<Boss>();
	attackCount = 1.5f;
	//b->enemyBaseComponent.physics->AddVelocity(VECTOR3(0, 3000, 0), false);
	VECTOR3 pos = b->enemyBaseComponent.playerObj->GetTransform()->position;

	VECTOR3 angle = pos - b->GetBaseObject()->GetTransform()->position;
	rotation = angle.Normalize();
	firstColl = true;
	look = true;
	distance = pos.Size();
}
