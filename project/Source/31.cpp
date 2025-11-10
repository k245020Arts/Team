#include "playerStateBase.h"
#include "playerStateManager.h"
#include "Animator.h"
#include "player.h"
#include "sphereCollider.h"
#include "Debug.h"
#include "SoundManager.h"
#include "controllerInputManager.h"
#include "inputManager.h"

PlayerStateBase::PlayerStateBase()
{
	firstColl = false;
	nextAttack = false;
	noStateChange = false;
	animId = ID::P_ANIM_IDOL;
}

PlayerStateBase::~PlayerStateBase()
{
}

void PlayerStateBase::Update()
{
}

void PlayerStateBase::Draw()
{

}

void PlayerStateBase::AvoidStart()
{
	Player* p = GetBase<Player>();
	p->playerCom.stateManager->ChangeState(ID::P_ANIM_AVOID);
	noStateChange = false;
}

void PlayerStateBase::Start() {
	Player* p = GetBase<Player>();
	p->playerCom.anim->Play(animId,0.1f);
	noStateChange = false;
	//Debug::DebugLog(string);
}

void PlayerStateBase::AttackCollsion()
{
	Player* p = GetBase<Player>();
	//アニメーションのイベントが始まったら攻撃の当たり判定を生成する。
	if (p->playerCom.anim->AnimEventCan()) {
		//最初の一回のみ生成したい
		if (firstColl) {
			firstColl = false;
			p->playerCom.player->CollsionStart<SphereCollider>(CollsionInformation::SPHERE, collTrans);
			p->playerCom.player->SetShape(CollsionInformation::SPHERE);
			p->playerCom.sound->RandamSe("swordWind", 5);
		}
	}
}

void PlayerStateBase::DefalutWalk()
{
	Player* p = GetBase<Player>();
	//移動処理
	p->playerCom.player->Move(20000.0f * Time::DeltaTimeRate(), 3200.0f);
	if (p->playerCom.InputManager->KeyInputDown("avoid")) {
		p->playerCom.player->AvoidReady();
	}
	if (p->playerCom.InputManager->KeyInputDown("attack")) {
		p->playerCom.stateManager->ChangeState(ID::P_ANIM_ATTACK1);
	}
}