#pragma once
#include "../EnemyBase.h"
#include "../../Component/Object/Object3D.h"
#include "BossInformation.h"
#include "../EnemyInformation.h"

class Player;
class CharaWeapon;
class BossStatus;
//class TrashEnemyManager;

class PlayerSpecialAttack;

class Boss : public EnemyBase
{
public:
	friend class AttackSorting;
	friend class BossCoolTime;
	friend class BossIdol;
	friend class BossRun;
	friend class BossWalk;
	friend class BossNormalAttack1;
	friend class BossNormalAttack2;
	friend class BossNormalAttack3;
	friend class BossNormalAttack4;
	friend class BossNormalAttack5;
	friend class BossNormalAttack6;
	friend class BossSpecialAttack1;
	friend class BossSpecialSmallAttack1;
	friend class BossSpecialAttack2;
	friend class BossDie;
	friend class BossRoar;
	friend class BossThreat;
	friend class BossAttackBase;
	friend class BossDamage;
	friend class BossAppear;
	friend class BossLose;
	
	enum HP_RATE
	{
		MAX,
		EIGHT,
		FIVE,
		THREE,
	};

	Boss();
	~Boss();
	void Update()override;
	void Draw()override;
	/// <summary>
	/// jsonを読み込み
	/// </summary>
	/// <param name="j">jsonデータ</param>
	/// <returns>jsonDataの中身</returns>
	EnemyInformation::EnemyReaction JsonRead(const JSON& j);
	/// <summary>
	/// ボスの開始
	/// </summary>
	/// <param name="_obj"></param>
	void Start(Object3D* _obj);
	
	void ImguiDraw()override;
	
	/// <summary>
	/// プレイヤーの方に向く関数
	/// </summary>
	void LookPlayer();

	/// <summary>
	/// プレイヤーの方に向く関数
	/// </summary>
	/// <param name="speed">振り向くスピード</param>
	void LookPlayer(float speed);

	/// <summary>
	/// 攻撃の当たり判定の開始
	/// </summary>
	/// <typeparam name="T">当たり判定のクラス名</typeparam>
	/// <param name="_shape">当たり判定の形</param>
	/// <param name="_trans">基準のTransform</param>
	/// <returns>当たり判定クラスのポインタ</returns>
	template<typename T>
	T* CollsionStart(CollsionInformation::Shape _shape, const Transform& _trans) {
		if (attackColl == nullptr) {
			CollsionInfo info = CharaBase::CollsionInstant<T>(_shape, _trans);
			info.tag = CollsionInformation::Tag::B_ATTACK;
			collName = "b_attack";
			attackColl->CollsionAdd(info, _trans, "b_attack");
		};
		return static_cast<T*>(attackColl);
	}
	/// <summary>
	/// playerにダメージを食らった時の判定
	/// </summary>
	void PlayerHit()override;
	/// <summary>
	/// 攻撃の軌跡を生成
	/// </summary>
	/// <param name="_right"></param>
	void Drail(bool _right);
	/// <summary>
	/// HPの割合を取得
	/// </summary>
	/// <returns>HPの割合の列挙型</returns>
	HP_RATE Hp();
	/// <summary>
	/// ボスが移動をする
	/// </summary>
	/// <param name="_speed">スピード</param>
	/// <param name="_max">最大スピード</param>
	void MoveBoss(float _speed,float _max);
	/// <summary>
	/// 攻撃のクールタイムの取得
	/// </summary>
	/// <returns>攻撃のクールタイム</returns>
	float GetAttackCoolTime();

	/// <summary>
	/// ボスの攻撃をさせたいときに呼ぶ
	/// </summary>
	void BossAttackStateChange();
	/// <summary>
	/// プレイヤーの範囲内なら攻撃
	/// </summary>
	/// <returns>trueなら攻撃可能</returns>
	bool RunChangeAttack();

private:
	void PlayerSpecialAttackHit(const EnemyInformation::EnemyReaction& _e, std::shared_ptr<PlayerSpecialAttack> _ps,VECTOR3 _randomPos,float _randomAngle);
	Boss* boss;
	BossStatus* bs;

	//BossInformation::BossComponent bossComponent;
	Transform* bossTransform;

	const int BOSS_RIGHT_HAND_FRAME = 15;
	const int BOSS_LEFT_HAND_FRAME = 11;
	CharaWeapon* chara;

	int maxAttack;
	bool comboFirstAttack;

	HP_RATE hpRate;
	bool roaf;

	//TrashEnemyManager* trashEnemy;
	float coolTime;

	float attackNum;
	Player* player;
	bool threat;

	float noAttackChangeCounter;
	int alotAttack;
	bool oneDie;
};
