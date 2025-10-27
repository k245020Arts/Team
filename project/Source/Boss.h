#pragma once
#include "EnemyBase.h"
#include"Object3D.h"
#include "BossInformation.h"

class Player;
class CharaWeapon;

static const float ChaseRange = 5000.0f;

class Boss : public EnemyBase
{
public:
	friend class AttackSorting;
	friend class BossIdol;
	friend class BossRun;
	friend class BossNormalAttack1;
	friend class BossNormalAttack2;
	friend class BossNormalAttack3;
	friend class BossSpecialAttack1;

	Boss();
	~Boss();
	void Update()override;
	void Draw()override;

	void Start(Object3D* _obj);

	//ÉvÉåÉCÉÑÅ[ÇÃï˚Ç…å¸Ç≠ä÷êî
	void LookPlayer();

	template<typename T>
	T* CollsionStart(CollsionInformation::Shape _shape, Transform _trans) {
		if (attackColl == nullptr) {
			CollsionInfo info = CharaBase::CollsionInstant<T>(_shape, _trans);
			info.tag = CollsionInformation::Tag::B_ATTACK;
			collName = "b_attack";
			attackColl->CollsionAdd(info, _trans, "b_attack");
		};
		return static_cast<T*>(attackColl);
	}

	void PlayerHit()override;
	void Drail(bool _right);

private:
	Boss* boss;
	
	//BossInformation::BossComponent bossComponent;
	Transform* bossTransform;

	const int BOSS_RIGHT_HAND_FRAME = 15;
	const int BOSS_LEFT_HAND_FRAME = 11;
	CharaWeapon* chara;

	int maxAttack;
};
