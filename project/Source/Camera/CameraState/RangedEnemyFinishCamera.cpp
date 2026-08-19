#include "RangedEnemyFinishCamera.h"
#include "../Camera.h"
#include "../../Player/player.h"
#include "../../Enemy/TrashEnemy/TrashEnemyGroup.h"

RangedEnemyFinishCamera::RangedEnemyFinishCamera()
{
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

	camera->cameraComponent.cameraTransform->position = targetPos;
	camera->target = groupManager->HitEnemyPosition();
}

void RangedEnemyFinishCamera::Start()
{
	Camera* camera = GetBase<Camera>();
	player = camera->cameraComponent.player.obj->Component()->GetComponent<Player>();
	groupManager = FindGameObject<TrashEnemyGroup>();

	targetPos = player->GetPlayerObj()->GetTransform()->position + PosOffset;
	camera->target = (player->GetPlayerObj()->GetTransform()->position + groupManager->HitEnemyPosition()) * 0.5f;
	
	lookPos = groupManager->HitEnemyPosition();
}

void RangedEnemyFinishCamera::Finish()
{
}
