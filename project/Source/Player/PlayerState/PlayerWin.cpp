#include "PlayerWin.h"

PlayerWin::PlayerWin()
{
	string = string = Function::GetClassNameC<PlayerWin>();;
	animId = ID::P_ANIM_IDOL;;
	//id = ID::P_ANIM_RUN;
}

PlayerWin::~PlayerWin()
{
}

void PlayerWin::Update()
{
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
