#include "RangedEnemyFinishCamera.h"
#include "../Camera.h"
#include "../../Player/player.h"

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
}

void RangedEnemyFinishCamera::Start()
{
	Camera* camera = GetBase<Camera>();
	player = camera->cameraComponent.player.obj->Component()->GetComponent<Player>();

	targetPos = player->GetPlayerObj()->GetTransform()->position + PosOffset;
	camera->target = player->GetPlayerObj()->GetTransform()->position;
}

void RangedEnemyFinishCamera::Finish()
{
}
