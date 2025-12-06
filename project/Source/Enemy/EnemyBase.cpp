#include "EnemyBase.h"
#include "../Component/Animator/Animator.h"
#include "../Component/Color/color.h"
#include "../State/StateManager.h"
#include "../Weapon/WeaponManager.h"
#include "../Component/Physics/Physics.h"
#include "../Player/PlayerState/AttackState/PlayerAttackStateBase.h"
#include "../../Library/MemoryCount.h"

EnemyBase::EnemyBase()
{
	loopNum = -1;
	hit = false;
	hitCounter = 0.0f;
	pState = nullptr;
	alpha = 255;
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
	//if (hp <= 0.0f && enemyBaseComponent.state->GetState<StateBase>()->GetID() == StateID::E_ANIM_IDOL) {
	//	//enemyBaseComponent.state->NowChangeState(ID::E_DIE);
	//	enemyBaseComponent.state->SetNoStateChange(true);
	//}
	if (loopNum >= 0) {
		hitCounter -= Time::DeltaTimeRate();
		if (hitCounter <= 0.0f) {
			PlayerHit();
		}
	}
	if (pState->GetState<PlayerAttackStateBase>() != nullptr) {
		if (!pState->GetState<PlayerAttackStateBase>()->IsAttack()) {
			hit = false;
		}
	}
	const Color::Rgb r = enemyBaseComponent.color->GetRgb();
	enemyBaseComponent.color->setRGB(Color::Rgb(r.r, r.g, r.b, alpha));
}

void EnemyBase::Draw()
{
}

void EnemyBase::DrawTrail() {

	//剣の軌跡を作成
	enemyBaseComponent.weapon->CreateTrailEnemy(VECTOR3(0, 0, 0), VECTOR3(-70, 200, -230), 100.0f, 10.0f, 200.0f, 255.0f, 28, 0.5f);
}

void EnemyBase::EnemyDamageMove(EnemyDamage::EnemyDamageInfo _info)
{
	//敵がダメージを受けた時の吹っ飛ばし量の設定
	enemyBaseComponent.physics->SetVelocity(_info.speed * MGetRotY(enemyBaseComponent.playerObj->GetTransform()->rotation.y));
}
