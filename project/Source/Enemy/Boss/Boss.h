#pragma once
#include "../EnemyBase.h"
#include "../../Component/Object/Object3D.h"
#include "BossInformation.h"

class Player;
class CharaWeapon;
class BossStatus;

class Boss : public EnemyBase
{
public:
	friend class AttackSorting;
	friend class BossCoolTime;
	friend class BossIdol;
	friend class BossRun;
	friend class BossNormalAttack1;
	friend class BossNormalAttack2;
	friend class BossNormalAttack3;
	friend class BossSpecialAttack1;
	friend class BossDie;

	Boss();
	~Boss();
	void Update()override;
	void Draw()override;

	void Start(Object3D* _obj);

	void ImguiDraw()override;

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
	BossStatus* bs;

	//BossInformation::BossComponent bossComponent;
	Transform* bossTransform;

	const int BOSS_RIGHT_HAND_FRAME = 15;
	const int BOSS_LEFT_HAND_FRAME = 11;
	CharaWeapon* chara;

	int maxAttack;
};
