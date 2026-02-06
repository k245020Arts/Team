#include "EnemyBase.h"
#include "../Component/Animator/Animator.h"
#include "../Component/Color/color.h"
#include "../State/StateManager.h"
#include "../Weapon/WeaponManager.h"
#include "../Component/Physics/Physics.h"
#include "../Player/PlayerState/AttackState/PlayerAttackStateBase.h"
#include "../Common/Memory/MemoryCount.h"
#include "EnemyManager.h"
#include "../Common/LoadManager.h"
#include "../Component/UI/EnemyDamageUI.h"
#include "../Component/ComponentManager.h"

EnemyBase::EnemyBase()
{
	loopNum = -1;
	hit = false;
	hitCounter = 0.0f;
	pState = nullptr;
	alpha = 255;
	FindGameObject<EnemyManager>()->AddList(this, obj);
	upPos = VECTOR3(0, 600, 0);
	cursolImage = -1;
	damageFlash = 0.0f;
	specialAttackHit = false;

}

EnemyBase::~EnemyBase()
{
	FindGameObject<EnemyManager>()->RemoveList(this, obj);
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
	/*const Color::Rgb r = enemyBaseComponent.color->GetRgb();
	enemyBaseComponent.color->setRGB(Color::Rgb(r.r, r.g, r.b, (float)alpha));*/
}

void EnemyBase::Draw()
{
	if (!lastTarget) {
		return;
	}
	DrawSphere3D(obj->GetTransform()->position + upPos, 20, 2, 0xffff00, 0xffff00, true);
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

float EnemyBase::DamageCalculation(VECTOR3 _pos, float _damage,float _defense, float deviation)
{
	float damage = _damage + (_damage * 2) / (_defense / 4);

	damageNum = new Object2D();

	damageNum->Init(Transform(VZero, VZero, VOne), "damageNum");
	damageNum->Component()->AddComponent<EnemyDamageUI>()->
		SetInformation(_pos+ VECTOR3(GetRand(100),GetRand(100),GetRand(100))/*VECTOR3(GetRand(400) - 200, 800 + GetRand(400) - 200, GetRand(400) - 200)*/,
		damage +GetRand(deviation), VECTOR3(0, -0.4f, 0), 0.5f,
		Load::GetHandle(ID::DAMAGE_UI_BUTTON), GetEnemyObj()->GetTransform(), VECTOR2I(81, 90));
	
	return damage;
}