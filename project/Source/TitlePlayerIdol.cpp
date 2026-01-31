#include "TitlePlayerIdol.h"
#include "Component/Animator/Animator.h"

TitlePlayerIdol::TitlePlayerIdol()
{
	animId = ID::PLAYER_WIN_IDOL;
	string = string = Function::GetClassNameC<TitlePlayerIdol>();;
}

TitlePlayerIdol::~TitlePlayerIdol()
{
}

void TitlePlayerIdol::Update()
{
	//‚±‚ÌŠÖ”‚ÅtitlePlayer‚Ìî•ñ‚ğ“Ç‚Ş
	//private‚Ì•Ï”‚ğ‚¢‚¶‚è‚½‚¢‚Æ‚«‚ÍtitlePlayer.h‚Åfriend Class‚ğg‚¢İ’è
	TitlePlayer* player = GetBase<TitlePlayer>();
}

void TitlePlayerIdol::Draw()
{
}

void TitlePlayerIdol::Start()
{
	//PlayerStateBase::Start();
	TitlePlayer* player = GetBase<TitlePlayer>();
	player->playerCom.anim->Play(animId, blendSpeed);
	noStateChange = false;
}

void TitlePlayerIdol::Finish()
{
}
