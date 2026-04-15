#include "BossAttackDataSerializer.h"
#include "../../Common/InputManager/InputManager.h"
#include "../../../ImGui/imgui.h"
#include "BossState/AttackSorting.h"
#include <iostream>
#include "../../../Source/Common/FileSystemUtils/FileSystemUtils.h"
#include "../../Common/ResourceLoader.h"

#define ANIM_FILE

BossAttackDataSerializer::BossAttackDataSerializer() : BossAttackDataSerializer(nullptr,"")
{
}

BossAttackDataSerializer::BossAttackDataSerializer(std::shared_ptr<AttackSorting> _sort, std::string _bossName)
{
	windowMode = false;
	BossName = _bossName;
	sorting = _sort;
	attackParam = sorting->GetAttackParam();
	actions = sorting->GetActionParam();
	attackKeys.clear();
	for (auto& a : attackParam) {
		attackKeys.push_back(a.first);
	}
	bossAnim = nullptr;
	currentSelectAnimInfos = Animator::AnimFileInfo();


	std::string filePath = "data/model/animation";

	for (const auto& entry : std::filesystem::directory_iterator(filePath)) {
		// フォルダはスキップ
		if (entry.is_directory()) {
			continue;
		}

		std::string name = entry.path().stem().string();
		animFileName.push_back(name);
	}

}

BossAttackDataSerializer::~BossAttackDataSerializer()
{
}

void BossAttackDataSerializer::SetAnim(Animator* anim)
{
	bossAnim = anim;
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

	//攻撃リスト作成
	static int currentIndex = 0;

	std::vector<const char*> items;
	for (auto& k : attackKeys) {
		items.push_back(k.c_str());
	}
	bool change = false;
	if (!items.empty()) {
		change = ImGui::Combo("AttackSelect", &currentIndex, items.data(), (int)items.size());
	}
	if (change) {
		std::string selectedID = attackKeys[currentIndex];
		auto& param = attackParam[selectedID];
		currentSelectAnimInfos = bossAnim->GetSelectFileInfo(param.animFileName);
	}
	//選択中の攻撃取得
	if (!attackKeys.empty())
	{
		std::string selectedID = attackKeys[currentIndex];
		if (ImGui::BeginTabBar("EditTabs")) {

			if (ImGui::BeginTabItem("Attack")) {

				auto& param = attackParam[selectedID];

				// ActionParamをIDで探す
				ActionParam* actionPtr = nullptr;

				for (auto& a : actions) {
					if (a.id == selectedID) {
						actionPtr = &a;
						break;
					}
				}

				//表示
				ImGui::Separator();
				ImGui::Text("AttackID : %s", selectedID.c_str());

				//AttackParam（攻撃データ）
				/*ImGui::DragInt("AnimNum", &param.animNum);
				ImGui::DragText("AnimFile", (char*)param.animFileName.c_str(), 256);*/

				//ActionParam編集
				if (actionPtr) {
					ImGui::DragInt("Priority", &actionPtr->priority);
					ImGui::DragInt("Weight", &actionPtr->weight);
					ImGui::DragInt("MaxAction", &actionPtr->maxAction);
					ImGui::DragFloat("Distance", &actionPtr->distance);
					ImGui::DragInt("AddWeight", &actionPtr->addWeight);
					ImGui::Checkbox("IsAttack", &actionPtr->attackState);
				}

				// 保存
				if (ImGui::Button("ActionsSave")) {
					//sorting->SaveSorthing(BossName);

					/*AttackSave(newID);
					sorting->AddAttack(newParam, newID);*/

					ActionsSave();

					sorting->LoadSorting(BossName);
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Animation")) {
				// アニメ編集
				ImGui::Separator();
				ImGui::Text("Anim : %s", currentSelectAnimInfos.fileName.c_str());

				ImGui::Text("ModelHandle", &currentSelectAnimInfos.hModel);
				ImGui::Checkbox("Loop", &currentSelectAnimInfos.loop);

				ImGui::InputFloat("MaxFrame", &currentSelectAnimInfos.maxFrame);
				ImGui::InputFloat("PlaySpeed", &currentSelectAnimInfos.playSpeed);

				ImGui::InputFloat("EventStart", &currentSelectAnimInfos.eventStartTime);
				ImGui::InputFloat("EventEnd", &currentSelectAnimInfos.eventFinishTime);

				// fileNameは安全に編集
				static char fileBuffer[256];
				strcpy_s(fileBuffer, currentSelectAnimInfos.fileName.c_str());

				if (ImGui::InputText("FileName", fileBuffer, 256)) {
					currentSelectAnimInfos.fileName = fileBuffer;
					currentSelectAnimInfos.id = fileBuffer;
				}
				if (ImGui::Button("AnimSave")) {

					bossAnim->SetSelectFileInfo(currentSelectAnimInfos);

					bossAnim->AnimDataSave("BossAnimData");
					bossAnim->AnimDataLoad("BossAnimData");

				}
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
		

	}

	ImGui::Separator();

	//攻撃の追加
	ImGui::Separator();
	ImGui::Text("Add New Attack");

	// 入力欄
	ImGui::InputText("AttackID (FileName)", newAttackID, 64);
#ifdef ANIM_FILE
	ImGui::InputText("AnimFileName", newAnimFile, 128);

	std::vector<const char*> animItems;
	animItems.clear();

	for (auto& fileName : animFileName) {
		animItems.push_back(fileName.c_str());
	}


	static int animFileIndex = 0;

	if (!animItems.empty())
	{
		if (ImGui::Combo("AnimationFile", &animFileIndex, animItems.data(), (int)animItems.size()))
		{
			// 選択された瞬間だけ反映
			strcpy_s(newAnimFile, sizeof(newAnimFile), animFileName[animFileIndex].c_str());
			newAnimFile[sizeof(newAnimFile) - 1] = '\0';
		}
	}
#else
	static char inputBuf[128] = "";
	static int selectedIndex = -1;

	// 入力欄
	if (ImGui::InputText("AnimSearch", inputBuf, sizeof(inputBuf)))
	{
		selectedIndex = -1; // 入力変わったらリセット
	}

	//========================
	// ■ 候補生成（フィルタ）
	//========================
	std::vector<std::string> filtered;
	std::vector<const char*> items;

	for (auto& name : animFileName) // ← さっき作った一覧
	{
		// 部分一致検索
		if (std::string(name).find(inputBuf) != std::string::npos)
		{
			filtered.push_back(name);
			items.push_back(filtered.back().c_str());
		}
	}

	//========================
	// ■ Combo表示
	//========================
	if (!items.empty())
	{
		if (ImGui::Combo("Candidates", &selectedIndex, items.data(), (int)items.size()))
		{
			// 選択したら入力欄に反映
			strcpy_s(inputBuf, sizeof(inputBuf), filtered[selectedIndex].c_str());
		}
	}
#endif // ANIM_FILE

	//バリデーション
	bool canAdd = true;
	std::string idStr = newAttackID;

	// 空チェック
	if (idStr.empty()) {
		canAdd = false;
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "ID is empty");
	}

	// 重複チェック
	else if (attackParam.find(idStr) != attackParam.end()) {
		canAdd = false;
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "ID already exists");
	}

	// アニメーション未入力
	else if (strlen(newAnimFile) == 0) {
		canAdd = false;
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "AnimFile is empty");
	}
	//ファイル存在チェック
	else if (!std::filesystem::exists(ResourceLoad::ANIM_PATH + std::string(newAnimFile) + ".mv1")) {
		canAdd = false;
		ImGui::TextColored(ImVec4(1,0,0,1), "Anim file not found");
	}
	

	//AddAttackボタン
	if (!canAdd) {
		ImGui::BeginDisabled();
	}

	if (ImGui::Button("AddAttack"))
	{
		std::string newID = newAttackID;

		// AttackParam追加（ローカル）
		BossAttackBase::BossAttackParam newParam;
		newParam.attackID = newID;
		newParam.animFileName = newAnimFile;
		newParam.animNum = FileSystemUtils::GetFileCount("data/json/BossAttack/" + BossName);

		attackParam[newID] = newParam;

		ID::IDType bossAttackAnimID = static_cast<ID::IDType>(attackParam[newID].animNum);

		ResourceLoad::LoadAnim(attackParam[newID].animFileName, bossAttackAnimID);

		attackParam[newID].animID = ID::StringToID(attackParam[newID].animFileName);

		AttackSave(newID);
		sorting->AddAttack(newParam, newID);

		// ActionParam追加（ローカル）
		ActionParam newAction;
		newAction.id = newID;
		newAction.attackState = true;
		newAction.priority = 1;
		newAction.weight = 0;
		newAction.maxAction = 1;
		newAction.distance = 0.0f;
		newAction.addWeight = 0;

		actions.push_back(newAction);

		ActionsSave();

		sorting->LoadSorting(BossName);

		Animator::AnimFileInfo animInfos = Animator::AnimFileInfo();

		animInfos.fileName = newAnimFile;
		animInfos.id = newAnimFile;
		animInfos.hModel = ResourceLoad::GetHandle(ID::StringToID(animInfos.id));
		animInfos.maxFrame = MV1GetAnimTotalTime(animInfos.hModel, 0);

		bossAnim->SetSelectFileInfo(animInfos);

		bossAnim->AnimDataSave("BossAnimData");
		bossAnim->AnimDataLoad("BossAnimData");

		// 入力リセット
		newAttackID[0] = '\0';
		newAnimFile[0] = '\0';

		attackKeys.clear();
		for (auto& a : attackParam) {
			attackKeys.push_back(a.first);
		}
	}

	if (!canAdd) {
		ImGui::EndDisabled();
	}

	ImGui::End();
}

void BossAttackDataSerializer::ActionsSave()
{
	std::string filePath = std::string("data/json/BossAttack/" + BossName + "/Sorting") + "/AttackSort" + ".json";

	JsonReader json;
	json.Load(filePath);

	nlohmann::json& root = json.Data();

	if (!root.contains("AttackSort")) {
		root["AttackSort"] = nlohmann::json::object();
	}
	for (auto& attack : actions) {
		std::string key = attack.id;
		root["AttackSort"][key] = attack;
	}


	json.Save(filePath, root);
}

void BossAttackDataSerializer::AttackSave(std::string _attackID)
{
	JSON root;

	root[_attackID.c_str()] = attackParam[_attackID];

	JsonReader jsonReader;

	jsonReader.Save("data/json/BossAttack/" + BossName + "/" + _attackID + ".json", root);
}