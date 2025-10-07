#pragma once
#include "CameraStateBase.h"

class FollowCamera : public CameraStateBase
{
public:
	FollowCamera();
	~FollowCamera();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	Player* p;
	bool enemyShake;
	bool playerShake;
	float backCounter;

	const float TIMER_MAX = 0.5f;

	VECTOR3 targetPos;
	VECTOR3 currentTarget;
};