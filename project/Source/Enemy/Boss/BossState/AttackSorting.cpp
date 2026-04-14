#include "AttackSorting.h"
#include "../Boss.h"
#include "../../../State/StateManager.h"
#include "BossStatus.h"
#include "../../../Common/Random.h"
#include "../../../Common/FileSystemUtils/FileSystemUtils.h"
#include "Attack/BossAttackBase.h"
#include "../../../Common/ResourceLoader.h"

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
	struct ActionParam
	{
		std::string id;
		bool attackState; //攻撃のStateか
		int priority;	//プライオリティ
		int weight;		//重さ
		int maxAction;	//連続で何回行動できるか

		float distance = 0;//距離によってその技が出やすいかどうか
		int addWeight = 0;//数字変動
	};
	std::vector<ActionParam> actions 
	{
		{"BossWalk",				false,10, 40, 0},
		{"BossNormalAttack1",		true,10, 10, 1},
		{"BossNormalAttack2",		true,10, 10, 1},
		{"BossNormalAttack3",		true,10, 10, 1},
		{"BossNormalAttack4",		true,10, 10, 1},
		{"BossNormalAttack5",		true,30, 10, 1},
		{"BossNormalAttack6",		true,30, 10, 1},
		{"BossNormalAttack7",		true,10, 10, 0, 2000},
		{"BossSpecialAttack1",		true,50, 30, 1},
		{"BossSpecialSmallAttack1",	true,50, 30, 1},
		{"BossSpecialAttack2",		true,50, 0,  1, 2000},
		{"BossBackStep",			false,50, 30, 0},
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

	bossPriority = 0;
	copyPriority = 0;
	moveCounter = 0;

	copyState = StateID::STATE_MAX;

	vec = VZero;
}

AttackSorting::~AttackSorting()
{
	for (auto& attack : attacks) {
		delete attack.second;
	}
	attacks.clear();
}

//#define NORMAL_MODE

void AttackSorting::Update()
{
	Boss* b = GetBase<Boss>();
	coolTime += Time::DeltaTimeRate();
	if (coolTime <= COOLTIME)
		return;

	if (b->maxAttack != -1)
		b->enemyBaseComponent.state->ChangeState(comboOrder[kind][attackNum - b->maxAttack]);
#ifdef NORMAL_MODE
	else 
		b->enemyBaseComponent.state->ChangeState(nextState/*attackKind[kind]*/);

#else
	else {
		if (nextAttack) {
			if (attacks[nextState] == nullptr) {
				b->BossAttackStateChange();
				return;
			}
			attacks[nextState]->Update();
		}
		else {
			b->enemyBaseComponent.state->ChangeState(StateID::StringToID(nextState));
		}
	
	}

#endif
}

void AttackSorting::Start()
{
	animId = ID::B_IDOL;
	EnemyStateBase::Start();
	Boss* b = GetBase<Boss>();
	/*if (b->maxAttack >= 0) {
		b->maxAttack--;
		if (b->maxAttack == -1) {
			NormalAttackSelect();
		}
		b->comboFirstAttack = false;
		return;
	}*/
	/*int maxAttack = b->bs->GetStatus().maxAttack;
	int randam = GetRand(1);*/
	hp = b->Hp();
	//コンボ攻撃をするか決める
	float comboAttackRate = 0.0f;

	switch (hp)
	{
	case Boss::MAX:
		comboAttackRate = 0.0f;
		bossPriority = 10;
		break;
	case Boss::EIGHT:
		comboAttackRate = 0.2f;
		bossPriority = 30;
		break;
	case Boss::FIVE:
		comboAttackRate = 0.7f;
		bossPriority = 50;
		break;
	case Boss::THREE:
		comboAttackRate = 0.9f;
		bossPriority = 80;
		break;
	}
	vec = b->enemyBaseComponent.playerObj->GetTransform()->position - b->GetEnemyObj()->GetTransform()->position;
	BuildTable(bossPriority);
	/*bool combo = Random::GetBernoulli(comboAttackRate);
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
	}*/

	b->comboFirstAttack = true;

	coolTime = 0;
}

void AttackSorting::Finish()
{
	copyState = nextState;
	moveCounter++;
	AttackFinish();
	forceAttack = false;
}

void AttackSorting::ForcedAttackStart(std::string _attackID)
{
	nextState = _attackID;
	nextAttack = true;
	AllAddWeightZero();
	AttackStart();
	forceAttack = true;
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

void AttackSorting::BuildTable(int _priority)
{
	if (forceAttack) {
		return;
	}
	int totalWeight = 0;
	
	for (auto& itr : actions)
	{
		if (itr.id == copyState)//一個前の行動と同じとき
		{
			if (itr.maxAction < moveCounter)//指定した行動
			{
				copyPriority = itr.priority;
				itr.priority = 0;
				continue;
			}
		}
		else
			moveCounter = 0;

		if (itr.priority > _priority)
			continue;
		else
		{
			if (itr.distance!=0.0f&&vec.Size() > itr.distance)
				itr.addWeight = 3000;

			totalWeight += itr.weight + itr.addWeight;
		}
			
	}

	//打てる技の合計からランダムな数字をだす
	int r = GetRand(totalWeight - 1);

	for (auto& itr : actions)
	{
		if (itr.priority == 0)
		{
			itr.priority = copyPriority;
			continue;
		}
		else if(itr.priority > _priority)//プライオリティを超えてるとき
			continue;

		r -= itr.weight + itr.addWeight;
	
		if (r < 0)
		{
			//AttackFinish();
			nextState = itr.id;
			nextAttack = itr.attackState;
			AllAddWeightZero();
			AttackStart();
			break;
		}
	}
}

void AttackSorting::AllAddWeightZero()
{
	for (auto& itr : actions)
	{
		if (itr.addWeight == 0)
			continue;

		itr.addWeight = 0;
	}
}


void AttackSorting::Load(std::string _bossName,Boss* _boss) 
{

	std::unordered_map<std::string, BossAttackBase::BossAttackParam> attackParam;
	
	
	//int attackNum = FileSystemUtils::GetDirectoryCount("data/json/BossAttack/" + _bossName);

	std::string filePath = "data/json/BossAttack/" + _bossName;

	for (const auto& entry : std::filesystem::directory_iterator(filePath)) {
		std::string fileName = entry.path().filename().string();
		std::string key = entry.path().stem().string();
		JsonReader jsonReader;
		JSON root;
		if (!jsonReader.Load(filePath + "/" + fileName))
		{
			return;
		}

		root = jsonReader.Data();

		if (root.contains(key))
		{
			attackParam[key] = root[key].get<BossAttackBase::BossAttackParam>();
		}
		else if (!root.empty())
		{
			// フォールバック
			attackParam[key] = root.begin().value().get<BossAttackBase::BossAttackParam>();
		}

		// 念のためID補完
		if (attackParam[key].attackID.empty())
		{
			attackParam[key].attackID = key;
		}


		ID::IDType bossAttackAnimID = static_cast<ID::IDType>(attackParam[key].animNum);

		ResourceLoad::LoadAnim(attackParam[key].animFileName, bossAttackAnimID);

		attackParam[key].animID = ID::StringToID(attackParam[key].animFileName);

		attacks[key] = new BossAttackBase();

		attacks[key]->Init(obj,StateID::StringToID(key));

		attacks[key]->SetAttackParam(attackParam[key]);
		
	}
	for (auto& t : attacks) {
		t.second->SetComponent<Boss>(_boss);
	}

}

void AttackSorting::AttackStart() 
{
	if (attacks[nextState] == nullptr) {
		Debug::DebugLog("ボスの攻撃がスタートできません");
		return;
	}
	attacks[nextState]->BossStart();
}

void AttackSorting::AttackFinish() 
{
	if (attacks[nextState] == nullptr) {
		Debug::DebugLog("ボスの攻撃が終了できません");
		return;
	}
	attacks[nextState]->BossFinish();
}

BossAttackBase* AttackSorting::GetNowAttackState()
{
	return attacks[nextState];
}
