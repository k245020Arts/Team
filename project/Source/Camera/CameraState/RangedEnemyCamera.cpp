#include "RangedEnemyCamera.h"
#include "../Camera.h"
#include "../../Enemy/TrashEnemy/TrashEnemyManager.h"
#include "../../Player/player.h"

RangedEnemyCamera::RangedEnemyCamera()
{
	tEnemyManager = nullptr;
	player = nullptr;

	enemyPos = VZero;
	pPos = VZero;
}

RangedEnemyCamera::~RangedEnemyCamera()
{
}

void RangedEnemyCamera::Update()
{
	Camera* camera = GetBase<Camera>();

	enemyPos = tEnemyManager->GetRangedLeaderPos();
	pPos = player->GetPlayerObj()->GetTransform()->position + PlayerPosOffset;

	VECTOR3 dir = VNorm(enemyPos - pPos);
	dir.y = 0.0f;

	float dist = VSize(enemyPos - pPos);

	camera->target = (pPos + enemyPos) * 0.5f;

	VECTOR3 offset = dir * -1 * CamPos + VGet(0, 400, 0);

	camera->cameraComponent.cameraTransform->position = pPos + offset;
}

void RangedEnemyCamera::Draw()
{
}

void RangedEnemyCamera::Start()
{
	Camera* camera = GetBase<Camera>();

	tEnemyManager = FindGameObject<TrashEnemyManager>();
	player = camera->cameraComponent.player.obj->Component()->GetComponent<Player>();

	enemyPos = tEnemyManager->GetRangedLeaderPos();
	pPos = player->GetPlayerObj()->GetTransform()->position + PlayerPosOffset;
}

void RangedEnemyCamera::Finish()
{
}
