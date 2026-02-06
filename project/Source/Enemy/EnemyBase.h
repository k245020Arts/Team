#pragma once
#include "../CharaBase/CharaBase.h"
#include "enemyInformation.h"
#include "TrashEnemy/EnemyState/EnemyDamage.h"
#include "../Common/ID/EffectID.h"

class EnemyBase : public CharaBase
{
public:
	friend class EnemyStateBase;
	EnemyBase();
	virtual ~EnemyBase();

	void Update()override;
	void Draw()override;

	template<typename T>
	T* CollsionStart(CollsionInformation::Shape _shape, Transform _trans) {
		return nullptr;
	}

	StateManager* GetStateManager() { return enemyBaseComponent.state; }
	BaseObject* GetEnemyObj() { return obj; }

	virtual void PlayerHit() { hit = true; }
	void DrawTrail();

	void EnemyDamageMove(EnemyDamage::EnemyDamageInfo _info);
	bool GetHit() { return hit; }
	void SetAlpha(int _alpha) { alpha = _alpha; }

	void SpecialAttackHit() { specialAttackHit = true; }
	
	/// <summary>
	/// ダメージ計算式
	/// </summary>
	/// <param name="_pos">ポジションを入れる</param>
	/// <param name="_damage">食らうダメージを入れる</param>
	/// <param name="_defense">防御数値を入れる</param>
	/// <param name="deviation">ダメージ表記するときの数字の誤差</param>
	/// <returns></returns>
	float DamageCalculation(VECTOR3 _pos,float _damage, float _defense,float deviation);

protected:

	EnemyInformation::EnemyComponent enemyBaseComponent;
	float damageFlash;
	float hitCounter;
	int loopNum;
	StateManager* pState;
	Object2D* damageNum;
	
	const float MAX_HP = 100;
	bool hit;
	int alpha;

	int cursolImage;

	VECTOR3 upPos;

	bool specialAttackHit;
};
