#include "FreeCamera.h"
#include "../camera.h"
#include "../../Common/InputManager/ControllerInputManager.h"
#include "../../Common/Easing.h"
#include <algorithm>

FreeCamera::FreeCamera()
{
	string			= Function::GetClassNameC<FreeCamera>();
	//id = ID::C_FREE;
	backCounter		= 0.0f;
	beforeTarget	= 0.0f;
}

FreeCamera::~FreeCamera()
{
}

void FreeCamera::Update()
{
	Camera* c = GetBase<Camera>();

	//滑らかに他のstateから移行した時に移動するようにしている。
	if (backCounter >= 0.0f) {
		float t = 1.0f - backCounter / TIMER_MAX;
		VECTOR3 easedT		= Easing::EaseOut(c->currentDistance, c->defalutDistance, t);
		VECTOR3 targetp = c->cameraComponent.player.transform->position + VECTOR3(0, 400, 0);
		c->target			= Easing::EaseOut(currentTarget, targetp, t);
		c->currentDistance	= easedT;
		backCounter			-= Time::DeltaTimeRate();
	}
	else {
		c->currentDistance	= c->defalutDistance;
		VECTOR3 targetp		= c->cameraComponent.player.transform->position + VECTOR3(0, 400, 0);
		
		/*if (fabs(beforeTarget - c->cameraComponent.player.transform->position.y) <= 1.0f) {
			targetp.y = beforeTarget;
		}*/
		c->target			= targetp ;
		//beforeTarget = targetp.y;
	}
	if (c->direction == EnemyAttackChangeCameraDirection::NONE) {
		StickMove();
	}
	else {
		EnemyChangeDir();
	}
	

	//プレイヤーの追従処理
	c->Follow();
	/*if (c->hit) {
		return;
	}*/
	
}

void FreeCamera::Draw()
{
}

void FreeCamera::Start()
{
	Camera* c = GetBase<Camera>();

	backCounter		= TIMER_MAX;
	currentTarget	= c->target;
	beforeTarget	= c->cameraComponent.player.transform->position.y;
}

void FreeCamera::Finish()
{
}

void FreeCamera::EnemyChangeDir()
{
	
	Camera* c = GetBase<Camera>();
	
	if (c->CameraRotationMove()) {
		c->direction = EnemyAttackChangeCameraDirection::NONE;
		c->moveTimer = 0.0f;
	}

	
	float rate = 1 - (c->moveTimer / c->MOVE_TIMER_MAX);

	VECTOR3 toEnemy = c->targetEnemyTransform->position -c->cameraComponent.cameraTransform->position;

	float targetYaw = atan2f(toEnemy.x, toEnemy.z);

	float diff = AnglePI(c->cameraComponent.cameraTransform->rotation.y, targetYaw);

	float speed = diff;

	float rotationData = speed * rate;

	float maxStep = c->angleMaxSpeed * DegToRad * Time::DeltaTimeRate();
	rotationData = std::clamp(rotationData, -maxStep, maxStep);

	c->cameraComponent.cameraTransform->rotation.y += rotationData;

	c->moveTimer -= Time::DeltaTimeRate();

	if (c->moveTimer <= 0.0f) {
		c->direction = EnemyAttackChangeCameraDirection::NONE;
	}
}

void FreeCamera::StickMove()
{
	Camera* c = GetBase<Camera>();
	//スティック移動
	if (c->cameraComponent.control->GetStickInput().rightStick.x >= 0.3f || CheckHitKey(KEY_INPUT_RIGHT)) {
		c->cameraComponent.cameraTransform->rotation.y += 120.0f * Time::DeltaTimeRate() * DegToRad;
	}
	if (c->cameraComponent.control->GetStickInput().rightStick.x <= -0.3f || CheckHitKey(KEY_INPUT_LEFT)) {
		c->cameraComponent.cameraTransform->rotation.y -= 120.0f * Time::DeltaTimeRate() * DegToRad;
	}
	if (c->cameraComponent.control->GetStickInput().rightStick.y >= 0.3f || CheckHitKey(KEY_INPUT_UP)) {
		if (c->cameraComponent.cameraTransform->rotation.x >= -50.0f * DegToRad) {
			c->cameraComponent.cameraTransform->rotation.x -= 120.0f * Time::DeltaTimeRate() * DegToRad;
		}

	}
	if (c->cameraComponent.control->GetStickInput().rightStick.y <= -0.3f || CheckHitKey(KEY_INPUT_DOWN)) {
		if (c->cameraComponent.cameraTransform->rotation.x <= 70.0f * DegToRad) {
			c->cameraComponent.cameraTransform->rotation.x += 120.0f * Time::DeltaTimeRate() * DegToRad;
		}
	}
}

float FreeCamera::AnglePI(float _current, float _target)
{
	float diff = _target - _current;

	while (diff > 180.0f * DegToRad) diff -= 2.0f * 180.0f * DegToRad;
	while (diff < -180.0f * DegToRad) diff += 2.0f * 180.0f * DegToRad;

	return diff;
}
