#pragma once
#include "../Component/Component.h"
#include "../Component/Shaker/Shaker.h"
#include "cameraInformation.h"
#include "../Common/ID/StateID.h"
#include "../Component/Transform/Quaternion.h"
#include "../Common/JsonReader.h"

static const std::string PLAYER_POS_NAME		= "Player";
static const std::string PLAYER_FIRST_POS_NAME	= "First_Player";
static const std::string ENEMY_POS_NAME			= "Enemy";
static const std::string ENEMY_FIRST_POS_NAME	= "First_Enemy";
static const std::string WORLD_POS_NAME			= "World";

namespace CutSceneSpece {

	enum class EaseType {
		Linear,
		In,
		Out,
		InOut
	};

	struct CutSceneCameraPos {
		VECTOR3 startPos;
		VECTOR3 endPos;
		VECTOR3 target;
	};

	struct CutScene {
		float duration;
		EaseType ease;
		CutSceneCameraPos camera;
		std::string followPosName;
		std::string followPosTarget;
		std::string firstPosBaseName;
	};

	// CutSceneCamera Å® JSON
	inline void to_json(nlohmann::json& j, const CutSceneCameraPos& c) {
		j = {
			{"startPos", c.startPos},
			{"endPos",   c.endPos},
			{"target",   c.target}
		};
	}

	inline void from_json(const nlohmann::json& j, CutSceneCameraPos& c) {
		j.at("startPos").get_to(c.startPos);
		j.at("endPos").get_to(c.endPos);
		j.at("target").get_to(c.target);
	}

	// EaseType Çï∂éöóÒÇ…ïœä∑
	inline std::string EaseTypeToString(EaseType e) {
		switch (e) {
		case EaseType::Linear: return "Linear";
		case EaseType::In:     return "In";
		case EaseType::Out:    return "Out";
		case EaseType::InOut:  return "InOut";
		}
		return "Linear";
	}

	inline EaseType StringToEaseType(const std::string& s) {
		if (s == "Linear") return EaseType::Linear;
		if (s == "In")     return EaseType::In;
		if (s == "Out")    return EaseType::Out;
		if (s == "InOut")  return EaseType::InOut;
		return EaseType::Linear;
	}

	// CutScene Å® JSON
	inline void to_json(nlohmann::json& j, const CutScene& c) {
		j = {
			{"duration", c.duration},
			{"ease", EaseTypeToString(c.ease)},
			{"camera", c.camera},
			{"followPos", c.followPosName},
			{"followTarget", c.followPosTarget},
			{"firstPosBaseName", c.firstPosBaseName},
		};
	}

	inline void from_json(const nlohmann::json& j, CutScene& c) {
		j.at("duration").get_to(c.duration);
		c.ease = StringToEaseType(j.at("ease").get<std::string>());
		j.at("camera").get_to(c.camera);
		j.at("followPos").get_to(c.followPosName);
		j.at("followTarget").get_to(c.followPosTarget);
		//j.at("firstPosBaseName").get_to(c.firstPosBaseName);
	}
}


class ControllerInputManager;
class InputManager;
class CameraEditorGui;

class Camera : public Component
{
public:
	friend class FollowCamera;
	friend class JustAvoidCamera;
	friend class JustAvoidAttackCamera;
	friend class JustAvoidAttackHitCamera;
	friend class CameraStateBase;
	friend class FreeCamera;
	friend class CameraEditorGui;
	friend class PlayerSpecialAttackCamera;
	friend class CutSceneCamera;
	
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

	void AttackEnemyFovChange(Transform* _targetTransform,float _maxspeed);

	bool IsFovIn(const Transform& _targetTransform,float maxFov);

	void RotationChange(const Transform& _targetTransform, float _speed);

	bool CameraRotationMove();

	void CameraEditor();

	void CutSceneChangeState(std::string _name);

	bool IsCutScene() { return isCutScene; }

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
	
	float moveTimer;
	const float MOVE_TIMER_MAX = 0.4f;
	float angleMaxSpeed;

	CameraEditorGui* editor;
	std::vector<CutSceneSpece::CutScene> cutSceneData;
	bool isCutScene;
};