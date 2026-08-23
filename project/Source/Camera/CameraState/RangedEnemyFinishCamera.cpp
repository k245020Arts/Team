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
	/*
	あたった敵のポジションをもらう
	あたった敵にいい感じに拡大
	エフェクトを足す
	音を追加する
	*/
	Camera* camera = GetBase<Camera>();

	//camera->cameraComponent.cameraTransform->position = targetPos;
	//camera->target = groupManager->HitEnemyPosition();

	if (timer >= 0.0f) {

		const float t = 1.0f - timer / MAX_TIMER;

		const VECTOR3 enemyPos = groupManager->HitEnemyPosition();
		VECTOR3 easedT = Easing::EaseOut(keepTarget,enemyPos, t);
		camera->target = easedT;
		VECTOR3 pos = Easing::EaseOut(keepPos, targetPos, t);
		camera->cameraComponent.cameraTransform->position = pos;
		timer -= Time::DeltaTimeRate();

	}
	else {
		camera->target = groupManager->HitEnemyPosition();
		camera->cameraComponent.cameraTransform->position = targetPos;
	}
}

void RangedEnemyFinishCamera::Start()
{
	Camera* camera = GetBase<Camera>();
	player = camera->cameraComponent.player.obj->Component()->GetComponent<Player>();
	groupManager = FindGameObject<TrashEnemyGroup>();

	keepPos = camera->cameraComponent.cameraTransform->position;
	targetPos = player->GetPlayerObj()->GetTransform()->position + PosOffset;
	camera->target = (player->GetPlayerObj()->GetTransform()->position + groupManager->HitEnemyPosition()) * 0.5f;
	
	lookPos = groupManager->HitEnemyPosition();

	keepTarget = camera->target;
	timer = MAX_TIMER;
}

void RangedEnemyFinishCamera::Finish()
{
}
