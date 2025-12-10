#include "BossRun.h"
#include "../Boss.h"
#include "../../../State/StateManager.h"
#include "AttackSorting.h"
#include "BossStatus.h"
#include "../../../Component/Physics/Physics.h"

BossRun::BossRun()
{
	//id = ID::B_RUN;
	animId = ID::B_RUN;
	string = Function::GetClassNameC<BossRun>();
	bs = nullptr;
}

BossRun::~BossRun()
{
}

void BossRun::Update()
{
	Boss* b = GetBase<Boss>();
	b->LookPlayer();

	if (bs == nullptr)
	{
		bs = b->bs;
		speed = bs->GetStatus().runSpeed;
	}

	//rotation = b->obj->GetTransform()->rotation;
	//const float ROTY = -rotation.y - 0.5f * DX_PI_F;

	////移動の計算
	//velocity.x = speed * cosf(ROTY);
	//velocity.z = speed * sinf(ROTY);

	////計算したものをボスのポジションに足す
	//b->obj->GetTransform()->position += velocity;
	float speed = 0.0f;
	float max = 0.0f;
	if (b->hpRate == Boss::FIVE || b->hpRate == Boss::THREE) {
		speed = 2000.0f;
		max = 2500.0f;
	}
	else {
		speed = 1000.0f;
		max = 1500.0f;
	}
	b->MoveBoss(speed, max);

	VECTOR3 targetVec = b->bossTransform->position - b->enemyBaseComponent.playerObj->GetTransform()->position;
	//プレイヤーと離れたらアイドルになる
	if (targetVec.Size() >= bs->GetStatus().chaseRange)
		b->enemyBaseComponent.state->ChangeState(StateID::BOSS_IDOL_S);
	//プレイヤーに近づいたら攻撃に移行
	if (b->RunChangeAttack()) {
		b->enemyBaseComponent.state->ChangeState(StateID::BOSS_COOL_TIME_S);
	}
}

void BossRun::Draw()
{
}

void BossRun::Start()
{
	Boss* b = GetBase<Boss>();
	if (b->hpRate == Boss::FIVE || b->hpRate == Boss::THREE) {
		animId = ID::B_DUSH;
	}
	else {
		animId = ID::B_RUN;
	}
	EnemyStateBase::Start();
}

void BossRun::Finish()
{
}
