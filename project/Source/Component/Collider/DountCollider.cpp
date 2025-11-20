#include "DountCollider.h"

DountCollider::DountCollider()
{

}

DountCollider::~DountCollider()
{
}

void DountCollider::Update()
{
	collTransform->scale.x += speed;
	outRadius += speed;
}

void DountCollider::Draw()
{
	/*DrawSphere3D(collTransform->WorldTransform().position, collTransform->scale.x, 10, 0xff0000, 0xff0000, false);
	DrawSphere3D(collTransform->WorldTransform().position, outRadius, 10, 0xffff00, 0xffff00, false);*/
}

void DountCollider::DountSet(CollsionInfo _info, Transform _transform, float _outRadius, float _speed)
{
	ColliderBase::CollsionAdd(_info, _transform);

	outRadius = _outRadius;
	speed = _speed;
}
