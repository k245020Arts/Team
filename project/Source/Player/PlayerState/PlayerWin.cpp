#include "PlayerWin.h"
#include "../Player.h"
#include "../../Component/Animator/Animator.h"

PlayerWin::PlayerWin()
{
	string = string = Function::GetClassNameC<PlayerWin>();;
	animId = ID::P_WIN;;
	//id = ID::P_ANIM_RUN;
}

PlayerWin::~PlayerWin()
{
}

void PlayerWin::Update()
{
	Player* player = GetBase<Player>();
	if (player->playerCom.anim->IsFinish()) {
		player->playerCom.anim->Play(ID::PLAYER_WIN_IDOL);
	}
}

void PlayerWin::Draw()
{
}

void PlayerWin::Start()
{
	PlayerStateBase::Start();
}

void PlayerWin::Finish()
{
}
