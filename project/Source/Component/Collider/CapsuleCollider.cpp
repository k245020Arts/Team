#include "CapsuleCollider.h"
#include "../Transform/Transform.h"
#include "../../Common/Debug/Debug.h"

CapsuleCollider::CapsuleCollider()
{
	debugId = 35;
	tag = Function::GetClassNameC<CapsuleCollider>();
	SetTag(tag);
	capsuleFrontTransform = nullptr;
}

CapsuleCollider::~CapsuleCollider()
{
	delete capsuleFrontTransform;
}

void CapsuleCollider::Update()
{
}

void CapsuleCollider::Draw()
{
	if (Debug::RayColliderDraw()) { //デバック表示モードがオフなら何もしない
		DrawCapsule3D(collTransform->WorldTransform().position, capsuleFrontTransform->WorldTransform().position,radius,24, 0xff0000,0xff0000,true);
	}
}

void CapsuleCollider::Start()
{
}

void CapsuleCollider::CapsuleSet(const CollsionInfo& _info, const Transform& _backTransform, const Transform& _frontTransform, float _sphere)
{
	CapsuleSet(_info, _backTransform, _frontTransform,_sphere, "");
}

void CapsuleCollider::CapsuleSet(const CollsionInfo& _info, const Transform& _backTransform, const Transform& _frontTransform, float _sphere, std::string _tag)
{
	ColliderBase::CollsionAdd(_info, _backTransform, _tag);

	capsuleFrontTransform = new Transform(capsuleFrontTransform);
	capsuleFrontTransform->SetParent(_info.parentTransfrom);
}
