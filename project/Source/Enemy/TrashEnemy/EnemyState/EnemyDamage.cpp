#include "enemyDamage.h"
#include "../../../Component/Color/color.h"
#include "EnemyStateManager.h"
#include "../Enemy.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../Component/Shaker/Shaker.h"
#include "../../../Camera/Camera.h"
#include "../../../Common/Sound/SoundManager.h"

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
	timer -= e->enemyBaseComponent.enemy->GetEnemyObj()->GetObjectTimeRate();
	if (timer <= 0.00f) {
		e->enemyBaseComponent.state->ChangeState(ID::E_ANIM_IDOL);
	}
}

void EnemyDamage::Draw()
{
}

void EnemyDamage::Start()
{
	Enemy* e = GetBase<Enemy>();
	EnemyStateBase::Start();
	
	first = true;
}

void EnemyDamage::Finish()
{
	Enemy* e = GetBase<Enemy>();
	e->enemyBaseComponent.anim->Play(ID::E_ANIM_IDOL, 0.5f);
	e->enemyBaseComponent.shaker->ShakeFinish();
}

void EnemyDamage::DamageInit(EnemyDamageInfo _info)
{
	Enemy* e = GetBase<Enemy>();
	info = _info;
	timer = info.damageTime;
	//e->eCom.physics->SetVelocity(info.speed * MGetRotY(e->eCom.playerObj->GetTransform()->rotation.y));
	e->enemyBaseComponent.shaker->ShakeStart(info.shakePower, Shaker::HORIZONAL_SHAKE, true, info.shakeTime);
	first = false;
}