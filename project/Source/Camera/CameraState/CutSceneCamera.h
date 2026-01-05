#pragma once
#include "CameraStateBase.h"

class CutSceneCamera : public CameraStateBase
{
public:
	CutSceneCamera();
	~CutSceneCamera();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

	VECTOR3 PlayerEnemyWorldToPos(std::string _name);

private:
	float time;
	int cutSceneIndex;
	VECTOR3 firstPos;
};