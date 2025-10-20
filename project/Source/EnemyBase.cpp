#include "EnemyBase.h"
#include "Animator.h"
#include "color.h"
#include "stateManager.h"
#include "weaponManager.h"

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

void EnemyBase::DrawTrail() {

	//剣の軌跡を作成
	enemyBaseComponent.weapon->CreateTrailEnemy(VECTOR3(0, 0, 0), VECTOR3(-70, 200, -230), 100.0f, 10.0f, 200.0f, 255.0f, 28, 0.5f);
}