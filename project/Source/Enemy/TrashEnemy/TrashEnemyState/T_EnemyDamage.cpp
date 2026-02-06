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
	isGetInformation = false;
	rightVec = VZero;
	side = 0;
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
	float h = 0.5f;		 //最高点までの時間
	float k = 800.0f;    //吹き飛びの高さ
	KnockbackMove(e, 20.0f, a, h, k);

	if (e->enemyBaseComponent.anim->IsFinish() && e->GetEnemyObj()->GetTransform()->position.y <= 0)
		e->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_STANDBY);
}

void T_EnemyDamage::Draw()
{

}

void T_EnemyDamage::Start()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	EnemyStateBase::Start();
	motionSpeed = e->enemyBaseComponent.anim->GetPlaySpeed();

	isGetInformation = false;
	counter = 0;
}

void T_EnemyDamage::Finish()
{

}

void T_EnemyDamage::KnockbackMove(TrashEnemy* _e, float _speed,float a, float h, float k)
{
	counter += Time::DeltaTimeRate();
	
	float offsetY = Orbit(a, h, k);

	if (offsetY <= 0)
		offsetY = 0;
	if (!isGetInformation)
	{
		//正面べく
		VECTOR3 frontVec = VECTOR3(0, 0, 1) * MGetRotY(_e->enemyBaseComponent.playerObj->GetTransform()->rotation.y);
		//プレイヤーとのベクトル
		VECTOR3 vec = _e->enemyBaseComponent.playerObj->GetTransform()->position - _e->GetEnemyObj()->GetTransform()->position;
		// 右ベクトル
		rightVec = VECTOR3(frontVec.z, 0.0f, -frontVec.x);
		// 左右判定
		side = VDot(rightVec, vec);

		isGetInformation = true;
	}
	
	//段々減速させるため
	float speed = _speed;
	if (speed >= 0)
		speed -= Time::DeltaTimeRate() * 10.0f;
	else
		speed = 0;
	//0以上なら右側、0以上なら左側（三項演算子）
	float dir = (side >= 0) ? 1.0f : -1.0f;
	//ポジションに適応する
	_e->GetEnemyObj()->GetTransform()->position -= rightVec * dir * speed;;

	_e->GetEnemyObj()->GetTransform()->position.y = offsetY;
}

float T_EnemyDamage::Orbit(float a, float h, float k)
{
	return a * (counter - h) * (counter - h) + k;
}