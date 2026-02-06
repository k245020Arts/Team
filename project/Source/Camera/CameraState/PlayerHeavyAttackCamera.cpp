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
			VECTOR3 easedT = Easing::EaseOut(c->defalutDistance, VECTOR3(0, 0, -500), t);
			c->currentDistance = easedT;
			backCounter -= Time::DeltaTimeRate();
		}
		else {
			c->currentDistance = VECTOR3(0, 0, -500);
		}
	}
	else {
		if (zoomTimer >= 0.0f) {

			float t = 1.0f - zoomTimer / 0.2f;
			VECTOR3 easedT = Easing::EaseOut(c->currentDistance, c->defalutDistance, t);
			c->currentDistance = easedT;
			zoomTimer -= Time::DeltaTimeRate();
		}
		else {
			c->currentDistance = c->defalutDistance;
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
}

void PlayerHeavyAttackCamera::Finish()
{
}
