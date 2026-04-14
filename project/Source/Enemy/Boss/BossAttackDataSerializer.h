#pragma once
#include <string>

class BossAttackDataSerializer
{
public:
	BossAttackDataSerializer();
	BossAttackDataSerializer(std::string _bossName);
	~BossAttackDataSerializer();

	void Update();

private:
	bool windowMode;
	std::string BossName;
};