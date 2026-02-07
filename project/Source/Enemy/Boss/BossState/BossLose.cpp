#include "BossLose.h"
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

BossLose::BossLose()
{
	animId = ID::BOSS_DIE;
	//id = ID::BOSS_DIE;
	string = Function::GetClassNameC<BossLose>();
	//slowTime = 0.0f;
}

BossLose::~BossLose()
{
}

void BossLose::Update()
{
	Boss* b = GetBase<Boss>();
	if (!b->enemyBaseComponent.sound->CheckSe(Sound_ID::WIN)) {
		b->enemyBaseComponent.gameManager->ChangeState(GameManager::SCENE_CHANGE);
	}
}

void BossLose::Draw()
{
}

void BossLose::Start()
{
	Boss* b = GetBase<Boss>();
	EnemyStateBase::Start();
	b->enemyBaseComponent.sound->StopBGM(Sound_ID::PLAY_BGM);
	b->enemyBaseComponent.sound->PlaySe(Sound_ID::WIN);
	b->enemyBaseComponent.camera->CutSceneChangeState("PlayerWin", false);
}

void BossLose::Finish()
{
}
