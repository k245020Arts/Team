#include "rayCollider.h"
#include "../Transform/Transform.h"
#include "../../Common/Debug/Debug.h"

RayCollider::RayCollider()
{
	debugId = 20;
	tag = Function::GetClassNameC<RayCollider>();
	SetTag(tag);
}

RayCollider::~RayCollider()
{
	delete transform2;
}

void RayCollider::Update()
{
}

void RayCollider::Draw()
{
	if (Debug::RayColliderDraw()) { //デバック表示モードがオフなら何もしない
		DrawLine3D(collTransform->WorldTransform().position, transform2->WorldTransform().position, 0xff0000);
	}
}

void RayCollider::Start()
{
	
}

void RayCollider::RaySet(const CollsionInfo& _info, const Transform& _transform, const Transform& _transform2)
{
	RaySet(_info, _transform,_transform2, "");
}

void RayCollider::RaySet(const CollsionInfo& _info, const Transform& _transform, const Transform& _transform2, std::string _tag)
{
	ColliderBase::CollsionAdd(_info, _transform,_tag);

	transform2 = new Transform(_transform2);
	transform2->SetParent(_info.parentTransfrom);
}
