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

	VECTOR3 dir = VZero;
	dir.y *= 0.0f;
	//b->bossTransform->GetRotationMatrix();
	dir = b->bossTransform->Forward() * -1000.0f;
	b->enemyBaseComponent.physics->AddVelocity(dir, false);
	VECTOR3 moveVelo;
	moveVelo = b->enemyBaseComponent.physics->GetVelocity() * VECTOR3(1.0f, 0.0f, 1.0f);

	float max = 1500.0f;
	//最大速度までいったらスピードマックスに補正
	if (moveVelo.SquareSize() >= max * max) {
		moveVelo = moveVelo.Normalize() * max;
		moveVelo.y = b->enemyBaseComponent.physics->GetVelocity().y;
		b->enemyBaseComponent.physics->SetVelocity(moveVelo);
	}

	VECTOR3 targetVec = b->bossTransform->position - b->enemyBaseComponent.playerObj->GetTransform()->position;
	//プレイヤーと離れたらアイドルになる
	if (targetVec.Size() >= bs->GetStatus().chaseRange)
		b->enemyBaseComponent.state->ChangeState(StateID::BOSS_IDOL_S);
	//プレイヤーに近づいたら攻撃に移行
	if (targetVec.Size() <= bs->GetStatus().range)
		b->enemyBaseComponent.state->ChangeState(StateID::BOSS_COOL_TIME_S);
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
