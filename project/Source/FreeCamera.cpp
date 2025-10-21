#include "FreeCamera.h"
#include "camera.h"
#include "controllerInputManager.h"
#include "Easing.h"

FreeCamera::FreeCamera()
{
	string = Function::GetClassNameC<FreeCamera>();
	id = ID::C_FREE;
}

FreeCamera::~FreeCamera()
{
}

void FreeCamera::Update()
{
	Camera* c = GetBase<Camera>();

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
	c->Follow();

	if (c->cameraComponent.control->GetStickInput().rightStick.x >= 0.3f) {
		c->cameraComponent.cameraTransform->rotation.y += 2.0f * DegToRad;
	}
	if (c->cameraComponent.control->GetStickInput().rightStick.x <= -0.3f) {
		c->cameraComponent.cameraTransform->rotation.y -= 2.0f * DegToRad;
	}
	if (c->cameraComponent.control->GetStickInput().rightStick.y >= 0.3f) {
		if (c->cameraComponent.cameraTransform->rotation.x >= -80.0f * DegToRad) {
			c->cameraComponent.cameraTransform->rotation.x -= 2.0f * DegToRad;
		}

	}
	if (c->cameraComponent.control->GetStickInput().rightStick.y <= -0.3f) {
		if (c->cameraComponent.cameraTransform->rotation.x <= 80.0f * DegToRad) {
			c->cameraComponent.cameraTransform->rotation.x += 2.0f * DegToRad;
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
