#include "BossThrowRock.h"
#include "../../Collider/SphereCollider.h"
#include "../../Collider/RayCollider.h"
#include "../../Collider/ModelCollider.h"
#include "../../Collider/DountCollider.h"
#include "../../MeshRenderer/MeshRenderer.h"
#include "../../ComponentManager.h"
#include "../../../Common/Effect/EffectManager.h"
#include "../../../Common/Sound/SoundManager.h"
#include "../../../Common/LoadManager.h"
#include "../../Physics/Physics.h"
#include "BossRockManager.h"
#include "../../../Common/Random.h"

BossThrowRock::BossThrowRock()
{
	throwRock = false;
	throwReady = true;
	throwRockBaseModel = -1;
	dir = VZero;
	boneNum = 0;
	addPos = VZero;

	debugId = 34;
	tag = Function::GetClassNameC<BossThrowRock>();

}

BossThrowRock::~BossThrowRock()
{
}

void BossThrowRock::Update()
{
	BossRockBase::Update();
	if (throwReady) {
		Transform* transform = obj->GetTransform();
		MATRIX matrixWorld = MV1GetFrameLocalWorldMatrix(throwRockBaseModel, boneNum);
		// WORLD座標を取得
		VECTOR3 frameWorldPos = MV1GetFramePosition(throwRockBaseModel, boneNum);
		MATRIX objWorldMat = obj->GetParent()->GetTransform()->WorldTransform().GetMatrix();
		MATRIX invObjWorldMat = MInverse(objWorldMat);
		VECTOR3 frashPosLocal = frameWorldPos * invObjWorldMat;
		transform->position = frameWorldPos + addPos;
	}
}

void BossThrowRock::Draw()
{
}

void BossThrowRock::Ground()
{
	BossRockBase::Ground();
}

void BossThrowRock::CreateThrowRock(VECTOR3& _addPos)
{
	CollsionInfo info;
	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = true;
	info.tag = CollsionInformation::BOSS_ROCK_ATTACK;
	playerHitColl = obj->Component()->AddComponent<SphereCollider>();
	playerHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(200.0f, 1.0f, 1.0f)), "_rockAttack");

	info.oneColl = false;
	info.tag = CollsionInformation::JUST_AVOID;
	if (justAvoidCollider == nullptr) {
		justAvoidCollider = obj->Component()->AddComponent<SphereCollider>();
		justAvoidCollider->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(450.0f, 1.0f, 1.0f)), "justAvoid_rock");
	}

	groundInit = false;

	physics = obj->Component()->GetComponent<Physics>();
	fly = false;
	blast = false;
	color = obj->Component()->GetComponent<Color>();

	blinkBaseMax = 0.5f;
	blinkCounter = blinkBaseMax;
	ChangeColorMode = false;
	nowBlast = false;
	throwReady = true;

	throwRockBaseModel = Load::GetHandle(ID::B_MODEL);
	boneNum = 15;
	addPos = _addPos;
}

void BossThrowRock::ThrowRockStart(BaseObject* _player)
{
	if (throwRock) {
		return;
	}
	dir = obj->GetParent()->GetTransform()->rotation.Normalize();
	throwReady = false;
	CollsionInfo info;
	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::RAY;
	info.oneColl = false;
	info.tag = CollsionInformation::BOSS_ROCK_F;
	if (randColl == nullptr) {
		randColl = obj->Component()->AddComponent<RayCollider>();
		randColl->RaySet(info, Transform(VECTOR3(0, 300, 0), VZero, VOne), Transform(VECTOR3(0, -300, 0), VZero, VOne));
	}

	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = true;
	info.tag = CollsionInformation::BOSS_ROCK_PLAYER_ATTACK;

	if (playerAttackHitColl == nullptr) {
		playerAttackHitColl = obj->Component()->AddComponent<SphereCollider>();
		playerAttackHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(250.0f, 1.0f, 1.0f)), "bossplayerAttack");
	}

	throwRock = true;
	//プレイヤーに向かって飛ぶ
	VECTOR3 start = obj->GetTransform()->position;
	VECTOR3 target = _player->GetTransform()->position;

	VECTOR3 diff = target - start;

	float g = 2000.0f;
	physics->SetGravity(VECTOR3(0, -g, 0));

	VECTOR3 diffXZ = VECTOR3(diff.x, 0, diff.z);
	float distance = diffXZ.Size();

	float height = diff.y;

	float angle = 30.0f * DegToRad;

	float cosA = cosf(angle);
	float sinA = sinf(angle);

	float numerator = g * distance * distance;
	float denominator = 2 * cosA * cosA * (distance * tanf(angle) - height);

	float speed = sqrtf(numerator / denominator);

	// 速度ベクトル
	VECTOR3 dir = diffXZ.Normalize();

	VECTOR3 velocity;
	velocity.x = dir.x * speed * cosA;
	velocity.z = dir.z * speed * cosA;
	velocity.y = speed * sinA;

	physics->AddVelocity(velocity, false);
}

void BossThrowRock::DropRock()
{
	if (throwRock) {
		return;
	}
	throwReady = false;
	CollsionInfo info;
	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::RAY;
	info.oneColl = false;
	info.tag = CollsionInformation::BOSS_ROCK_F;
	if (randColl == nullptr) {
		randColl = obj->Component()->AddComponent<RayCollider>();
		randColl->RaySet(info, Transform(VECTOR3(0, 300, 0), VZero, VOne), Transform(VECTOR3(0, -300, 0), VZero, VOne));
	}

	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = true;
	info.tag = CollsionInformation::BOSS_ROCK_PLAYER_ATTACK;

	if (playerAttackHitColl == nullptr) {
		playerAttackHitColl = obj->Component()->AddComponent<SphereCollider>();
		playerAttackHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(250.0f, 1.0f, 1.0f)), "bossplayerAttack");
	}

	throwRock = true;

	float g = 2000.0f;
	physics->SetGravity(VECTOR3(0, -g, 0));
}
