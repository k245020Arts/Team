#include "PlayerSpecialAttack.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../Component/Physics/Physics.h"
#include "../playerStateManager.h"
#include "../../../Common/InputManager/ControllerInputManager.h"
#include "../../player.h"
#include "../../../Common/function.h"
#include "../../../Component/Shaker/Shaker.h"
#include "../../../Common/Effect/EffectManager.h"
#include "../../../Component/Color/Color.h"
#include "../../../Common/InputManager/InputManager.h"
#include "../../../Common/Sound/SoundManager.h"
#include "../../../Component/Collider/SphereCollider.h"
#include "../../../Enemy/EnemyManager.h"

PlayerSpecialAttack::PlayerSpecialAttack()
{
	string = Function::GetClassNameC<PlayerSpecialAttack>();
	//id = ID::P_ANIM_JUST_AVOID_ATTACK4;
	animId = ID::P_ANIM_ATTACK1;
	radius = 2000.0f;
	//TODO ìñÇΩÇËîªíËÇìÆÇ©Ç»Ç¢ÇÊÇ§Ç…êVÇµÇ¢ìñÇΩÇËîªíËÇÃê∂ê¨
	collTrans = Transform(VECTOR3(0, 100, 200), VZero, VECTOR3(radius, 0, 0));
	//frontSpeed = 1000.0f;
	hitDamage = 5.0f;
	startPos = VZero;
	
	centerPos = VZero;
	moveNum = 0;
}

PlayerSpecialAttack::~PlayerSpecialAttack()
{
}

void PlayerSpecialAttack::Update()
{
	Player* p = GetBase<Player>();
	p->playerCom.player->DrawTrail(VECTOR3(0, 0, -100), VECTOR3(0, 0, -350), 0.0f, 0.0f, 255.0f, 150.0f, 28, 0.8f);
	if (p->playerCom.keyboard->GetIsKeyboardPut(KEY_INPUT_3)) {
		p->playerCom.stateManager->ChangeState(StateID::PLAYER_WAIT_S);
	}
	float  distance =  VECTOR3(centerPos - p->playerTransform->position).Size();

	if (distance > radius) {
		if (moveNum > 0) {
			float angle = 36.0f * DegToRad;
			p->playerTransform->rotation.y += angle;
			MoveStart(angle * DegToRad);
			moveNum--;
		}
		else {
			p->playerTransform->position = startPos;
			p->playerCom.stateManager->ChangeState(StateID::PLAYER_WAIT_S);
			p->playerCom.physics->SetVelocity(VZero);
		}
	
	}
}

void PlayerSpecialAttack::Draw()
{
	//DrawSphere3D(centerPos, radius, 1, 0xffffff, 0xffffff, true);
}

void PlayerSpecialAttack::Start()
{
	Player* p = GetBase<Player>();
	PlayerAttackStateBase::Start();
	firstColl = true;
	//AttackCollsion();
	startPos = p->playerTransform->position;
	VECTOR3 forward = p->playerTransform->Forward();
	centerPos = startPos + forward * radius;
	MoveStart(0.0f);
	moveNum = 36;
}

void PlayerSpecialAttack::Finish()
{
	Player* p = GetBase<Player>();
	ColliderBase* collider = p->obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("special");
}

void PlayerSpecialAttack::MoveStart(float _angle)
{
	Player* p = GetBase<Player>();
	ColliderBase* collider = p->obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("special");
	AddCollsion();
	p->playerCom.sound->RandamSe("swordWind", 5);
	
	p->playerTransform->position = centerPos + VECTOR3(0.0f,0.0f,radius) * MGetRotY(p->playerTransform->rotation.y);
	VECTOR3 forward = p->playerTransform->Forward() * MGetRotY(p->playerTransform->rotation.y);
	p->playerCom.physics->SetVelocity(forward * 30000.0f);
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
	collider->CollsionAdd(info, Transform(centerPos, VZero, VECTOR3(radius, 0, 0)),"special");
	p->playerCom.enemyManager->CanPlayerSpecialHit();
}

void PlayerSpecialAttack::StateImguiDraw()
{
	ImGui::Text("attackNum = %d", moveNum);
}
