#include "PlayerLose.h"
#include "../../Component/Animator/Animator.h"
#include "../../GameManager/GameManager.h"
#include "../../Camera/Camera.h"
#include "../player.h"
#include "../../Common/Sound/SoundManager.h"
#include "../../Component/Collider/SphereCollider.h"
#include "../../Enemy/EnemyManager.h"

PlayerLose::PlayerLose()
{
	animId = ID::P_DIE;
	//id = ID::P_DIE;
	string = Function::GetClassNameC<PlayerLose>();
	//one = false;

}

PlayerLose::~PlayerLose()
{
}

void PlayerLose::Update()
{
	Player* p = GetBase<Player>();
	if (!p->playerCom.sound->CheckSe(Sound_ID::LOSE)) {
		p->playerCom.gameManager->ChangeState(GameManager::SCENE_CHANGE);
	}
}

void PlayerLose::Draw()
{
}

void PlayerLose::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	p->playerCom.sound->StopBGM(Sound_ID::PLAY_BGM);
	p->playerCom.sound->PlaySe(Sound_ID::LOSE);
	//one = false;
	p->playerCom.enemyManager->CameraRockOnStart(p->playerCom.camera);
	p->playerCom.camera->CutSceneChangeState("PlayerDie", false);
}

void PlayerLose::Finish()
{
}
