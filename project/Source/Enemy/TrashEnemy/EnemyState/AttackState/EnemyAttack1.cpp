#include "enemyAttack1.h"
#include "../../../../Component/Animator/Animator.h"
#include "../EnemyStateManager.h"
#include "../../Enemy.h"
#include "../../../../Common/Effect/EffectManager.h"
#include "../../../../Common/LoadManager.h"
#include "../../../../Common/Sound/SoundManager.h"
#include "../../../../Weapon/WeaponManager.h"
#include "../../../../Camera/Camera.h"
#include "../../TrashEnemy.h"

EnemyAttack1::EnemyAttack1()
{
	animId = ID::B_N_ATTACK3; /*ID::E_ATTACK1*/;
	collTrans = Transform(VECTOR3(-100, 50, 100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	string = Function::GetClassNameC<EnemyAttack1>();
	//id = ID::TE_ATTACK;
	sound = false;
	animStopCounter = 0.0f;
	effectAverageSpeed = 0.0f;
	hitDamage = 5.0f;
}

EnemyAttack1::~EnemyAttack1()
{
}

void EnemyAttack1::Update()
{
	Enemy* e = GetBase<Enemy>();
	if (e->enemyBaseComponent.anim->IsFinish())
	{
		//e->enemyBaseComponent.state->ChangeState(ID::TE_IDOL);
	}
	/*if (com.anim->GetCurrentFrame() >= 12.0f && com.anim->GetCurrentFrame() <= 20.0f) {
		com.anim->SetPlaySpeed(0.2f);
	} 
	else {
		com.anim->SetPlaySpeed(1.0f);
	}*/
	AttackSound();
	AttackFlash(ID::IDType::E_MODEL,12, "E_AttackV");
	Trail();
	
	
	
	AttackCollsion();
	float attackStartTime = e->enemyBaseComponent.anim->EventStartTime(animId);
	/*if (com.anim->GetCurrentFrame() > attackStartTime) {
		return;
	}*/
	float sub = e->enemyBaseComponent.anim->GetCurrentFrame() - e->enemyBaseComponent.anim->GetCurrentBeforeFrame();
	float speed = sub * effectAverageSpeed;
	//float rate = 1.0f - com.anim->GetCurrentFrame() / attackStartTime;
	e->enemyBaseComponent.effect->SetSpeedEffekseer(Effect_ID::ENEMY_ATTACK_CICLE, speed);
}

void EnemyAttack1::Draw()
{
	/*ECTOR3 pos =  com.enemy->GetEnemyTransform()->position;
	pos.y = 500.0f;
	float attackStartTime = com.anim->EventStartTime(animId);
	if (com.anim->GetCurrentFrame() > attackStartTime) {
		return;
	}
	float rate = 1.0f - com.anim->GetCurrentFrame() / attackStartTime;
	VECTOR3 screenPos = ConvWorldPosToScreenPos(pos);
	DrawCircle(screenPos.x, screenPos.y, rate * 50.0f, 0xffff00, false);*/
	//DrawCircle(screenPos.x, screenPos.y, 50, 0xffff00, false);
}

void EnemyAttack1::Start()
{
	/*TrashEnemy* e = GetBase<TrashEnemy>();
	EnemyStateBase::Start();
	firstColl = true;
	sound = true;
	effectAverageSpeed = 60.0f / e->enemyBaseComponent.anim->EventStartTime(animId);
	e->enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(0, 100, 0), VZero, VOne), e->enemyBaseComponent.trashEnemy->GetEnemyObj(), Effect_ID::ENEMY_ATTACK_CICLE, 1.0f);*/

	/*TrashEnemy* e = GetBase<TrashEnemy>();
	EnemyStateBase::Start();
	firstColl = true;
	e->enemyBaseComponent.anim->AnimEventReset();*/
}

void EnemyAttack1::Finish()
{
	/*TrashEnemy* e = GetBase<TrashEnemy>();
	e->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
	e->enemyBaseComponent.effect->StopEffekseer(Effect_ID::ENEMY_ATTACK_CICLE);*/
}
