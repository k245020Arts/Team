#include "FreeCamera.h"
#include "../camera.h"
#include "../../Common/InputManager/ControllerInputManager.h"
#include "../../Common/Easing.h"

FreeCamera::FreeCamera()
{
	string = Function::GetClassNameC<FreeCamera>();
	//id = ID::C_FREE;
	backCounter = 0.0f;
	beforeTarget = 0.0f;
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
		VECTOR3 easedT = Easing::EaseOut(c->currentDistance, c->defalutDistance, t);
		c->target = Easing::EaseOut(currentTarget, (VECTOR3)(c->cameraComponent.player.transform->position + VECTOR3(0, 400, 0)), t);
		c->currentDistance = easedT;
		backCounter -= Time::DeltaTimeRate();
	}
	else {
		c->currentDistance = c->defalutDistance;
		VECTOR3 targetp = c->cameraComponent.player.transform->position;
		
		if (fabs(beforeTarget - c->cameraComponent.player.transform->position.y) <= 1.0f) {
			targetp.y = beforeTarget;
		}
		c->target = targetp+ VECTOR3(0,400,0);
		beforeTarget = targetp.y;
	}
	//プレイヤーの追従処理
	c->Follow();
	if (c->hit) {
		return;
	}
	//スティック移動
	if (c->cameraComponent.control->GetStickInput().rightStick.x >= 0.3f) {
		c->cameraComponent.cameraTransform->rotation.y += 180.0f * Time::DeltaTimeRate() * DegToRad;
	}
	if (c->cameraComponent.control->GetStickInput().rightStick.x <= -0.3f) {
		c->cameraComponent.cameraTransform->rotation.y -= 180.0f * Time::DeltaTimeRate() * DegToRad;
	}
	if (c->cameraComponent.control->GetStickInput().rightStick.y >= 0.3f) {
		if (c->cameraComponent.cameraTransform->rotation.x >= -50.0f * DegToRad) {
			c->cameraComponent.cameraTransform->rotation.x -= 180.0f * Time::DeltaTimeRate() * DegToRad;
		}

	}
	if (c->cameraComponent.control->GetStickInput().rightStick.y <= -0.3f) {
		if (c->cameraComponent.cameraTransform->rotation.x <= 60.0f * DegToRad) {
			c->cameraComponent.cameraTransform->rotation.x += 180.0f * Time::DeltaTimeRate() * DegToRad;
		}
	}
}

void FreeCamera::Draw()
{
}

void FreeCamera::Start()
{
	Camera* c = GetBase<Camera>();

	backCounter = TIMER_MAX;
	currentTarget = c->target;
	beforeTarget = c->cameraComponent.player.transform->position.y;
}

void FreeCamera::Finish()
{
}
