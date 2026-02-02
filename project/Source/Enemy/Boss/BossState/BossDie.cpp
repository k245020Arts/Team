#include "BossDie.h"
#include "../../../Camera/Camera.h"
#include "../../../Component/Shaker/Shaker.h"
#include "../Boss.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../GameManager/GameManager.h"
#include "../../../Common/Sound/SoundManager.h"
#include "../../../Component/Collider/ColliderBase.h"
#include "../../../Component/ComponentManager.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Component/Collider/ModelCollider.h"
#include "../../../Component/Collider/SphereCollider.h"

BossDie::BossDie()
{
	animId		= ID::BOSS_DIE;
	//id = ID::BOSS_DIE;
	string		= Function::GetClassNameC<BossDie>();
	one			= true;
	slowTime	= 0.0f;
}

BossDie::~BossDie()
{
}

void BossDie::Update()
{
	Boss* b = GetBase<Boss>();
	if (b->enemyBaseComponent.anim->IsFinish()) {
		b->enemyBaseComponent.gameManager->ChangeState("WIN");
		if (one) {
			b->enemyBaseComponent.sound->StopBGM(Sound_ID::PLAY_BGM);
			b->enemyBaseComponent.sound->PlaySe(Sound_ID::WIN);
			b->enemyBaseComponent.camera->CutSceneChangeState("PlayerWin",true);
			one = false;
		}
	
	}
	else {
		//éÄÇÒÇæèuä‘ÇÕÉXÉçÅ[çƒê∂ÇÇ≥ÇπÇÈ
		if (slowTime <= 0.0f) {
			Time::ChangeDeltaRate(1.0f);
			b->enemyBaseComponent.camera->CameraShakeStop();
			b->enemyBaseComponent.shaker->ShakeFinish();
		}
		else {
			slowTime -= Time::DeltaTime();
		}
	}
}

void BossDie::Draw()
{
}

void BossDie::Start()
{
	Boss* b		= GetBase<Boss>();
	EnemyStateBase::Start();
	Time::ChangeDeltaRate(0.1f);
	slowTime	= 0.5f;
	b->enemyBaseComponent.camera->CameraShake(VOne * 10.0f, Shaker::MIX_SHAKE, false, -1.0f);
	b->enemyBaseComponent.shaker->ShakeStart(VOne * 10.0f, Shaker::MIX_SHAKE, false, -1.0f);
	one			= true;
	//éÄÇÒÇæÇ∆Ç´Ç…ìñÇΩÇËîªíËÇè¡ÇµÇΩÇ¢ÇÃÇ≈ìñÇΩÇËîªíËÇè¡Ç∑ÅB
	obj->Component()->RemoveAllComponent<SphereCollider>();
	obj->Component()->RemoveAllComponent<ModelCollider>();
	b->enemyBaseComponent.physics->SetGravity(VZero);
}

void BossDie::Finish()
{
}
