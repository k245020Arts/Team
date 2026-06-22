#pragma once
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class T_EnemyStatus
{
public:
	struct Status
	{
		float normalAttack1;//攻撃１のダメージ数値 
		float maxHp ;		//最大HP 
		float defense;
		float coolTime;		//一段目の攻撃までの時間 
		float runSpeed;		//移動速度 
		float range;		//idelとrunの切り替わり 
		float atkRange;		//runと攻撃の切り替わり 
		float playerRange; 
		float chaseRange;	//プレイヤーと離れたときに切り替わる 
		float cooperateRange;
	};

	T_EnemyStatus()
	{
		std::ifstream file("data/json/TrashEnemyData.json");
		json j;
		file >> j;

		status.normalAttack1	= j["normalAttack1"];
		status.maxHp			= j["maxHp"];
		status.defense			= j["defense"];
		status.coolTime			= j["coolTime"];
		status.runSpeed			= j["runSpeed"];
		status.range			= j["range"];
		status.atkRange			= j["atkRange"];
		status.playerRange		= j["playerRange"];
		status.chaseRange		= j["chaseRange"];
		status.cooperateRange	= j["cooperateRange"];
	}

	Status GetStatus() { return status; }

private:
	Status status;
};