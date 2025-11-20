#pragma once
#include "ColliderBase.h"

class DountCollider : public ColliderBase
{
public:
	DountCollider();
	~DountCollider();

	void Update()override;
	void Draw()override;

	void DountSet(CollsionInfo _info, Transform _transform,float _outRadius,float _speed);

	float GetOutRadius() { return outRadius; }

private:

	float outRadius;

	float speed;

};