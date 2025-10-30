#include "BossRun.h"
#include "Boss.h"
#include "stateManager.h"
#include "AttackSorting.h"
#include "BossStatus.h"

BossRun::BossRun()
{
	id = ID::B_RUN;
	animId = ID::B_RUN;
	string = Function::GetClassNameC<BossRun>();
	bs = new BossStatus;
	speed = bs->GetStatus().runSpeed;
}

BossRun::~BossRun()
{
}

void BossRun::Update()
{
	Boss* b = GetBase<Boss>();
	b->LookPlayer();
	const float ROTY = -rotation.y - 0.5f * DX_PI_F;

	rotation = b->obj->GetTransform()->rotation;
	//移動の計算
	velocity.x = speed * cosf(ROTY);
	velocity.z = speed * sinf(ROTY);

	//計算したものをボスのポジションに足す
	b->obj->GetTransform()->position += velocity;

	VECTOR3 targetVec = b->bossTransform->position - b->enemyBaseComponent.playerObj->GetTransform()->position;
	//プレイヤーと離れたらアイドルになる
	if (targetVec.Size() >= ChaseRange)
		b->enemyBaseComponent.state->ChangeState(ID::B_IDOL);
	//プレイヤーに近づいたら攻撃に移行
	if (targetVec.Size() <= 1000.0f)
		b->enemyBaseComponent.state->ChangeState(ID::B_COOLTIME);
}

void BossRun::Draw()
{
}

void BossRun::Start()
{
	EnemyStateBase::Start();
}

void BossRun::Finish()
{
}
