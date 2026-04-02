#include "playerWait.h"
#include "playerStateManager.h"
#include "../../Component/Physics/Physics.h"
#include ".../../../../Component/Animator/Animator.h"
#include "../../GameControler/GameControler.h"
#include "../player.h"
#include "../../Camera/Camera.h"
#include "../../Common/Debug/DebugLogText.h"

PlayerWait::PlayerWait()
{

	string = string = Function::GetClassNameC<PlayerWait>();;
	animId = ID::P_ANIM_IDOL;
	//id = ID::P_ANIM_IDOL;
}

PlayerWait::~PlayerWait()
{

}

void PlayerWait::Update()
{
	Player* p = GetBase<Player>();
	if (CheckHitKey(KEY_INPUT_0)) {
		p->playerCom.stateManager->ChangeState(StateID::PLAYER_WALK_S);
	}
	if (!p->playerCom.camera->IsCutScene()) {
		DebugLogText::GetInstance()->Log(LogLevel::INFO,"pass");
		//assert(false);
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
	if (p->turn) {
		blendSpeed = 0.3f;
	}
	else {
		blendSpeed = 0.3f;
	}
	
	PlayerStateBase::Start();
	p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA);
	p->attackTargetTrans = Transform();
}

void PlayerWait::Finish()
{

}
