#include "BossAttackDataSerializer.h"
#include "../../Common/InputManager/InputManager.h"
#include "../../../ImGui/imgui.h"

BossAttackDataSerializer::BossAttackDataSerializer() : BossAttackDataSerializer("")
{
}

BossAttackDataSerializer::BossAttackDataSerializer(std::string _bossName)
{
	windowMode = false;
	BossName = _bossName;
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
	ImGui::Text("ñÓÇ¡Ç€Å[");
	ImGui::End();
}