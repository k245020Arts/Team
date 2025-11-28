#include "BossSpecialSmallAttack1.h"
#include "../../Boss.h"
#include "../../../../State/StateManager.h"
#include "../../../../Component/Animator/Animator.h"
#include "../../../../Component/Physics/Physics.h"
#include "../../../../Common/Effect/EffectManager.h"
#include "../../../../Component/Collider/DountCollider.h"
#include "../../../../Component/EnemyAttackObject/ShockWave/ShockWave.h"

BossSpecialSmallAttack1::BossSpecialSmallAttack1()
{
	//id = ID::B_S_ATTACK1_SMALL;
	string = Function::GetClassNameC<BossSpecialSmallAttack1>();
	animId = ID::B_S_ATTACK1_SMALL;

	collTrans = Transform(VECTOR3(0, -50, 0), VZero, VECTOR3(500.0f, 0.0f, 0.0f));
}

BossSpecialSmallAttack1::~BossSpecialSmallAttack1()
{
}

void BossSpecialSmallAttack1::Update()
{
	Boss* b = GetBase<Boss>();

	if (b->enemyBaseComponent.anim->GetCurrentFrame() <= attackStart) {
		return;
	}
	else {
		if (firstCount) {
			firstCount = false;
			b->enemyBaseComponent.physics->AddVelocity(VECTOR3(0, 3000, 0), false);
			subSpeed = 0;
			VECTOR3 pos = b->enemyBaseComponent.playerObj->GetTransform()->position;

			VECTOR3 sub = pos - b->GetBaseObject()->GetTransform()->position;
			b->enemyBaseComponent.physics->AddVelocity(sub, false);
		}
	}

	b->enemyBaseComponent.physics->AddGravity(VECTOR3(0, -subSpeed, 0));
	subSpeed += 100.0f;

	if (b->enemyBaseComponent.physics->GetGround()) {
		if (b->enemyBaseComponent.anim->GetCurrentFrame() >= 40) {
			if (effect) {
				effect = false;
				BaseObject* obj1 = b->enemyBaseComponent.effect->CreateEffekseer(*b->GetBaseObject()->GetTransform(), b->GetBaseObject(), Effect_ID::BOSS_WAVE, 1.0f);
				ShockWave* w = obj1->Component()->AddComponent<ShockWave>();
				b->enemyBaseComponent.effect->ParentTransformRemove(obj1);
				w->CreateWave(CollsionInformation::B_E_ATTACK, Transform(VZero, VZero, VOne), 50.0f, 50.0f);
			}
		}

		if (b->enemyBaseComponent.anim->IsFinish()) {
			if (b->maxAttack != -1)
				b->enemyBaseComponent.state->ChangeState(StateID::ATTACK_SORTING_S);
			else
				b->enemyBaseComponent.state->ChangeState(StateID::BOSS_RUN_S);
		}
	}
	if (b->enemyBaseComponent.anim->GetCurrentFrame() <= b->enemyBaseComponent.anim->EventFinishTime(animId)) {
		VECTOR3 pos = b->enemyBaseComponent.playerObj->GetTransform()->position;
		VECTOR3 sub = pos - b->GetBaseObject()->GetTransform()->position;
		VECTOR3 ynotPos = sub * VECTOR3(1, 0, 1);
		float size = ynotPos.Size();
		VECTOR3 move = ynotPos.Normalize() * size;
		move.y = b->enemyBaseComponent.physics->GetVelocity().y;
		b->enemyBaseComponent.physics->SetVelocity(move);
		b->LookPlayer();
	}
	else {
		b->enemyBaseComponent.physics->SetVelocity(VZero);
	}


	BossAttackCollsion();
}

void BossSpecialSmallAttack1::Draw()
{
}

void BossSpecialSmallAttack1::Start()
{
	EnemyStateBase::Start();
	Boss* b = GetBase<Boss>();

	b->enemyBaseComponent.anim->AnimEventReset();
	firstColl = true;
	attackStart = 30.0f;
	firstCount = true;
	effect = true;
	b->enemyBaseComponent.anim->SetMaxFrame(animId, 60.0f);

	if (b->comboFirstAttack)
		b->enemyBaseComponent.anim->SetFrame(0.0f);
	else
		b->enemyBaseComponent.anim->SetFrame(30.0f);
}

void BossSpecialSmallAttack1::Finish()
{
	Boss* b = GetBase<Boss>();
	b->enemyBaseComponent.physics->SetGravity(VECTOR3(0, -1500, 0));
	b->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
}
