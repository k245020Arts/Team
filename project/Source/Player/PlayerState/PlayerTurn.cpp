#include "PlayerTurn.h"
#include "playerStateManager.h"
#include "../../Component/Physics/Physics.h"
#include "../../Component/Transform/Transform.h"
#include "../../Common/InputManager/ControllerInputManager.h"
#include "../player.h"
#include "../../Component/Animator/Animator.h"

PlayerTurn::PlayerTurn()
{
	string = string = Function::GetClassNameC<PlayerTurn>();;
	animId = ID::P_TURN_ANIM;
}

PlayerTurn::~PlayerTurn()
{
}

void PlayerTurn::Update()
{
	Player* p = GetBase<Player>();
	if (p->playerCom.anim->IsFinish()) {
		p->playerCom.stateManager->ChangeState(StateID::PLAYER_WALK_S);
	}
}

void PlayerTurn::Draw()
{
}

void PlayerTurn::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA);
	//blendSpeed = 0.4f;
}

void PlayerTurn::Finish()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA);
	//blendSpeed = 0.4f;
}
