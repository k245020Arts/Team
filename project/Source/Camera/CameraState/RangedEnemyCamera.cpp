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
	pPos = player->GetPlayerObj()->GetTransform()->position;

	camera->target = (pPos + enemyPos) * 0.5f;

	VECTOR3 dir = enemyPos - pPos;

	// ƒJƒƒ‰ˆÊ’u
	VECTOR3 camPosition = camera->target - dir * camPos.z + VECTOR3(0, camPos.y, 0);
	camera->cameraComponent.cameraTransform->position = camPosition;
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
	pPos = player->GetPlayerObj()->GetTransform()->position;
}

void RangedEnemyCamera::Finish()
{
}
