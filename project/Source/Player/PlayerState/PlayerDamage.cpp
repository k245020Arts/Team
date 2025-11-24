#include "playerDamage.h"
#include "../../Component/Animator/Animator.h"
#include "playerStateManager.h"
#include "../../Component/Color/color.h"
#include "../../Component/Physics/Physics.h"
#include "../../Component/Shaker/Shaker.h"
#include "../player.h"

PlayerDamage::PlayerDamage()
{

	string = string = Function::GetClassNameC<PlayerDamage>();;
	animId = ID::P_DAMAGE;
	//id = ID::P_DAMAGE;
}

PlayerDamage::~PlayerDamage()
{
}

void PlayerDamage::Update()
{
	Player* p = GetBase<Player>();
	if (p->playerCom.anim->IsFinish()) {
		p->playerCom.stateManager->ChangeState(StateID::PLAYER_WALK_S);
	}
}

void PlayerDamage::Draw()
{
}

void PlayerDamage::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	p->playerCom.color->setRGB(Color::Rgb(255.0f, 0.0f, 0.0f,255.0f));
	eRotation = p->playerCom.hitObj->GetTransform()->rotation;
	//p->playerCom.physics->SetVelocity(VECTOR3(0, 0, 3000) * MGetRotY(eRotation.y));
	p->playerCom.physics->SetVelocity(VECTOR3(0, 0, 3000) * MGetRotY(-eRotation.y));
	p->playerCom.shaker->ShakeStart(VECTOR3(10, 10, 10), Shaker::HORIZONAL_SHAKE, false, 0.5f);
}

void PlayerDamage::Finish()
{
	Player* p = GetBase<Player>();
	p->playerCom.color->setRGB(Color::Rgb(255.0f, 255.0f, 255.0f, 255.0f));
	p->playerCom.shaker->ShakeFinish();
	//p->playerCom.physics->SetGravity(VECTOR3(0, -150, 0));
}
