#pragma once
#include "../Component/Component.h"
#include "../Component/Shaker/Shaker.h"
#include "cameraInformation.h"
#include "../Common/ID/StateID.h"

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

	void TargetSet(BaseObject* _obj);

	void TargetEnemySet();

	void CameraLeapSet(float _rape);
	void ChangeStateCamera(StateID::State_ID _id);

	void CameraRotationSet();
	void Follow();
	VECTOR3 GetTarget() { return target; }

	void CollsionPosHit(VECTOR3 norm, float size, VECTOR3 groundPos);

	void PushCamera(VECTOR3 norm, float size,VECTOR3 groundPos);

	void AttackEnemyFovChange(Transform* _targetTransform);

	bool IsFovIn(const Transform& _targetTransform,float maxFov);

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
	float beforePos;
	float nearFog;
	float farFog;
	float counter;
	bool hit;
	float rokPos;
	float moveAmout;
	VECTOR3 hitNormal;
	float  hitDist;
	VECTOR3 hitPos;
	EnemyAttackChangeCameraDirection direction;
	Transform* targetEnemyTransform;
};
