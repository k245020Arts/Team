#include "playerWalk.h"
#include "playerStateManager.h"
#include "../../Component/Physics/Physics.h"
#include "../../Component/Transform/Transform.h"
#include "../../Common/InputManager/ControllerInputManager.h"
#include "../../Common/Sound/SoundManager.h"
#include "../player.h"
#include "../../Component/Animator/Animator.h"

PlayerWalk::PlayerWalk()
{

	string = string = Function::GetClassNameC<PlayerWalk>();;
	animId = ID::P_ANIM_RUN;;
	//id = ID::P_ANIM_RUN;
	avoidStart = false;
	sound = false;
	firstOnes = false;
	secondOnes = false;
}

PlayerWalk::~PlayerWalk()
{

}

void PlayerWalk::Update()
{
	Player* p = GetBase<Player>();
	if (CheckHitKey(KEY_INPUT_1)) {
		p->playerCom.stateManager->ChangeState(StateID::PLAYER_WAIT_S);
	}
	DefalutWalk();

	if (p->playerCom.anim->GetCurrentFrame() >= 5.0f && p->playerCom.anim->GetCurrentFrame() <= 6.0f) {
		if (firstOnes) {
			sound = true;
		}
		firstOnes = false;
	} 
	if (p->playerCom.anim->GetCurrentFrame() >= 17.0f && p->playerCom.anim->GetCurrentFrame() <= 18.0f) {
		if (secondOnes) {
			sound = true;
		}
		secondOnes = false;
	}
	if (sound) {
		p->playerCom.sound->PlayRamdomChangeFrequencySe(Sound_ID::PLAYER_WALK1,10000,60000);
		sound = false;
	}
	if (p->playerCom.anim->GetCurrentFrame() >= 19.0f) {
		firstOnes = true;
		secondOnes = true;
	}
}

void PlayerWalk::Draw()
{
	StateBase::Draw();
}

void PlayerWalk::Start()
{
	Player* p = GetBase<Player>();
	if (p->turn) {
		blendSpeed = 0.3f;
		p->turn = false;
	}
	else {
		blendSpeed = 0.2f;
	}
	
	PlayerStateBase::Start();
	p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA);

	
}

void PlayerWalk::Finish()
{
	Player* p = GetBase<Player>();
	p->playerCom.anim->SetPlaySpeed(1.0f);
}
