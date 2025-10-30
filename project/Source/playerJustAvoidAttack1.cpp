#include "playerJustAvoidAttack1.h"
#include "Animator.h"
#include "playerStateManager.h"
#include <typeinfo>
#include "controllerInputManager.h"
#include "player.h"
#include "Physics.h"
#include "camera.h"
#include "Easing.h"
#include "Shaker.h"
#include "function.h"
#include "EffectManager.h"
#include "SoundManager.h"
#include "color.h"
#include "inputManager.h"

PlayerJustAvoidAttack1::PlayerJustAvoidAttack1()
{
	string = Function::GetClassNameC<PlayerJustAvoidAttack1>();
	id = ID::P_ANIM_JUST_AVOID_ATTACK1;
	nextAttack = false;
	animId = ID::P_ANIM_JUST_AVOID_ATTACK1;
	collTrans = Transform(VECTOR3(0, 70, 200), VZero, VECTOR3(250, 0, 0));
	timer = 0.0f;
	nextAttackID = ID::P_ANIM_JUST_AVOID_ATTACK4;
	frontSpeed = 2000.0f;
	hitDamage = 1.0f;
}

PlayerJustAvoidAttack1::~PlayerJustAvoidAttack1()
{
}

void PlayerJustAvoidAttack1::Update()
{
	Player* p = GetBase<Player>();
	AttackCollsion();
	PlayerAttackStateBase::Update();
	if (noStateChange) {
		return;
	}
	if (distSize <= ATTACK_MOVE_DIST) {
		EnemyRotation();
	}
	timer -= Time::DeltaTimeRate();
	//’·‰Ÿ‚µ‚ð‚µ‚Ä‚¢‚éŠÔ‚Ìˆ—
	if (p->playerCom.anim->GetCurrentFrame() >= 10.0f && timer <= 0.0f) {
		//UŒ‚‚ðƒqƒbƒg‚³‚¹‚½‚¢‰ñ”‚ª‚Ü‚¾‚ ‚Á‚½‚çUŒ‚‚ð‘±‚¯‚é
		if (count > 0) {
			Again();
			count--;
			return;
		}
		runTimer = 0.1f;
		noStateChange = true;
		p->playerCom.anim->SetPlaySpeed(ATTACK_FINISH_ANIM_SPEED);
	}
	if (p->playerCom.InputManager->KeyInputDown("avoid")) {
		p->playerCom.player->AvoidReady();
		noStateChange = true;
	}
	if (p->playerCom.InputManager->KeyInput("attack")) {
		nextAttack = true;
	}
	else {
		runTimer = 0.3f;
		noStateChange = true;
		p->playerCom.color->setRGB(Color::Rgb(255, 255, 255, 255));
		p->playerCom.anim->SetPlaySpeed(ATTACK_FINISH_ANIM_SPEED);
		nextAttack = false;
	}
	//UŒ‚‚ðÅ‘å‚Ü‚Å‚¢‚Á‚½‚çƒvƒŒƒCƒ„[‚ðŽ~‚ß‚é
	if (count >= MAX_ATTACKNUM) {
		dist = targetTrans.position - p->playerCom.player->GetPlayerTransform()->position;
		if (dist.Size() <= DISTANCE_MOVE) {
			p->playerCom.anim->SetPlaySpeed(1.5f);
			p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA + VECTOR3(15000.0f, 15000.0f, 15000.0f));
		}
	}
}

void PlayerJustAvoidAttack1::Draw()
{
}

void PlayerJustAvoidAttack1::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	PlayerAttackStateBase::Start();
	animSpeed = 1.0f;
	count = MAX_ATTACKNUM;
	p->playerCom.anim->SetPlaySpeed(0.05f);
	firstColl = true;
	if (distSize <= ATTACK_MOVE_DIST) {
		p->playerCom.physics->SetVelocity(norm * distSize * 4.0f);
	}
}

void PlayerJustAvoidAttack1::Finish()
{
	Player* p = GetBase<Player>();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA);
	p->playerCom.player->DeleteCollision();
	p->playerCom.anim->AnimEventReset();
	PlayerAttackStateBase::Finish();
}

void PlayerJustAvoidAttack1::Again()
{
	Player* p = GetBase<Player>();
	//p->playerCom.anim->Play(ID::P_ANIM_IDOL);
	p->playerCom.player->DeleteCollision();
	firstColl = true;
	//PlayerStateBase::Start();
	if (distSize <= ATTACK_MOVE_DIST) {
		p->playerCom.physics->SetVelocity(norm * distSize * 2.5f);
	}
	animSpeed += 0.1f;
	p->playerCom.anim->SetPlaySpeed(0.0f);
	p->playerCom.anim->SetFrame(10.0f);
	timer = 0.12f;
	p->playerCom.sound->RandamSe("P_AttackV", 4);
}
