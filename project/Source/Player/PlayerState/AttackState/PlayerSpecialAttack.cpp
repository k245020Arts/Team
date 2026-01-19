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
#include "../../../Stage/SkyManager.h"

PlayerSpecialAttack::PlayerSpecialAttack()
{
	string = Function::GetClassNameC<PlayerSpecialAttack>();
	//id = ID::P_ANIM_JUST_AVOID_ATTACK4;
	animId = ID::P_SPECIAL_ATTACK_ANIM;
	radius = 2000.0f;
	//TODO 当たり判定を動かないように新しい当たり判定の生成
	collTrans = Transform(VECTOR3(0, 100, 200), VZero, VECTOR3(radius, 0, 0));
	//frontSpeed = 1000.0f;
	hitDamage = 3.0f;
	

	moveNum = 0;
	state = NO_ATTACK;

	waitCounter = -1.0f;
	chargeCounter = -1.0f;

	keepPos = VZero;
	specialAngle = 0.0f;
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
	p->playerCom.anim->Play(ID::P_SPECIAL_ATTACK_BEFORE_ANIM);
	firstColl = true;
	defalutRotation = p->playerTransform->rotation;
	//p->playerCom.camera->CutSceneChangeState("playerSpecialAttackBefore", CutSceneSpece::ALL_ENEMY);
	//AttackCollsion();

	state = BEFORE;
	p->playerCom.anim->SetPlaySpeed(1.0f);

	waitCounter = -1.0f;
	chargeCounter = -1.0f;
	p->noDamage = true;

	p->playerTransform->rotation.z += 90.0f * DegToRad;

	keepPos = p->playerTransform->position;

	p->playerTransform->position = CUT_SCENE_POS;
	specialAngle = 0.0f;
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

	/*VECTOR3 forward = p->playerTransform->Forward() * MGetRotY(p->playerTransform->rotation.y);
	p->playerCom.physics->AddVelocity(forward * 50000.0f, true);*/

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
	Player* p = GetBase<Player>();
	if (!p->playerCom.camera->IsCutScene()) {
		state = GROUND_ATTACK;
		p->playerTransform->position = keepPos;
		p->specialAttackStartPos = p->playerTransform->position;
		VECTOR3 forward = p->playerTransform->Forward();
		p->specialAttackCenterPos = p->specialAttackStartPos + forward * radius;
		MoveStart(0.0f);
		moveNum = 18;
		p->playerCom.camera->ChangeStateCamera(StateID::PLAYER_SPECIAL_ATTACK_CAMERA_S);
		//p->playerCom.effect->CreateEffekseer(Transform(p->specialAttackCenterPos, VZero, VOne * 4.0f), nullptr, Effect_ID::PLAYER_SPECIAL_PLACE, 1.8f);
		//p->playerCom.effect->CreateEffekseer(Transform(p->specialAttackCenterPos, VZero, VOne * 8.0f), nullptr, Effect_ID::PLAYER_SPECIAL_SLASH, 1.8f);
	}
}
#define MODE_1
void PlayerSpecialAttack::GroundUpdate()
{
#ifdef MODE_1
	Player* p = GetBase<Player>();

	specialAngle += 1000.0f * DegToRad * Time::DeltaTimeRate();

	// 中心から円運動
	VECTOR3 offset;
	offset.x = cosf(specialAngle) * radius;
	offset.z = sinf(specialAngle) * radius;
	offset.y = 0.0f;

	p->playerTransform->position = p->specialAttackCenterPos + offset;

	// 向きは進行方向に合わせる（重要）
	float yaw = atan2f(offset.z, offset.x) + DX_PI_F / 2.0f;
	p->playerTransform->rotation.y = yaw;

	// トレイルや演出
	p->playerCom.player->DrawTrail(VECTOR3(0, 0, 100), VECTOR3(0, 0, -350), 0.0f, 0.0f, 255.0f, 150.0f, 28, 0.8f);

	moveNum--;
	if (moveNum <= 0)
	{
		// 次フェーズへ
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
		p->playerCom.shaker->ShakeStart(VOne * 5.0f, Shaker::MIX_SHAKE, false, -1);
		p->playerCom.camera->CutSceneChangeState("playerSpecialCut");
		p->obj->Component()->GetComponent<SphereCollider>()->CollsionFinish();
	}
	else {
		// 描画用角度
		yaw = atan2(dir.z, dir.x);
		p->playerTransform->rotation.y = yaw;
	}

	

#else
	Player* p = GetBase<Player>();
	p->playerCom.player->DrawTrail(VECTOR3(0, 0, 100), VECTOR3(0, 0, -350), 0.0f, 0.0f, 255.0f, 150.0f, 28, 0.8f);
	if (p->playerCom.keyboard->GetIsKeyboardPut(KEY_INPUT_3)) {
		p->playerCom.stateManager->ChangeState(StateID::PLAYER_WAIT_S);
	}

	float  distance = VECTOR3(p->specialAttackCenterPos - p->playerTransform->position).Size();
	VECTOR3 forward = p->playerTransform->Forward() * MGetRotY(p->playerTransform->rotation.y);
	p->playerCom.physics->AddVelocity(forward * 50000.0f, true);
	p->playerCom.blur->MosionStart(0.5f, 0.000001f, animId, 0);
	if (distance > radius + 200.0f) {
		if (moveNum > 0) {
			float angle = 0.0f;
			//if (moveNum % 2 == 0) {
				angle = 45.0f * DegToRad;
			//}
			//else {
				//angle = -90.0f * DegToRad;
			//}
			VECTOR3 norm = forward * radius;
			//p->playerTransform->position = norm;
			MoveStart(angle * DegToRad);
			p->playerTransform->rotation.y += angle;
			
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
			p->playerCom.shaker->ShakeStart(VOne * 5.0f, Shaker::MIX_SHAKE, false, -1);
			p->playerCom.camera->CutSceneChangeState("playerSpecialCut");
			p->obj->Component()->GetComponent<SphereCollider>()->CollsionFinish();
		}
	}
#endif // DEBUG
}

void PlayerSpecialAttack::ChargeUpdate()
{
	Player* p = GetBase<Player>();
	chargeCounter -= Time::DeltaTimeRate();
	Debug::DebugOutPutPrintf("charge : velocity.x = %.1f : velocity.y = %.1f : velocity.z = %.1f", p->playerCom.physics->GetVelocity().x, p->playerCom.physics->GetVelocity().y, p->playerCom.physics->GetVelocity().z);
	Debug::DebugOutPutPrintf("charge : position.x = %.1f : position.y = %.1f : position.z = %.1f", p->playerCom.player->playerTransform->position.x, p->playerCom.player->playerTransform->position.y, p->playerCom.player->playerTransform->position.z);
	p->playerCom.physics->SetVelocity(VZero);
	if (chargeCounter <= 0.0f) {
		p->playerTransform->position = p->specialAttackCenterPos + VECTOR3(0.0f, 0.0f, radius) * MGetRotY(p->playerTransform->rotation.y);
		MoveStart(0.0f);
		chargeCounter = 0.0f;
		state = FINAL_ATTACK;
		//p->playerCom.anim->SetPlaySpeed(3.0f);
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
		waitCounter = 10.0f;
		p->playerCom.physics->SetVelocity(VZero);
		p->playerCom.effect->CreateEffekseer(Transform(p->specialAttackCenterPos + VECTOR3(0.0f,150.0f,0.0f), VECTOR3(0.0f, 0.0f, 180.0f * DegToRad), VOne * 8.0f), nullptr, Effect_ID::PLAYER_SPECIAL_FINAL, 1.5f);
		//p->playerCom.effect->SetSpeedEffekseer(Effect_ID::PLAYER_SPECIAL_FINAL, 1.0f);
	}
}