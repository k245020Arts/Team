#pragma once
#include "TitlePlayer.h"
#include "Player/PlayerState/PlayerStateBase.h"

class TitleControl;
class TitlePlayer;
class BlurScreen;

class TitlePlayerMove : public PlayerStateBase
{
public:
	TitlePlayerMove();
	~TitlePlayerMove();
	void Update() override;
	void Draw() override;
	void Start() override;
	void Finish()override;

	float progress;
private:
	TitleControl* titleCtrl;
	TitlePlayer* player;
	static const int SHADOW_NUM_MAX = 10;
	int shadowNum;
	float easingCount;
	float cameraAngle;
	VECTOR3 cameraRotation;
	float cameraLeap;
	bool attack;
	int num;

	BlurScreen* blurScreen;
	float startTimer;
	float volume;

	void Avoid(float speed);
};