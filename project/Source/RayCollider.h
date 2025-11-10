#pragma once
#include "colliderBase.h"

class RayCollider : public ColliderBase
{
public:
	RayCollider();
	~RayCollider();

	void Update() override;
	void Draw() override;

	void Start()override;

	void RaySet(CollsionInfo _info, Transform _transform,Transform _transform2);
	Transform* Get2Transform() { return transform2; }

private:
	Transform* transform2;
};