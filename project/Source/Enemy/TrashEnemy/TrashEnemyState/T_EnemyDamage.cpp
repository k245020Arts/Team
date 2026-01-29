#include "T_EnemyDamage.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "../../../Player/Player.h"

T_EnemyDamage::T_EnemyDamage()
{
	animId = ID::E_DIE;
	string = Function::GetClassNameC<T_EnemyDamage>();

	motionSpeed = 0;

	counter = 0;
}

T_EnemyDamage::~T_EnemyDamage()
{
}

void T_EnemyDamage::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();

	if (e->IsPlayerSpecialMove())
	{
		e->enemyBaseComponent.anim->SetPlaySpeed(0);
		return;
	}
	
	e->enemyBaseComponent.anim->SetPlaySpeed(motionSpeed);

	float a = -2000.0f;  //落下の強さ（重力）
	float h = 1.0f;		 //最高点までの時間
	float k = 700.0f;    //吹き飛びの高さ
	KnockbackMove(e, a, h, k);
}

void T_EnemyDamage::Draw()
{

}

void T_EnemyDamage::Start()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	EnemyStateBase::Start();
	motionSpeed = e->enemyBaseComponent.anim->GetPlaySpeed();

	counter = 0;
}

void T_EnemyDamage::Finish()
{

}

void T_EnemyDamage::KnockbackMove(TrashEnemy* _e, float a, float h, float k)
{
	counter += Time::DeltaTimeRate();
	
	float offsetY = Orbit(a, h, k);

	if (offsetY <= 0)
	{
		offsetY = 0;
		if (_e->enemyBaseComponent.anim->IsFinish())
			_e->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_STANDBY);
	}
	//正面べく
	VECTOR3 frontVec = VECTOR3(0, 0, 1) * MGetRotY(_e->enemyBaseComponent.playerObj->GetTransform()->rotation.y);
	//プレイヤーとのベクトル
	VECTOR3 vec = _e->enemyBaseComponent.playerObj->GetTransform()->position - _e->GetEnemyObj()->GetTransform()->position;
	// 右ベクトル
	VECTOR3 rightVec = VECTOR3(frontVec.z, 0.0f, -frontVec.x);
	// 左右判定
	float side = VDot(rightVec, vec);
	//内積
	float dotProduct = VDot(frontVec, vec);
	//ポジションに足す
	_e->GetEnemyObj()->GetTransform()->position -= rightVec * side * 0.02f;

	_e->GetEnemyObj()->GetTransform()->position.y = offsetY;
}

float T_EnemyDamage::Orbit(float a, float h, float k)
{
	return a * (counter - h) * (counter - h) + k;
}