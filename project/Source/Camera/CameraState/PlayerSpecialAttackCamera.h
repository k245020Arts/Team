#pragma once
#include "CameraStateBase.h"

class PlayerSpecialAttackCamera : public CameraStateBase
{
public:
	PlayerSpecialAttackCamera();
	~PlayerSpecialAttackCamera();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:

};
