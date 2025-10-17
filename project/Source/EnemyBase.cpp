#include "EnemyBase.h"
#include "Animator.h"
#include "color.h"
#include "stateManager.h"

EnemyBase::EnemyBase()
{
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::Update()
{
	if (!enemyBaseComponent.anim->AnimEventCan()) {
		if (attackColl != nullptr) {
			DeleteColliderComponent();
			attackColl = nullptr;
		}
	}
	//敵のダメージフラッシュが終わったら元に戻す
	if (damageFlash > 0.0f) {
		damageFlash -= Time::DeltaTimeRate();
		if (damageFlash < 0.0f) {
			enemyBaseComponent.color->setRGB(Color::Rgb(0.0f, 0.0f, 0.0f, 255.0f));
		}
	}
	if (CheckHitKey(KEY_INPUT_G)) {
		obj->GetTransform()->position = EnemyInformation::BASE_POS;
	}
	if (hp <= 0.0f && enemyBaseComponent.state->GetState<StateBase>()->GetID() == ID::E_ANIM_IDOL) {
		enemyBaseComponent.state->NowChangeState(ID::E_DIE);
		enemyBaseComponent.state->SetNoStateChange(true);
	}
	if (loopNum >= 0) {
		hitCounter -= obj->GetObjectTimeRate();
		if (hitCounter <= 0.0f) {
			PlayerHit();
		}
	}
}

void EnemyBase::Draw()
{
}
