#pragma once
#include "../Component/Component.h"
#include "../Component/Shaker/Shaker.h"
#include "cameraInformation.h"
#include "../Common/ID/StateID.h"
#include "../Component/Transform/Quaternion.h"

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
	Quaternion cameraQuaternion;
};

inline MATRIX MyLookAt(const VECTOR& eye, const VECTOR& target, const VECTOR& up)
{
	MATRIX m;

	// ========= 1. 前方向 =========
	VECTOR forward = VNorm(VSub(target, eye));  // (target - eye)

	// ========= 2. 右方向 =========
	VECTOR right = VNorm(VCross(up, forward));

	// ========= 3. 実上方向（再計算） =========
	VECTOR newUp = VCross(forward, right);

	// ========= 4. 行列を詰める =========
	m.m[0][0] = right.x;
	m.m[0][1] = right.y;
	m.m[0][2] = right.z;
	m.m[0][3] = 0.0f;

	m.m[1][0] = newUp.x;
	m.m[1][1] = newUp.y;
	m.m[1][2] = newUp.z;
	m.m[1][3] = 0.0f;

	m.m[2][0] = forward.x;
	m.m[2][1] = forward.y;
	m.m[2][2] = forward.z;
	m.m[2][3] = 0.0f;

	m.m[3][0] = 0.0f;
	m.m[3][1] = 0.0f;
	m.m[3][2] = 0.0f;
	m.m[3][3] = 1.0f;

	return m;
}
