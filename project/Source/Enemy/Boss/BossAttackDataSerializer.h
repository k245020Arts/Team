#pragma once
#include <string>
#include <memory>
#include "BossState/Attack/BossAttackBase.h"

class AttackSorting;

class BossAttackDataSerializer
{
public:
	BossAttackDataSerializer();
	BossAttackDataSerializer(std::shared_ptr<AttackSorting> _sort,std::string _bossName);
	~BossAttackDataSerializer();

	void Update();

private:
	bool windowMode;
	std::string BossName;
	std::shared_ptr<AttackSorting> sorting;
	std::unordered_map<std::string, BossAttackBase::BossAttackParam> attackParam;
};