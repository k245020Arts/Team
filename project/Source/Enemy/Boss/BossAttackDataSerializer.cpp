#include "BossAttackDataSerializer.h"
#include "../../Common/InputManager/InputManager.h"
#include "../../../ImGui/imgui.h"
#include "BossState/AttackSorting.h"
#

BossAttackDataSerializer::BossAttackDataSerializer() : BossAttackDataSerializer(nullptr,"")
{
}

BossAttackDataSerializer::BossAttackDataSerializer(std::shared_ptr<AttackSorting> _sort, std::string _bossName)
{
	windowMode = false;
	BossName = _bossName;
	sorting = _sort;
	attackParam = sorting->GetAttackParam();
}

BossAttackDataSerializer::~BossAttackDataSerializer()
{
}

void BossAttackDataSerializer::Update() 
{
	if (InputManager::GetInstance()->KeyInputDown("BossParamWindow")) {
		windowMode = !windowMode;
	}
	if (!windowMode) {
		return;
	}
	ImGui::Begin("BossAttackSerialize");

	for (auto& attack : attackParam) {
		ImGui::Text(attack.second.attackID.c_str());
	}
	ImGui::End();
}