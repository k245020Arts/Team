#pragma once
#include "CameraStateBase.h"

class JustAvoidCamera : public CameraStateBase
{
public:
	JustAvoidCamera();
	~JustAvoidCamera();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:

    VECTOR3 defalutDistance;
	VECTOR3 zoomDistance;
    VECTOR3 currentDistance;
    float speed;
	float changeTimer;

    float timer;
    const float MAX_TIMER = 0.05f;
	const float CHANGE_STATE_TIMER = 0.8f;
};