#pragma once
#include "playerStateBase.h"
#include "../../Component/Transform/Transform.h"

class Animator;

class PlayerAvoid : public PlayerStateBase
{
public:
	PlayerAvoid();
	~PlayerAvoid();

	void Update()override;
	void Draw()override;

	void Start() override;
	void Finish()override;

	void StateChangeBase();
	

private:
	float time;
	float cameraAngle;
	float maxFrame;
};
