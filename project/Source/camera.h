#pragma once
#include "Component.h"
#include "Shaker.h"
#include "cameraInformation.h"
#include "ID.h"

class ControllerInputManager;
class InputManager;

class Camera : public Component
{
public:
	friend class FollowCamera;
	friend class JustAvoidCamera;
	friend class JustAvoidAttackCamera;
	friend class JustAvoidAttackHitCamera;
	friend class CameraStateBase;
	friend class FreeCamera;
	
	Camera();
	~Camera();

	void Update()override;
	void Draw()override;

	void Start(BaseObject* _eObj);
	void ImguiDraw()override;
	void PlayerSet(BaseObject* _obj);

	Transform* GetCameraTransform() { return cameraComponent.cameraTransform; }
	void CameraShake(VECTOR3 _power, Shaker::ShakePattern _pattern, bool _stop, float _second);
	void CameraShakeStop();

	void TargetEnemySet();

	void CameraLeapSet(float _rape);
	void ChangeStateCamera(ID::IDType _id);

	void CameraRotationSet();
	void Follow();

private:

	float timeTest;
	bool normalCamera;
	int debugButton;
	int targetChangeButton;
	VECTOR3 target;
	float reap;
	CameraInformation::CameraComponent cameraComponent;
	VECTOR3 currentDistance;
	VECTOR3 defalutDistance;
	float fov;
	ControllerInputManager* control;
	InputManager* input;
	bool rockOn;
};
