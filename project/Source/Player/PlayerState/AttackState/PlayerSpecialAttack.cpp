#include "PlayerSpecialAttack.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../Component/Physics/Physics.h"
#include "../playerStateManager.h"
#include "../../../Common/InputManager/ControllerInputManager.h"
#include "../../player.h"
#include "../../../Common/function.h"
#include "../../../Component/MotionBlur/MotionBlur.h"
#include "../../../Component/Shaker/Shaker.h"
#include "../../../Common/Effect/EffectManager.h"
#include "../../../Component/Color/Color.h"
#include "../../../Common/InputManager/InputManager.h"
#include "../../../Common/Sound/SoundManager.h"
#include "../../../Component/Collider/SphereCollider.h"
#include "../../../Enemy/EnemyManager.h"
#include "../../../Camera/Camera.h"

PlayerSpecialAttack::PlayerSpecialAttack()
{
	string = Function::GetClassNameC<PlayerSpecialAttack>();
	//id = ID::P_ANIM_JUST_AVOID_ATTACK4;
	animId = ID::P_SPECIAL_ATTACK_ANIM;
	radius = 2000.0f;
	//TODO ìñÇΩÇËîªíËÇìÆÇ©Ç»Ç¢ÇÊÇ§Ç…êVÇµÇ¢ìñÇΩÇËîªíËÇÃê∂ê¨
	collTrans = Transform(VECTOR3(0, 100, 200), VZero, VECTOR3(radius, 0, 0));
	//frontSpeed = 1000.0f;
	hitDamage = 3.0f;
	

	moveNum = 0;
	state = NO_ATTACK;

	waitCounter = -1.0f;
	chargeCounter = -1.0f;
}

PlayerSpecialAttack::~PlayerSpecialAttack()
{
}

void PlayerSpecialAttack::Update()
{
	switch (state)
	{
	case PlayerSpecialAttack::NO_ATTACK:
		return;
		break;
	case PlayerSpecialAttack::BEFORE:
		BeforeUpdate();
		break;
	case PlayerSpecialAttack::GROUND_ATTACK:
		GroundUpdate();
		break;
	case PlayerSpecialAttack::CHARGE:
		ChargeUpdate();
		break;
	case PlayerSpecialAttack::FINAL_ATTACK:
		FinalAttackUpdate();
		break;
	default:
		break;
	}
	
}

void PlayerSpecialAttack::Draw()
{
	Player* p = GetBase<Player>();
	//DrawSphere3D(p->specialAttackCenterPos, 10, 1, 0xffffff, 0xffffff, true);
}

void PlayerSpecialAttack::Start()
{
	Player* p = GetBase<Player>();
	PlayerAttackStateBase::Start();
	firstColl = true;
	defalutRotation = p->playerTransform->rotation;
	//AttackCollsion();
	p->specialAttackStartPos = p->playerTransform->position;
	VECTOR3 forward = p->playerTransform->Forward();
	p->specialAttackCenterPos = p->specialAttackStartPos + forward * radius;
	MoveStart(0.0f);
	moveNum = 18;
	p->playerCom.camera->ChangeStateCamera(StateID::PLAYER_SPECIAL_ATTACK_CAMERA_S);
	p->playerCom.effect->CreateEffekseer(Transform(p->specialAttackCenterPos, VZero, VOne * 4.0f), nullptr, Effect_ID::PLAYER_SPECIAL_PLACE, 1.8f);
	p->playerCom.effect->CreateEffekseer(Transform(p->specialAttackCenterPos, VZero, VOne * 8.0f), nullptr, Effect_ID::PLAYER_SPECIAL_SLASH, 1.8f);

	state = GROUND_ATTACK;

	waitCounter = -1.0f;
	chargeCounter = -1.0f;
	p->noDamage = true;

}

void PlayerSpecialAttack::Finish()
{
	Player* p = GetBase<Player>();
	ColliderBase* collider = p->obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("special");
	//p->playerCom.camera->ChangeStateCamera(StateID::FREE_CAMERA_S);
	p->playerCom.effect->StopEffekseer(Effect_ID::PLAYER_SPECIAL_PLACE);
	p->playerCom.effect->StopEffekseer(Effect_ID::PLAYER_SPECIAL_SLASH);
	state = NO_ATTACK;
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->noDamage = false;
	p->obj->Component()->GetComponent<SphereCollider>()->CollsionRespown();
}

void PlayerSpecialAttack::MoveStart(float _angle)
{
	Player* p = GetBase<Player>();
	ColliderBase* collider = p->obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("special");
	AddCollsion();
	p->playerCom.sound->RandamSe("swordWind", 5);
	
	p->playerTransform->position = p->specialAttackCenterPos + VECTOR3(0.0f,0.0f,radius) * MGetRotY(p->playerTransform->rotation.y);
	float dist = VECTOR3(p->specialAttackCenterPos - p->playerTransform->position).Size();
	p->playerCom.physics->SetVelocity(VZero);
	//VECTOR3 forward = p->playerTransform->Forward() * MGetRotY(p->playerTransform->rotation.y);
	//p->playerCom.physics->SetVelocity(forward * 30000.0f);
}

void PlayerSpecialAttack::AddCollsion()
{
	Player* p = GetBase<Player>();
	ColliderBase* collider = p->obj->Component()->AddComponent<SphereCollider>();
	CollsionInfo info;
	info.parentTransfrom = nullptr;
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = false;
	info.tag = CollsionInformation::Tag::P_SPECIAL_ATTACK;
	info.size = 1.0f;
	collider->CollsionAdd(info, Transform(p->specialAttackCenterPos, VZero, VECTOR3(radius, 0, 0)),"special");
	p->playerCom.enemyManager->CanPlayerSpecialHit();
}

void PlayerSpecialAttack::StateImguiDraw()
{
	ImGui::Text("attackNum = %d", moveNum);
}

void PlayerSpecialAttack::BeforeUpdate()
{

}

void PlayerSpecialAttack::GroundUpdate()
{
	Player* p = GetBase<Player>();
	p->playerCom.player->DrawTrail(VECTOR3(0, 0, 100), VECTOR3(0, 0, -350), 0.0f, 0.0f, 255.0f, 150.0f, 28, 0.8f);
	if (p->playerCom.keyboard->GetIsKeyboardPut(KEY_INPUT_3)) {
		p->playerCom.stateManager->ChangeState(StateID::PLAYER_WAIT_S);
	}
	float  distance = VECTOR3(p->specialAttackCenterPos - p->playerTransform->position).Size();
	VECTOR3 forward = p->playerTransform->Forward() * MGetRotY(p->playerTransform->rotation.y);
	p->playerCom.physics->AddVelocity(forward * 1000000.0f, true);
	p->playerCom.blur->MosionStart(0.5f, 0.000001f, animId, 0);
	if (distance > radius + 200.0f) {
		if (moveNum > 0) {
			float angle = 36.0f * DegToRad;
			p->playerTransform->rotation.y += angle;
			MoveStart(angle * DegToRad);
			moveNum--;
		}
		else {
			p->playerTransform->position = p->specialAttackStartPos;
			//p->playerCom.stateManager->ChangeState(StateID::PLAYER_WAIT_S);
			ColliderBase* collider = p->obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("special");
			float angle = 36.0f * DegToRad;
			p->playerTransform->rotation = defalutRotation;
			p->playerCom.physics->SetVelocity(VZero);
			state = CHARGE;
			chargeCounter = 1.5f;
			AddCollsion();
			p->playerCom.anim->Play(animId);
			p->playerCom.anim->SetFrame(2.0f);
			p->playerCom.anim->SetMaxFrame(animId, 29.5f);
			p->playerCom.anim->SetPlaySpeed(0.0f);
			p->playerCom.shaker->ShakeStart(VOne * 5.0f, Shaker::MIX_SHAKE, false, -1);
			p->playerCom.camera->CutSceneChangeState("playerSpecialCut");
			p->obj->Component()->GetComponent<SphereCollider>()->CollsionFinish();
		}
	}
}

void PlayerSpecialAttack::ChargeUpdate()
{
	Player* p = GetBase<Player>();
	chargeCounter -= Time::DeltaTimeRate();
	Debug::DebugOutPutPrintf("charge : velocity.x = %.1f : velocity.y = %.1f : velocity.z = %.1f", p->playerCom.physics->GetVelocity().x, p->playerCom.physics->GetVelocity().y, p->playerCom.physics->GetVelocity().z);
	Debug::DebugOutPutPrintf("charge : position.x = %.1f : position.y = %.1f : position.z = %.1f", p->playerCom.player->playerTransform->position.x, p->playerCom.player->playerTransform->position.y, p->playerCom.player->playerTransform->position.z);
	p->playerCom.physics->SetVelocity(VZero);
	if (chargeCounter <= 0.0f) {
		MoveStart(0.0f);
		chargeCounter = 0.0f;
		state = FINAL_ATTACK;
		p->playerCom.anim->SetPlaySpeed(3.0f);
		p->playerCom.shaker->ShakeFinish();
	}
}

void PlayerSpecialAttack::FinalAttackUpdate()
{
	Player* p = GetBase<Player>();
	
	if (waitCounter >= 0.0f) {
		waitCounter -= Time::DeltaTimeRate();
		if (!p->playerCom.camera->IsCutScene()) {
			p->playerCom.stateManager->ChangeState(StateID::PLAYER_WAIT_S);
		}
		return;
	}
	p->playerCom.player->DrawTrail(VECTOR3(0, 0, 100), VECTOR3(0, 0, -350), 0.0f, 0.0f, 255.0f, 150.0f, 28, 0.8f);
	float  distance = VECTOR3(p->specialAttackCenterPos - p->playerTransform->position).Size();
	VECTOR3 forward = p->playerTransform->Forward();
	p->playerCom.physics->AddVelocity(forward * 15000.0f, true);
	if (distance > radius) {
		//ColliderBase* collider = p->obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("special");
		//p->playerTransform->position = forward * (radius);
		waitCounter = 1.0f;
		p->playerCom.physics->SetVelocity(VZero);
	}
}