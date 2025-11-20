#pragma once
#include "ColliderBase.h"

class DountCollider : public ColliderBase
{
public:
	DountCollider();
	~DountCollider();

	void Update()override;
	void Draw()override;

	void DountSet(CollsionInfo _info, Transform _transform,float _outRadius);

	float GetOutRadius() { return outRadius; }
	void AddOutRadius(float radius) { outRadius += radius; }

private:

	float outRadius;

	float speed;

};