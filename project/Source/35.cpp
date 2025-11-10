#include "playerWalk.h"
#include "playerStateManager.h"
#include "Physics.h"
#include "transform.h"
#include "controllerInputManager.h"
#include "player.h"
#include "Animator.h"

PlayerWalk::PlayerWalk()
{

	string = string = Function::GetClassNameC<PlayerWalk>();;
	animId = ID::P_ANIM_RUN;;
	id = ID::P_ANIM_RUN;
}

PlayerWalk::~PlayerWalk()
{

}

void PlayerWalk::Update()
{
	Player* p = GetBase<Player>();
	if (CheckHitKey(KEY_INPUT_1)) {
		p->playerCom.stateManager->ChangeState(ID::P_ANIM_IDOL);
	}
	DefalutWalk();
}

void PlayerWalk::Draw()
{
	StateBase::Draw();
}

void PlayerWalk::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA);
}

void PlayerWalk::Finish()
{
	Player* p = GetBase<Player>();
	p->playerCom.anim->SetPlaySpeed(1.0f);
}
