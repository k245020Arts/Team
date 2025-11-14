#include "playerWait.h"
#include "playerStateManager.h"
#include "../../Component/Physics/Physics.h"
#include ".../../../../Component/Animator/Animator.h"
#include "../../GameManager/GameManager.h"
#include "../player.h"

PlayerWait::PlayerWait()
{

	string = string = Function::GetClassNameC<PlayerWait>();;
	animId = ID::P_ANIM_IDOL;
	id = ID::P_ANIM_IDOL;
}

PlayerWait::~PlayerWait()
{

}

void PlayerWait::Update()
{
	Player* p = GetBase<Player>();
	if (p->playerCom.gameManager->GetGameState() == &GameManager::BeforeUpdate) {
		return;
	}
	if (CheckHitKey(KEY_INPUT_0)) {
		p->playerCom.stateManager->ChangeState(ID::P_ANIM_RUN);
	}

	DefalutWalk();
}

void PlayerWait::Draw()
{
	StateBase::Draw();
}

void PlayerWait::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA);
}

void PlayerWait::Finish()
{

}
