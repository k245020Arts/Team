#include "RangedEnemyFinishCamera.h"
#include "../Camera.h"
#include "../../Player/player.h"
#include "../../Enemy/TrashEnemy/TrashEnemyGroup.h"
#include "../../Common/Easing/Easing.h"

RangedEnemyFinishCamera::RangedEnemyFinishCamera()
{
	string = Function::GetClassNameC<RangedEnemyFinishCamera>();

	player = nullptr;
	groupManager = nullptr;
	targetPos = VZero;
	lookPos = VZero;

	keepTarget = VZero;
	timer = 0;
	keepPos = VZero;
}

RangedEnemyFinishCamera::~RangedEnemyFinishCamera()
{
}

void RangedEnemyFinishCamera::Update()
{
	Camera* camera = GetBase<Camera>();

	if (timer >= 0.0f) 
	{
		const float t = 1.0f - timer / MaxTimer;

		const VECTOR3 enemyPos = groupManager->HitEnemyPosition();
		VECTOR3 easedT = Easing::EaseOut(keepTarget,enemyPos, t);
		camera->target = easedT;
		VECTOR3 pos = Easing::EaseOut(keepPos, targetPos, t);
		camera->cameraComponent.cameraTransform->position = pos;
		timer -= Time::DeltaTimeRate();
	}
	else
	{
		lookPos = groupManager->HitEnemyPosition() + EnemyPosOffset;
		camera->target = lookPos;
		//camera->cameraComponent.cameraTransform->position = targetPos;
	}
}

void RangedEnemyFinishCamera::Start()
{
	Camera* camera = GetBase<Camera>();
	player = camera->cameraComponent.player.obj->Component()->GetComponent<Player>();
	groupManager = FindGameObject<TrashEnemyGroup>();

	keepPos = camera->cameraComponent.cameraTransform->position;
	targetPos = player->GetPlayerObj()->GetTransform()->position + PosOffset * MGetRotY(player->GetPlayerObj()->GetTransform()->rotation.y);

	lookPos = groupManager->HitEnemyPosition() + EnemyPosOffset;
	camera->target = lookPos/*(player->GetPlayerObj()->GetTransform()->position + lookPos) * 0.5f*/;
	
	
	keepTarget = camera->target;
	timer = MaxTimer;
	camera->canUseSpecial = false;

}

void RangedEnemyFinishCamera::Finish()
{
	Camera* camera = GetBase<Camera>();
	camera->canUseSpecial = true;
}
