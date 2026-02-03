#include "BossAppear.h"
#include "../Boss.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "BossStatus.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Camera/Camera.h"
#include "../../../GameManager/GameManager.h"
#include "../../../Common/Fead/Fead.h"
#include "../../../Common/Effect/EffectManager.h"
#include "../../../Common/Sound/SoundManager.h"
#include "../../../Component/UI/UIManager/UIManager.h"

BossAppear::BossAppear()
{
	animId = ID::B_APPEAR_FALL;
	//id = ID::B_IDOL;
	string = Function::GetClassNameC<BossAppear>();
	fead = FindGameObject<Fead>();
	feedInFinish = false;
	uiManager = FindGameObject<UIManager>();
}

BossAppear::~BossAppear()
{
}

void BossAppear::Update()
{
	Boss* b = GetBase<Boss>();
	if (b->enemyBaseComponent.camera->GetCutNum() == 3) {
		if (!feedInFinish) {
			b->enemyBaseComponent.physics->GetBaseObject()->SetObjectTimeRate(1.0f);
			feedInFinish = true;
			b->enemyBaseComponent.sound->PlaySe(Sound_ID::BOSS_JUMP_WIND);
		}
		
	}
	if (b->enemyBaseComponent.camera->GetCutNum() == 5) {
		if (!roar) {
			b->enemyBaseComponent.anim->Play(ID::B_ROAR, 0.2f);
			roar = true;
		}
		
	}
	if (roar) {
		if (b->enemyBaseComponent.anim->AnimEventCan()) {
			if (first) {
				//b->trashEnemy->CreateEnemy(VZero, 4);
				first = false;
				b->enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(0.0f, 100.0f, 0.0f), VZero, VOne), b->GetBaseObject(), Effect_ID::BOSS_ROAR, 2.0f);
				b->enemyBaseComponent.sound->PlaySe(Sound_ID::BOSS_ROAR_VOICE);
				b->enemyBaseComponent.sound->FeedInOut(Sound_ID::BOSS_BEFORE, 0.5f);
			}

		}
		if (b->enemyBaseComponent.anim->IsFinish()) {
			b->enemyBaseComponent.gameManager->ChangeState("PLAY");
			b->enemyBaseComponent.sound->FeedInStart(Sound_ID::PLAY_BGM, 1.0f);
		}
	}

	if (feedInFinish) {
		b->enemyBaseComponent.physics->AddGravity(VECTOR3(0, -80.0f, 0));
		if (b->enemyBaseComponent.physics->GetGround()) {
			//b->enemyBaseComponent.gameManager->ChangeState("PLAY");
			b->enemyBaseComponent.anim->Play(ID::B_APPEAR_LAND);
			b->enemyBaseComponent.camera->CameraShake(VECTOR3(100, 100, 100), Shaker::MIX_SHAKE, false, 1.0f);
			feedInFinish = false;
			b->enemyBaseComponent.sound->PlaySe(Sound_ID::ENEMY_FALL);
		}
		if (!b->enemyBaseComponent.camera->IsCutScene()) {
			
		}
	}
}

void BossAppear::Draw()
{
}

void BossAppear::Start()
{
	Boss* b = GetBase<Boss>();
	EnemyStateBase::Start();
	obj->GetTransform()->position = VECTOR3(0, 10000, 2000);
	b->enemyBaseComponent.physics->GetBaseObject()->SetObjectTimeRate(0.0f);
	feedInFinish = false;
	roar = false;
	first = true;
	uiManager->SetUIDraw(false);
	b->enemyBaseComponent.sound->FeedInOut(Sound_ID::PLAY_BGM, 0.5f);
	b->enemyBaseComponent.sound->PlayBGM(Sound_ID::BOSS_BEFORE,true,true);
	b->enemyBaseComponent.sound->FeedInStart(Sound_ID::BOSS_BEFORE,0.5f);
}

void BossAppear::Finish()
{
}
