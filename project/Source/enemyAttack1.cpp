#include "enemyAttack1.h"
#include "Animator.h"
#include "EnemyStateManager.h"
#include "enemy.h"
#include "EffectManager.h"
#include "LoadManager.h"
#include "SoundManager.h"
#include "weaponManager.h"
#include "camera.h"

EnemyAttack1::EnemyAttack1()
{
	animId = ID::E_ATTACK1;
	collTrans = Transform(VECTOR3(-100, 50, 100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	string = Function::GetClassNameC<EnemyAttack1>();
	id = ID::E_ATTACK1;
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
	if (e->enemyBaseComponent.anim->IsFinish()) {
		e->enemyBaseComponent.state->ChangeState(ID::E_ANIM_IDOL);
	}
	/*if (com.anim->GetCurrentFrame() >= 12.0f && com.anim->GetCurrentFrame() <= 20.0f) {
		com.anim->SetPlaySpeed(0.2f);
	} 
	else {
		com.anim->SetPlaySpeed(1.0f);
	}*/
	float time = e->enemyBaseComponent.anim->EventStartTime(ID::E_ATTACK1);
	//“G‚ÌUŒ‚‚Ì‰¹‚ğ”­¶
	if (time - 1.0f <= e->enemyBaseComponent.anim->GetCurrentFrame() && time >= e->enemyBaseComponent.anim->GetCurrentFrame()) {
		if (!e->enemyBaseComponent.sound->CheckSe(Sound_ID::ENEMY_SWORD_WIND1) && !e->enemyBaseComponent.sound->CheckSe(Sound_ID::ENEMY_SWORD_WIND2)) {
			e->enemyBaseComponent.sound->RandamSe("EnemySword", 2);
		}
	}
	//“G‚ÌŒ•‰ñ‚è‚ğŒõ‚ç‚¹‚Ä‚¢‚é‚±‚Æ‚Ö‚Ìİ’è
	if (time - 7.0f <= e->enemyBaseComponent.anim->GetCurrentFrame() && time >= e->enemyBaseComponent.anim->GetCurrentFrame()) {
		if (sound) {
			e->enemyBaseComponent.effect->CreateEffekseer(Transform(MV1GetFramePosition(Load::GetHandle(ID::IDType::E_MODEL), 12), VZero, VOne * 3.0f), nullptr, Effect_ID::ENEMY_FLASH, 1.0f);
			e->enemyBaseComponent.sound->PlaySe(Sound_ID::ENEMY_ATTACK_BEFORE);
			e->enemyBaseComponent.sound->RandamSe("E_AttackV", 3);
			//com.weapon->CreateTrailEnemy(VECTOR3(0, 0, 0), VECTOR3(500, 500, 1000) * MGetRotY(com.enemy->GetEnemyTransform()->rotation.y), 100.0f, 10.0f, 200.0f, 255.0f, 28, 0.5f);
			sound = false;
		}
		//com.weapon->CreateTrailEnemy(VECTOR3(0, 0, 0), VECTOR3(0, 0, 300) * MGetRotY(com.enemy->GetEnemyTransform()->rotation.y), 100.0f, 10.0f, 200.0f, 255.0f, 28, 1.0f);
	}
	//Œ•‚Ì‹OÕ‚ğ”­¶
	if (time - 7.0f <= e->enemyBaseComponent.anim->GetCurrentFrame() && time + 5.0f >= e->enemyBaseComponent.anim->GetCurrentFrame()) {
		e->enemyBaseComponent.enemy->DrawTrail();
	}
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
	Enemy* e = GetBase<Enemy>();
	EnemyStateBase::Start();
	firstColl = true;
	sound = true;
	effectAverageSpeed = 60.0f / e->enemyBaseComponent.anim->EventStartTime(animId);
	e->enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(0, 100, 0), VZero, VOne), e->enemyBaseComponent.enemy->GetEnemyObj(), Effect_ID::ENEMY_ATTACK_CICLE, 1.0f);
}

void EnemyAttack1::Finish()
{
	Enemy* e = GetBase<Enemy>();
	e->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
	e->enemyBaseComponent.effect->StopEffekseer(Effect_ID::ENEMY_ATTACK_CICLE);
}
