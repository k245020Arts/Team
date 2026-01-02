#include "T_EnemyDead.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "../../../Component/Collider/ModelCollider.h"
#include "../../../Component/Collider/SphereCollider.h"

T_EnemyDead::T_EnemyDead()
{
	animId = ID::E_DIE;
	string = Function::GetClassNameC<T_EnemyDead>();
	counter = 0;
}

T_EnemyDead::~T_EnemyDead()
{
}

void T_EnemyDead::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();

	counter += Time::DeltaTimeRate(); //フレーム時間
	
	float rotY = e->GetEnemyObj()->GetTransform()->rotation.y;
	e->GetEnemyObj()->GetTransform()->position.x -= 80 * counter * cosf(rotY - 0.5f * DX_PI_F);
	//e->GetEnemyObj()->GetTransform()->position.y += 5.0f;
	e->GetEnemyObj()->GetTransform()->position.z -= 80 * counter * sinf(rotY - 0.5f * DX_PI_F);

	float a = -1000.0f;  //落下の強さ（重力）
	float h = 0.8f;		 //最高点までの時間
	float k = 500.0f;    //吹き飛びの高さ

	float offsetY = a * (counter - h) * (counter - h) + k;

	if (offsetY <= 0)
		offsetY = 0;
	e->GetEnemyObj()->GetTransform()->position.y = offsetY;

	if (e->enemyBaseComponent.anim->IsFinish()&& offsetY <= 0)
		e->active = false;
}

void T_EnemyDead::Draw()
{
}

void T_EnemyDead::Start()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	EnemyStateBase::Start();
	e->enemyBaseComponent.anim->SetPlaySpeed(0.8f);
	obj->Component()->RemoveAllComponent<SphereCollider>();
	obj->Component()->RemoveAllComponent<ModelCollider>();
}

void T_EnemyDead::Finish()
{
}

float T_EnemyDead::Orbit(float x, float a, float h, float k)
{
	float dx = x - h;
	return a * dx * dx + k;
}
