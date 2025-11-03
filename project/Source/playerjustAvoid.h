#pragma once
#include "playerStateBase.h"
#include "transform.h"

class BlurScreen;

class PlayerJustAvoid : public PlayerStateBase
{
public:
	PlayerJustAvoid();
	~PlayerJustAvoid();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

	void JustAvoidShadow();
	void JustAvoidRotation();

private:
	static const int SHADOW_NUM_MAX = 10;
	int shadowNum;
	bool avoidShadowStart;
	float easingCount;
	float cameraAngle;
	VECTOR3 cameraRotation;
	float cameraLeap;
	bool attack;
	int num;

	BlurScreen* blurScreen;
	float startTimer;
};
