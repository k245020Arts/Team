#pragma once
#include "colliderBase.h"

class AABBCollider : public ColliderBase
{
public:
	AABBCollider();
	~AABBCollider();

	void Update()override;
	void Draw()override;

private:

};