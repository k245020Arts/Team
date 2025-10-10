#pragma once
#include "stateBase.h"

class FreeCamera : public StateBase
{
public:
	FreeCamera();
	~FreeCamera();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	float backCounter;

	const float TIMER_MAX = 0.5f;
	VECTOR3 currentTarget;
};
