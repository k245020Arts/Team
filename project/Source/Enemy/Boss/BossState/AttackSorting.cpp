#include "AttackSorting.h"
#include "../Boss.h"
#include "../../../State/StateManager.h"
#include "BossStatus.h"
#include "../../../Common/Random.h"

namespace {
	const int ATTACK_KIND_MAX		= 6;
	const int COMBO_ATTACK_KIND_MAX = 3;
	/*enum COMBO_ATTACK
	{
		NORMAL_COMBO1,
		NORMAL_COMBO2,
		JUMP_COMBO,
		RUN_COMBO,
	};
	std::vector<COMBO_ATTACK> comboKind{
		{NORMAL_COMBO1},
		{NORMAL_COMBO2},
		{JUMP_COMBO},
		{RUN_COMBO},
	};*/
	//単発攻撃の種類
	std::vector<StateID::State_ID> attackKind{
		{StateID::BOSS_NORMAL_ATTACK1_S},
		{StateID::BOSS_NORMAL_ATTACK2_S},
		{StateID::BOSS_NORMAL_ATTACK3_S},
		{StateID::BOSS_SPECIAL_ATTACK1_S},
		{StateID::BOSS_SPECIAL_SMALL_ATTACK1_S},
		{StateID::BOSS_SPECIAL_ATTACK2_S},
		{StateID::BOSS_NORMAL_ATTACK4_S},
		{StateID::BOSS_NORMAL_ATTACK5_S},
		{StateID::BOSS_NORMAL_ATTACK6_S},
	};
	//通常攻撃の重み
	const std::vector<std::vector<double>> normalAttackParam{
		{ 1.0,	1.0,	0.5,	0.0,	0.0,	0.0,	1.0,	1.0,	0.5 },
		{ 1.0,	1.0,	1.0,	0.0,	0.0,	0.0,	1.0,	1.0,	1.0  },
		{ 0.1,	0.1,	0.5,	0.7,	1.0,	0.1,	0.5,	0.5,	0.5  },
		{ 0.05,	0.05,	1.0,	0.5,	0.9,	0.1,	0.5,	0.5,	0.5  },
	};
	//コンボ攻撃の重み
	const std::vector<std::vector<double>> comboAttackParam{
		{ 1.0,	0.5,	0.0,	0.0},
		{ 0.8,	0.7,	0.2,	0.0},
		{ 0.5,	1.0,	0.5,	0.1},
		{ 0.2,	0.5,	0.7,	0.1},
	};
	//コンボ攻撃の順番
	const std::vector<std::vector<StateID::State_ID>> comboOrder{
		{StateID::BOSS_NORMAL_ATTACK1_S,		StateID::BOSS_NORMAL_ATTACK2_S,			StateID::BOSS_NORMAL_ATTACK3_S},
		{StateID::BOSS_NORMAL_ATTACK4_S,		StateID::BOSS_NORMAL_ATTACK5_S,			StateID::BOSS_NORMAL_ATTACK6_S},
		{StateID::BOSS_SPECIAL_SMALL_ATTACK1_S, StateID::BOSS_SPECIAL_SMALL_ATTACK1_S,	StateID::BOSS_SPECIAL_ATTACK1_S },
		{StateID::BOSS_SPECIAL_ATTACK2_S,		StateID::BOSS_SPECIAL_ATTACK2_S,		StateID::BOSS_SPECIAL_ATTACK2_S,	StateID::BOSS_SPECIAL_ATTACK2_S}
	};
}

AttackSorting::AttackSorting()
{
	coolTime	= 0;
	string		= Function::GetClassNameC<AttackSorting>();
	hp			= Boss::MAX;
	attackNum	= 0;
	jump		= false;
	kind		= 0;

}

AttackSorting::~AttackSorting()
{

}

void AttackSorting::Update()
{
	Boss* b = GetBase<Boss>();

	if (b->maxAttack != -1) {
		b->enemyBaseComponent.state->ChangeState(comboOrder[kind][attackNum - b->maxAttack]);
	}
	else {
		b->enemyBaseComponent.state->ChangeState(attackKind[kind]);
	}
}

void AttackSorting::Start()
{
	animId = ID::B_IDOL;
	EnemyStateBase::Start();
	Boss* b = GetBase<Boss>();
	if (b->maxAttack >= 0) {
		b->maxAttack--;
		if (b->maxAttack == -1) {
			NormalAttackSelect();
		}
		b->comboFirstAttack = false;
		return;
	}
	int maxAttack = b->bs->GetStatus().maxAttack;
	int randam = GetRand(1);
	hp = b->Hp();
	//コンボ攻撃をするか決める
	float comboAttackRate = 0.0f;
	switch (hp)
	{
	case Boss::MAX:
		comboAttackRate = 0.0f;
		break;
	case Boss::EIGHT:
		comboAttackRate = 0.2f;
		break;
	case Boss::FIVE:
		comboAttackRate = 0.7f;
		break;
	case Boss::THREE:
		comboAttackRate = 0.9f;
		break;
	}

	bool combo = Random::GetBernoulli(comboAttackRate);
	if (combo) {
		std::vector<double> rand = comboAttackParam[hp];
		VECTOR3 dist = b->obj->GetTransform()->position - b->enemyBaseComponent.playerObj->GetTransform()->position;
		float size = dist.Size();
		if (size >= 5000.0f) {
			rand[3] += 2.0f;
		}
		kind = Random::GetWeightedIndex(rand);
		b->maxAttack = (int)comboOrder[kind].size() - 1;
		attackNum = b->maxAttack;
		b->comboFirstAttack = true;
	}
	else {
		NormalAttackSelect();
	}
	
}

void AttackSorting::Finish()
{

}

void AttackSorting::NormalAttackSelect()
{
	Boss* b = GetBase<Boss>();
	std::vector<double> rand = normalAttackParam[hp];
	/*switch (hp)
	{
	case AttackSorting::MAX:
		rand = 
		break;
		break;
	case AttackSorting::EIGHT:
		rand = 
		break;
	case AttackSorting::FIVE:
		
		break;
	case AttackSorting::THREE:
		
		break;
	default:
		break;
	}*/
	VECTOR3 dist = b->obj->GetTransform()->position - b->enemyBaseComponent.playerObj->GetTransform()->position;
	float size = dist.Size();
	if (size > 1400.0f) {
		rand[6] += 0.5f;
		rand[7] += 0.5f;
		rand[8] += 0.5f;
	}
	else {
		rand[0] += 0.5f;
		rand[1] += 0.5f;
		rand[2] += 0.5f;
	}
	if (size > 5000.0f) {
		rand[5] += 3.0f;
	}
	kind = Random::GetWeightedIndex(rand);
	b->maxAttack = -1;
}

int AttackSorting::AttackPriority()
{
	return 0;
}
