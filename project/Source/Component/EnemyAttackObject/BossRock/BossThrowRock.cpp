//#include "BossThrowRock.h"
//#include "../../Collider/SphereCollider.h"
//#include "../../Collider/RayCollider.h"
//#include "../../Collider/ModelCollider.h"
//#include "../../Collider/DountCollider.h"
//#include "../../MeshRenderer/MeshRenderer.h"
//#include "../../ComponentManager.h"
//#include "../../../Common/Effect/EffectManager.h"
//#include "../../../Common/Sound/SoundManager.h"
//#include "../../../Common/ResourceLoader.h"
//#include "../../Physics/Physics.h"
//#include "BossRockManager.h"
//#include "../../../Common/Random.h"
//
//BossThrowRock::BossThrowRock()
//{
//	throwRock = false;
//	throwReady = true;
//	throwRockBaseModel = -1;
//	dir = VZero;
//	boneNum = 0;
//	addPos = VZero;
//
//	debugId = 34;
//	tag = Function::GetClassNameC<BossThrowRock>();
//
//}
//
//BossThrowRock::~BossThrowRock()
//{
//}
//
//void BossThrowRock::Update()
//{
//	BossRockBase::Update();
//	//if (throwReady) {
//	//	Transform* transform = obj->GetTransform();
//	//	MATRIX matrixWorld = MV1GetFrameLocalWorldMatrix(throwRockBaseModel, boneNum);
//	//	// WORLDÀ•W‚ðŽæ“¾
//	//	VECTOR3 frameWorldPos = MV1GetFramePosition(throwRockBaseModel, boneNum);
//	//	MATRIX objWorldMat = obj->GetParent()->GetTransform()->WorldTransform().GetMatrix();
//	//	MATRIX invObjWorldMat = MInverse(objWorldMat);
//	//	VECTOR3 frashPosLocal = frameWorldPos * invObjWorldMat;
//	//	transform->position = frameWorldPos + addPos;
//	//}
//}
//
//void BossThrowRock::Draw()
//{
//}
//
//void BossThrowRock::Ground()
//{
//	BossRockBase::Ground();
//}
//
//void BossThrowRock::CreateThrowRock(VECTOR3& _addPos)
//{
//	CollsionInfo info;
//	info.parentTransfrom = obj->GetTransform();
//	info.shape = CollsionInformation::SPHERE;
//	info.oneColl = true;
//	info.tag = CollsionInformation::BOSS_ROCK_ATTACK;
//	playerHitColl = obj->Component()->AddComponent<SphereCollider>();
//	playerHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(200.0f, 1.0f, 1.0f)), "_rockAttack");
//
//	info.oneColl = false;
//	info.tag = CollsionInformation::JUST_AVOID;
//	if (justAvoidCollider == nullptr) {
//		justAvoidCollider = obj->Component()->AddComponent<SphereCollider>();
//		justAvoidCollider->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(450.0f, 1.0f, 1.0f)), "justAvoid_rock");
//	}
//
//	groundInit = false;
//
//	physics = obj->Component()->GetComponent<Physics>();
//	fly = false;
//	blast = false;
//	color = obj->Component()->GetComponent<Color>();
//
//	blinkBaseMax = 0.5f;
//	blinkCounter = blinkBaseMax;
//	ChangeColorMode = false;
//	nowBlast = false;
//	throwReady = true;
//
//	throwRockBaseModel = ResourceLoad::GetHandle(ID::B_MODEL);
//	boneNum = 15;
//	addPos = _addPos;
//}
//
//
//void BossThrowRock::DropRock()
//{
//	
//}
