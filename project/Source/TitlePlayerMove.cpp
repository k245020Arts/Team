#include "TitlePlayerMove.h"
#include "Component/Animator/Animator.h"


TitlePlayerMove::TitlePlayerMove()
{
	animId = ID::P_ANIM_AVOID;
	string = string = Function::GetClassNameC<TitlePlayerMove>();;
}

TitlePlayerMove::~TitlePlayerMove()
{
}

void TitlePlayerMove::Update()
{
}

void TitlePlayerMove::Draw()
{
}

void TitlePlayerMove::Start()
{
	TitlePlayer* player = GetBase<TitlePlayer>();
	player->playerCom.anim->Play(animId, blendSpeed);
	noStateChange = false;
}

void TitlePlayerMove::Finish()
{
}
