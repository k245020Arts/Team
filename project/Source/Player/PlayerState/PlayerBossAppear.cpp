#include "PlayerBossAppear.h"
#include "../../Component/Animator/Animator.h"
#include "playerStateManager.h"
#include "../../Component/Color/Color.h"
#include "../../Component/Physics/Physics.h"
#include "../player.h"
#include "../../Component/Shaker/Shaker.h"
#include "../../Common/Function.h"
#include "../../Common/Sound/SoundManager.h"
#include "../../Camera/Camera.h"
#include "../../Common//Fead/Fead.h"

PlayerBossAppear::PlayerBossAppear()
{
	string = string = Function::GetClassNameC<PlayerBossAppear>();;
	animId = ID::P_ANIM_IDOL;
	fead = FindGameObject<Fead>();
}

PlayerBossAppear::~PlayerBossAppear()
{
}

void PlayerBossAppear::Update()
{
	if (!feadOut && !fead->IsFead()) {
		fead->FeadOut(1.0f, 0x000000, Easing::EaseIn<int>);
		feadOut = true;
	}
}

void PlayerBossAppear::Draw()
{
}

void PlayerBossAppear::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	feadOut = false;
}

void PlayerBossAppear::Finish()
{
}
