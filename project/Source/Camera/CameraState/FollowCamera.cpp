#include "FollowCamera.h"
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

FollowCamera::FollowCamera()
{
	enemyShake = false;
	string = Function::GetClassNameC<FollowCamera>();
	//id = ID::C_FOLLOW;
	backCounter = 0.0f;
}

FollowCamera::~FollowCamera()
{

}

void FollowCamera::Update()
{
	Camera* c = GetBase<Camera>();
	//‘¼‚Ìˆ—‚©‚ç‹A‚Á‚Ä‚«‚½‚Æ‚«‚É‚·‚®‚ÉƒJƒƒ‰‚ÌˆÊ’u‚ª–ß‚ç‚È‚¢‚æ‚¤‚É•âŠ®‚ðŠ|‚¯‚Ä‚¢‚é
	if (backCounter >= 0.0f) {
		float t = 1.0f - backCounter / TIMER_MAX;
		VECTOR3 easedT = Easing::EaseOut(c->currentDistance, c->defalutDistance, t);
		c->target = Easing::EaseOut(currentTarget,c->cameraComponent.target.transform->position,t);
		c->currentDistance = easedT;
		backCounter -= Time::DeltaTimeRate();
	}
	else {
		c->currentDistance = c->defalutDistance;
		c->target = c->cameraComponent.target.transform->position;
	}
	//’Ç]ˆ—
	c->cameraComponent.camera->Follow();
	//c->TargetEnemySet();
	/*if (p->GetPlayerStateManager()->GetState<PlayerStateBase>()->GetString() == "PlayerJustAvoid") {
		*c->cameraComponent.target = c->cameraComponent.enemy.transform->position + c->cameraComponent.player.transform->position * 0.5f;
	}
	else {*/

	//ƒJƒƒ‰‚Ì‰ñ“]
	c->cameraComponent.camera->CameraRotationSet();
	if (c->cameraComponent.control->GetStickKnockingPut(0.5f).rightStick == S_LEFT || c->cameraComponent.control->GetStickKnockingPut(0.5f).rightStick == S_RIGHT) {
		c->cameraComponent.enemyManager->PlayerDistance(c->cameraComponent.camera);
		Start();
	}
}

void FollowCamera::Draw()
{

}

void FollowCamera::Start()
{
	Camera* c = GetBase<Camera>();
	p = c->cameraComponent.player.obj->Component()->GetComponent<Player>();

	backCounter = TIMER_MAX;
	currentTarget = c->target;
	c->cameraComponent.cameraTransform->rotation.x = 30.0f * DegToRad;
}

void FollowCamera::Finish()
{
	
}
