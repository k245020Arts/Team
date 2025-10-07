#include "enemyDamage.h"
#include "color.h"
#include "EnemyStateManager.h"
#include "enemy.h"
#include "Physics.h"
#include "Animator.h"
#include "Shaker.h"
#include "camera.h"
#include "SoundManager.h"

EnemyDamage::EnemyDamage()
{
	string = typeid(*this).name();
	const std::string prefix = "class ";
	if (string.find(prefix) == 0) {
		string = string.substr(prefix.length());
	}
	animId = ID::E_DAMAGE;
	id = ID::E_DAMAGE;
	timer = 0.0f;
	info = EnemyDamageInfo();
	first = true;
	
}

EnemyDamage::~EnemyDamage()
{
}

void EnemyDamage::Update()
{
	Enemy* e = GetBase<Enemy>();
	if (first) {
		
	}
	//時間が戻ったらステート遷移
	timer -= e->eCom.enemy->GetEnemyObj()->GetObjectTimeRate();
	if (timer <= 0.00f) {
		e->eCom.state->ChangeState(ID::E_ANIM_IDOL);
	}
}

void EnemyDamage::Draw()
{
}

void EnemyDamage::Start()
{
	Enemy* e = GetBase<Enemy>();
	EnemyStateBase::Start();
	e->eCom.sound->RandamSe("E_DamageV", 2);
	first = true;
}

void EnemyDamage::Finish()
{
	Enemy* e = GetBase<Enemy>();
	e->eCom.anim->Play(ID::E_ANIM_IDOL, 0.5f);
	e->eCom.shaker->ShakeFinish();
}

void EnemyDamage::DamageInit(EnemyDamageInfo _info)
{
	Enemy* e = GetBase<Enemy>();
	info = _info;
	timer = info.damageTime;
	//e->eCom.physics->SetVelocity(info.speed * MGetRotY(e->eCom.playerObj->GetTransform()->rotation.y));
	e->eCom.shaker->ShakeStart(info.shakePower, Shaker::HORIZONAL_SHAKE, true, info.shakeTime);
	first = false;
}