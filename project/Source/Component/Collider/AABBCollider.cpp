#include "AABBCollider.h"

AABBCollider::AABBCollider()
{
	debugId = 1;
	tag = Function::GetClassNameC<AABBCollider>();
	SetTag(tag);
}

AABBCollider::~AABBCollider()
{
}

void AABBCollider::Update()
{
}

void AABBCollider::Draw()
{
}

void AABBCollider::SetUpAABBCollsion(const CollsionInfo& _info, const VECTOR3& _boxMin, const VECTOR3& _boxMax, std::string _tag)
{
	ColliderBase::CollsionAdd(_info, Transform(),_tag);
	aabbInfo = AABBInfo(_boxMin, _boxMax);
}

void AABBCollider::SetUpAABBCollsion(const CollsionInfo& _info, const VECTOR3& _boxMin, const VECTOR3& _boxMax)
{
	SetUpAABBCollsion(_info, _boxMin, _boxMax,"");
}
