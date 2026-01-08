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

	Transform* PlayerEnemyWorldToPos(std::string _name);

	void StateImguiDraw()override;

private:
	float time;
	int cutSceneIndex;
	VECTOR3 firstPos;
	bool first;
	VECTOR3 keepPos;
	VECTOR3 keepTarget;
};