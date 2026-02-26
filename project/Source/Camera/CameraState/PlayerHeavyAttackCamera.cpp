#include "PlayerHeavyAttackCamera.h"
#include "../../Common/Easing.h"
#include "../../Component/Transform/Transform.h"
#include "../CameraInformation.h"
#include "./../../Component/Shaker/Shaker.h"
#include "../../Component/ComponentManager.h"
#include "../../Player/Player.h"
#include "../../Player/PlayerState/PlayerStateManager.h"
#include "../../Component/Object/Object3D.h"
#include "../Camera.h"
#include "../../Common/InputManager/InputManager.h"
#include "../../Enemy/EnemyManager.h"
#include <algorithm>

PlayerHeavyAttackCamera::PlayerHeavyAttackCamera()
{
	player = nullptr;
	backCounter = 0.0f;

	targetPos = VZero;
	currentTarget = VZero;
	beforeTarget = 0.0f;
}

PlayerHeavyAttackCamera::~PlayerHeavyAttackCamera()
{
}

void PlayerHeavyAttackCamera::Update()
{
	Camera* c = GetBase<Camera>();
	if (player->GetCharge()) {
		//‘¼‚Ìˆ—‚©‚ç‹A‚Á‚Ä‚«‚½‚Æ‚«‚É‚·‚®‚ÉƒJƒƒ‰‚ÌˆÊ’u‚ª–ß‚ç‚È‚¢‚æ‚¤‚É•âŠ®‚ðŠ|‚¯‚Ä‚¢‚é
		if (backCounter >= 0.0f) {

			float t = 1.0f - backCounter / 2.0f;
			VECTOR3 easedT = Easing::Lerp(c->defalutDistance, VECTOR3(0, 0, -1200), t);
			c->currentDistance = easedT;
			backCounter -= Time::DeltaTimeRate();
			VECTOR3 targetp = c->cameraComponent.player.transform->position + VECTOR3(0, 400, 0);
			c->target = Easing::Lerp(currentTarget, targetp, t);
		}
		else {
			c->currentDistance = VECTOR3(0, 0, -800);
			VECTOR3 targetp = c->cameraComponent.player.transform->position + VECTOR3(0, 400, 0);
			c->target = targetp;
		}
	}
	else {
		if (zoomTimer >= 0.0f) {

			float t = 1.0f - zoomTimer / 0.2f;
			VECTOR3 easedT = Easing::Lerp(c->currentDistance, c->defalutDistance, t);
			c->currentDistance = easedT;
			zoomTimer -= Time::DeltaTimeRate();
			VECTOR3 targetp = c->cameraComponent.player.transform->position + VECTOR3(0, 400, 0);
			c->target = Easing::Lerp(currentTarget, targetp, t);
		}
		else {
			c->currentDistance = c->defalutDistance;
			VECTOR3 targetp = c->cameraComponent.player.transform->position + VECTOR3(0, 400, 0);
			c->target = targetp;
		}
		
	}
	
	//’Ç]ˆ—
	c->cameraComponent.camera->Follow();
	c->cameraComponent.camera->CameraRotationSet();
}

void PlayerHeavyAttackCamera::Draw()
{
}

void PlayerHeavyAttackCamera::Start()
{
	Camera* c = GetBase<Camera>();
	player = c->cameraComponent.player.obj->Component()->GetComponent<Player>();

	//backCounter = TIMER_MAX;
	currentTarget = c->target;
	backCounter = 2.0f;
	zoomTimer = 0.2f;
	//c->cameraComponent.cameraTransform->rotation.y = player->GetPlayerTransform()->rotation.y;
}

void PlayerHeavyAttackCamera::Finish()
{
}
