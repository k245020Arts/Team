#include "BossAttackDataSerializer.h"
#include "../../Common/InputManager/InputManager.h"
#include "../../../ImGui/imgui.h"
#include "BossState/AttackSorting.h"
#include <iostream>
#include "../../../Source/Common/FileSystemUtils/FileSystemUtils.h"
#include "../../Common/ResourceLoader.h"
#include "../../../Source/Component/EnemyAttackObject/BossRock/BossRockBase.h"

#define ANIM_FILE

BossAttackDataSerializer::BossAttackDataSerializer() : BossAttackDataSerializer(nullptr,nullptr,"")
{
}

BossAttackDataSerializer::BossAttackDataSerializer(std::shared_ptr<AttackSorting> _sort, Boss* _boss, std::string _bossName)
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
	for (char& c : newAnimFile) {
		c = '\0';
	}
	for (char& c : newAttackID) {
		c = '\0';
	}
	rockManager = nullptr;
	boss = _boss;
	isEffect = false;
}

BossAttackDataSerializer::~BossAttackDataSerializer()
{
}

void BossAttackDataSerializer::SetAnim(Animator* anim)
{
	bossAnim = anim;
}

void BossAttackDataSerializer::SetThrowManager(BossRockManager* _data)
{
	rockManager = _data;
	throwObjectsData = rockManager->GetThrowObjectsData();
}

void BossAttackDataSerializer::Update()
{
	if (InputManager::GetInstance()->KeyInputDown("BossParamWindow")) {
		windowMode = !windowMode;
		if (windowMode) {
			boss->GetBaseObject()->SetObjectTimeRate(0.0f);
		}
		else {
			boss->GetBaseObject()->SetObjectTimeRate(1.0f);
		}
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
			//攻撃のソートの調整
			if (ImGui::BeginTabItem("AttackSort")) {

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

					ActionsSave();

					sorting->LoadSorting(BossName);
				}
				ImGui::EndTabItem();
			}
			//攻撃のイベントパラメータの追加
			if (ImGui::BeginTabItem("AttackParam"))
			{
				DrawAttackParamEditor(selectedID);
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
			//投擲物の追加
			if (ImGui::BeginTabItem("ThrowObjectAdd"))
			{
				DrawAddThrowObjects(throwObjectsData);
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

void BossAttackDataSerializer::DrawAttackParamEditor(std::string _selectID)
{
	auto& param = attackParam[_selectID];

	//========================
	// ■ 基本
	//========================
	if (ImGui::CollapsingHeader("Basic", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("HitDamage", &param.hitDamage, 0.1f);
		ImGui::DragFloat("SlowTime", &param.slowTime, 0.01f);
		ImGui::DragFloat("SlowAmount", &param.slowAmout, 0.01f);
		ImGui::DragFloat("SpeedUp", &param.speedUpMotionSpeed, 0.01f);

		ImGui::DragFloat("AttackStart##Basic", &param.attackCollsionStartTime, 0.01f);
		ImGui::DragFloat("AttackEnd##Basic", &param.attackCollsionEndTime, 0.01f);

		ImGui::DragFloat("JustAvoidStart##Basic", &param.justAvoidCollsionStartTime, 0.01f);
		ImGui::DragFloat("JustAvoidEnd##Basic", &param.justAvoidCollsionEndTime, 0.01f);

		ImGui::DragInt("AttackFrame", &param.attackPositionFrameNum, 1);
		ImGui::DragFloat("SoundTime", &param.attackSoundStartTime, 0.01f);

		DrawTransform("AttackCollTransform", param.attackCollTransform);
		DrawTransform("JustAvoidTransform", param.justAvoidCollTransform);

		ImGui::Checkbox("UseFlash", &param.useFlash);
		ImGui::DragFloat("FlashStartTime", &param.attackFlashStartTime, 0.01f);
	}

	//========================
	// ■ 移動
	//========================
	if (ImGui::CollapsingHeader("Move Event"))
	{
		ImGui::Checkbox("FrontMove", &param.frontMove);
		if (param.frontMove)
		{
			ImGui::DragFloat("MoveSpeed##Move", &param.baseSpeed, 0.1f);
			ImGui::DragFloat("StartTime##Move", &param.moveStartTime, 0.01f);
			ImGui::DragFloat("EndTime##Move", &param.moveFinishTime, 0.01f);
			ImGui::Checkbox("AddVelocity##Move", &param.addVelocity);
		}
	}

	//========================
	// ■ プレイヤー追従
	//========================
	if (ImGui::CollapsingHeader("Player Follow"))
	{
		ImGui::Checkbox("Enable##Follow", &param.playerAloowMove);

		if (param.playerAloowMove)
		{
			ImGui::DragFloat("BaseSpeed##Follow", &param.baseSpeed, 0.1f);
			ImGui::Checkbox("NearBossSpeedStop", &param.playerNearStop);
			ImGui::Checkbox("AllowPlayerStop", &param.playerNearAloowStop);
			ImGui::Checkbox("AddVelocity##Follow", &param.addVelocity);

			ImGui::DragFloat("NearDist", &param.playerBaseNear, 0.1f);
			ImGui::DragFloat("MaxSpeed", &param.maxMoveSpeed, 0.1f);
			ImGui::DragFloat("MinSpeed", &param.minMoveSpeed, 0.1f);

			ImGui::DragFloat("StartTime##Follow", &param.moveStartTime, 0.01f);
			ImGui::DragFloat("EndTime##Follow", &param.moveFinishTime, 0.01f);
		}
	}

	//========================
	// ■ 突進
	//========================
	if (ImGui::CollapsingHeader("Rush Event"))
	{
		ImGui::Checkbox("Enable##Rush", &param.rushMove);

		if (param.rushMove)
		{
			ImGui::DragFloat("Speed##Rush", &param.rushAfterSpeed, 0.1f);
			ImGui::DragFloat("Time##Rush", &param.rushTime, 0.01f);

			ImGui::Checkbox("Collision", &param.rushColl);
			ImGui::DragFloat("CollScale", &param.addRushCollScale, 0.1f);

			ImGui::DragFloat("RightFootSE", &param.rushSoundRightFoot, 0.01f);
			ImGui::DragFloat("LeftFootSE", &param.rushSoundLeftFoot, 0.01f);
		}
	}

	//========================
	// ■ 回転
	//========================
	if (ImGui::CollapsingHeader("Rotate Event"))
	{
		ImGui::Checkbox("Enable##Rotate", &param.rotateMove);

		if (param.rotateMove)
		{
			ImGui::DragFloat("Angle", &param.angleMoveAmout, 0.1f);
		}
	}

	//========================
	// ■ ジャンプ
	//========================
	if (ImGui::CollapsingHeader("Jump Event"))
	{
		ImGui::Checkbox("Enable##Jump", &param.jump);

		if (param.jump)
		{
			ImGui::DragFloat("JumpSpeed", &param.jumpSpeed, 0.1f);
			ImGui::DragFloat("Gravity", &param.addGravity, 0.1f);

			ImGui::DragFloat("StartTime##Jump", &param.jumpStartTime, 0.01f);
			ImGui::DragFloat("EffectTime", &param.groundEffectStartTime, 0.01f);

			ImGui::DragFloat("ShakePower", &param.groundShakeCamera, 0.1f);
			ImGui::DragFloat("ShakeTime", &param.groundShakeTime, 0.01f);
		}
	}

	//========================
	// ■ 衝撃波
	//========================
	if (ImGui::CollapsingHeader("ShockWave"))
	{
		ImGui::Checkbox("Enable##Shock", &param.shockWave);

		if (param.shockWave)
		{
			ImGui::DragFloat("Speed", &param.shockWaveSpeed, 0.1f);
			ImGui::DragFloat("StartRange", &param.startRange, 0.1f);
		}
	}

	//========================
	// ■ 投擲
	//========================
	if (ImGui::CollapsingHeader("Throw Event"))
	{
		ImGui::Checkbox("Enable##Throw", &param.throwObject);

		if (param.throwObject)
		{
			ImGui::Checkbox("ArmThrow", &param.armThrow);
			ImGui::DragInt("ArmFrame", &param.armFrameNum, 1);

			ImGui::DragFloat("StartTime", &param.throwStartTime, 0.01f);
			ImGui::DragFloat("AppearTime", &param.throwObjectApperaTime, 0.01f);

			ImGui::DragInt("ThrowNum", &param.throwObjectNum, 1);
			ImGui::DragFloat3("SpawnPos", &param.objectApperaPosition.x, 0.1f);

			ImGui::DragFloat("Interval", &param.intervalTime, 0.01f);
			ImGui::Checkbox("IntervalSub", &param.intervalTimeSub);
			ImGui::DragFloat("MaxInterval", &param.maxIntervalTime, 0.01f);
			ImGui::DragFloat("MinInterval", &param.minIntervalTime, 0.01f);

			ImGui::Separator();

			DrawThrowObjectEditor(param.throwAttackData);
		}
	}

	//========================
	// ■ カメラ
	//========================
	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::Checkbox("BossLook", &param.attackCameraBossLook);
		ImGui::DragFloat("ChangeSpeed", &param.cameraChangeSpeed, 0.1f);
	}

	//========================
	// ■ トレイル
	//========================
	if (ImGui::CollapsingHeader("Trail"))
	{
		ImGui::Checkbox("UseTrail", &param.useTrail);

		if (param.useTrail)
		{
			ImGui::Checkbox("RightHand", &param.trailRightHand);
		}
	}

	CopyParam(_selectID);

	//========================
	// ■ 保存
	//========================
	if (ImGui::Button("ParamSave")) {
		AttackSave(_selectID);
		sorting->ReloadParam(param, _selectID);
	}
}

void BossAttackDataSerializer::DrawTransform(const char* label, Transform& t)
{
	if (ImGui::TreeNode(label))
	{
		ImGui::DragFloat3("Position", &t.position.x, 0.1f);
		ImGui::DragFloat3("Rotation", &t.rotation.x, 0.1f);
		ImGui::DragFloat3("Scale", &t.scale.x, 0.1f);
		ImGui::TreePop();
	}
}

void BossAttackDataSerializer::DrawThrowObjectEditor(std::vector<BossAttackBase::ThrowObjectAttackData>& list)
{
	static int selectIndex = -1;

	ImGui::Separator();
	ImGui::Text("Throw Object List");

	// ■ リスト
	for (int i = 0; i < list.size(); i++)
	{
		std::string label = std::to_string(i) + " : " + list[i].throwObjectID;

		if (ImGui::Selectable(label.c_str(), selectIndex == i))
		{
			selectIndex = i;
		}
	}

	// ■ 追加
	if (ImGui::Button("Add Empty"))
	{
		list.push_back(BossAttackBase::ThrowObjectAttackData());
		selectIndex = (int)list.size() - 1;
	}

	ImGui::SameLine();

	if (ImGui::Button("Add Copy") && selectIndex >= 0)
	{
		auto copy = list[selectIndex];

		list.push_back(copy);
		selectIndex = (int)list.size() - 1;
	}

	ImGui::SameLine();


	if (ImGui::Button("Delete") && selectIndex >= 0)
	{
		list.erase(list.begin() + selectIndex);
		selectIndex = -1;
	}

	if (selectIndex < 0 || selectIndex >= list.size()) return;

	auto& t = list[selectIndex];

	ImGui::Separator();
	ImGui::Text("Edit Throw Data");

	//------------------------------------
	// ■ ID（テキスト編集）
	//------------------------------------
	char idBuf[128];
	strcpy_s(idBuf, t.throwObjectID.c_str());
	std::string idNum = std::to_string(selectIndex);
	if (ImGui::InputText("ID", idBuf, sizeof(idBuf)))
	{
		t.throwObjectID = idBuf;
	}

	//------------------------------------
	// ■ 基本
	//------------------------------------
	if (ImGui::CollapsingHeader("Base"))
	{
		ImGui::DragFloat3("Gravity", &t.baseGravity.x, 0.1f);
		ImGui::DragFloat3("Friction", &t.baseFirction.x, 0.1f);
	}

	//------------------------------------
	// ■ Push Collision
	//------------------------------------
	if (ImGui::CollapsingHeader("PushCollision"))
	{
		ImGui::Checkbox("Enable##Push", &t.pushCollCan);

		if (t.pushCollCan)
		{
			DrawTransform("PushTransform", t.pushCollTransform);
		}
	}

	//------------------------------------
	// ■ Ground（Ray）
	//------------------------------------
	if (ImGui::CollapsingHeader("Ground"))
	{
		ImGui::Checkbox("Enable##Ground", &t.randCan);

		if (t.randCan)
		{
			DrawRayColliderInfo("RandCollider", t.randCollInfo);

			// 安全処理
			if (t.randCollInfo.rayStartPos > t.randCollInfo.rayFinishPos)
				std::swap(t.randCollInfo.rayStartPos, t.randCollInfo.rayFinishPos);
		}

		ImGui::DragFloat("LifeTime", &t.randTime, 0.1f);
	}

	//------------------------------------
	// ■ Player Hit
	//------------------------------------
	if (ImGui::CollapsingHeader("Player Hit"))
	{
		ImGui::Checkbox("Enable##PlayerHit", &t.playerHit);

		if (t.playerHit)
		{
			ImGui::DragFloat("Radius", &t.playerHitCollRadius, 0.1f);
			ImGui::DragFloat("JustAvoidRadius", &t.playerHitJustAvoidCollRadius, 0.1f);
		}
	}

	//------------------------------------
	// ■ Ground Hit
	//------------------------------------
	if (ImGui::CollapsingHeader("Ground Hit"))
	{
		ImGui::Checkbox("Enable##GroundHit", &t.playerGroundHit);

		if (t.playerGroundHit)
		{
			ImGui::DragFloat("Radius", &t.playerGroundCollRadius, 0.1f);
			ImGui::Checkbox("playerGroundNoDamageReactionHit", &t.playerGroundNoDamageReactionHit);
			ImGui::Checkbox("playerGroundOneHit", &t.playerGroundOneHit);
		}
	}

	//------------------------------------
	// ■ Boss Hit
	//------------------------------------
	if (ImGui::CollapsingHeader("Boss Hit"))
	{
		ImGui::Checkbox("Enable##BossHit", &t.bossHit);

		if (t.bossHit)
		{
			ImGui::DragFloat("Radius", &t.bossHitCollRadius, 0.1f);
		}
	}

	//------------------------------------
	// ■ Player Attack Flying
	//------------------------------------
	if (ImGui::CollapsingHeader("Player Attack Flying"))
	{
		ImGui::Checkbox("Enable##Flying", &t.playerAttackFlying);

		if (t.playerAttackFlying)
		{
			ImGui::DragFloat("Radius", &t.playerAttackFlyingCollRadius, 0.1f);
			ImGui::DragFloat("Speed", &t.flyingSpeed, 0.1f);
			ImGui::DragFloat("Height", &t.flyingHeight, 0.1f);
		}
	}

	//------------------------------------
	// ■ Boss Rush Hit
	//------------------------------------
	if (ImGui::CollapsingHeader("Boss Rush Hit"))
	{
		ImGui::Checkbox("Enable##RushHit", &t.bossRushHit);

		if (t.bossRushHit)
		{
			ImGui::DragFloat("Radius", &t.bossRushHitCollRadius, 0.1f);
		}
	}

	//------------------------------------
	// ■ Blast（Dount）
	//------------------------------------
	if (ImGui::CollapsingHeader("Blast"))
	{
		ImGui::Checkbox("Enable##Blast", &t.blastCan);

		if (t.blastCan)
		{
			DrawDountColliderInfo("BlastCollider", t.blastColliderInfo);
			DrawDountColliderInfo("JustAvoidCollider", t.blastJustAvoidColliderInfo);

			// 安全処理
			t.blastColliderInfo.outRadius = max(t.blastColliderInfo.outRadius, t.blastColliderInfo.inRadius);

			ImGui::DragFloat("BlinkTime", &t.blastBlinkMaxCounter, 0.1f);
			ImGui::Checkbox("RandomBlast", &t.randomBlast);
			ImGui::DragFloat("Rate", &t.randomBlastRate, 0.1f);

			ImGui::DragFloat("MaxRadius", &t.maxRadius, 0.1f);
			ImGui::DragFloat("WaveSpeed", &t.waveSpeed, 0.1f);
		}
	}

	//------------------------------------
	// ■ Prediction（Ray）
	//------------------------------------
	if (ImGui::CollapsingHeader("Prediction"))
	{
		ImGui::Checkbox("Enable##Prediction", &t.predictionCicleCan);

		if (t.predictionCicleCan)
		{
			DrawRayColliderInfo("PredictionCollider", t.predictionCicleColliderInfo);
		}
	}

	//------------------------------------
	// ■ Throw
	//------------------------------------
	if (ImGui::CollapsingHeader("Throw"))
	{
		ImGui::Checkbox("ArmThrow", &t.armThrow);
		ImGui::DragInt("ArmFrame", &t.armFrameNum, 1);
		ImGui::DragFloat3("ArmOffset", &t.armAddPos.x, 0.1f);

		ImGui::Checkbox("ToPlayer", &t.throwToPlayer);
		ImGui::Checkbox("ToFront", &t.thorwToFront);

		ImGui::DragFloat("Speed", &t.throwSpeed, 0.1f);
		ImGui::DragFloat("UpSpeed", &t.upSpeed, 0.1f);
		ImGui::DragFloat("FirstSpeed", &t.throwFirstSpeed, 0.1f);

		ImGui::DragFloat3("Diffusion", &t.diffusionAngle.x, 0.1f);
	}

	//------------------------------------
	// ■ Fall
	//------------------------------------
	if (ImGui::CollapsingHeader("Fall"))
	{
		ImGui::Checkbox("Enable##Fall", &t.throwToFall);

		if (t.throwToFall)
		{
			ImGui::DragFloat("Height", &t.throwHeight, 0.1f);
			ImGui::DragFloat("Gravity", &t.throwFallGravity, 0.1f);
			ImGui::Checkbox("ToPlayer", &t.throwToFallToPlayer);
			ImGui::DragFloat3("ThrowStartPos", &t.thorwStartPos.x, 0.1f);
			ImGui::DragFloat("ThrowVelocity", &t.thorwVelocity.x, 0.1f);
		}
	}

	//------------------------------------
	// ■ その他
	//------------------------------------
	if (ImGui::CollapsingHeader("Other"))
	{
		ImGui::Checkbox("GroundDelete", &t.groundDelete);
		ImGui::Checkbox("PlayerAttackDrop", &t.playerAttackObjectDrop);
	}
}

void BossAttackDataSerializer::CopyParam(std::string _selectID)
{
	//========================
	// ■ コピー機能
	//========================
	auto& param = attackParam[_selectID];

	ImGui::Separator();
	ImGui::Text("Copy");

	static int copyIndex = 0;
	std::vector<std::string> keys;

	for (auto& a : attackParam) {
		keys.push_back(a.first);
	}

	std::sort(keys.begin(), keys.end());

	std::vector<const char*> items;
	for (auto& k : keys) {
		items.push_back(k.c_str());
	}

	ImGui::Combo("Source", &copyIndex, items.data(), (int)items.size());

	auto& src = attackParam[keys[copyIndex]];

	if (ImGui::Button("Copy ALL"))
	{
		param = src;
	}

	ImGui::SameLine();

	if (ImGui::Button("Copy Basic"))
	{
		param.hitDamage = src.hitDamage;
		param.slowTime = src.slowTime;
		param.slowAmout = src.slowAmout;
		param.speedUpMotionSpeed = src.speedUpMotionSpeed;

		param.attackCollsionStartTime = src.attackCollsionStartTime;
		param.attackCollsionEndTime = src.attackCollsionEndTime;

		param.justAvoidCollsionStartTime = src.justAvoidCollsionStartTime;
		param.justAvoidCollsionEndTime = src.justAvoidCollsionEndTime;

		param.attackPositionFrameNum = src.attackPositionFrameNum;
		param.attackSoundStartTime = src.attackSoundStartTime;

		param.attackCollTransform = src.attackCollTransform;
		param.justAvoidCollTransform = src.justAvoidCollTransform;

		param.useFlash = src.useFlash;
		param.attackFlashStartTime = src.attackFlashStartTime;
	}
	//========================
	// ■ イベント単位コピー
	//========================
	ImGui::Separator();
	ImGui::Text("Event Copy");

	// チェック状態
	static bool copyMove = false;
	static bool copyFollow = false;
	static bool copyRush = false;
	static bool copyRotate = false;
	static bool copyJump = false;
	static bool copyShock = false;
	static bool copyThrow = false;
	static bool copyCamera = false;
	static bool copyTrail = false;

	// チェックボックス
	ImGui::Checkbox("Move", &copyMove);
	ImGui::SameLine();
	ImGui::Checkbox("Follow", &copyFollow);
	ImGui::SameLine();
	ImGui::Checkbox("Rush", &copyRush);

	ImGui::Checkbox("Rotate", &copyRotate);
	ImGui::SameLine();
	ImGui::Checkbox("Jump", &copyJump);
	ImGui::SameLine();
	ImGui::Checkbox("Shock", &copyShock);

	ImGui::Checkbox("Throw", &copyThrow);
	ImGui::SameLine();
	ImGui::Checkbox("Camera", &copyCamera);
	ImGui::SameLine();
	ImGui::Checkbox("Trail", &copyTrail);

	// 一括ON/OFF
	if (ImGui::Button("All ON"))
	{
		copyMove = copyFollow = copyRush = copyRotate =
			copyJump = copyShock = copyThrow = copyCamera = copyTrail = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("All OFF"))
	{
		copyMove = copyFollow = copyRush = copyRotate =
			copyJump = copyShock = copyThrow = copyCamera = copyTrail = false;
	}

	//========================
	// ■ コピー実行
	//========================
	if (ImGui::Button("Copy Selected Events"))
	{
		//------------------------
		// Move
		//------------------------
		if (copyMove)
		{
			param.frontMove = src.frontMove;
			param.baseSpeed = src.baseSpeed;
			param.moveStartTime = src.moveStartTime;
			param.moveFinishTime = src.moveFinishTime;
			param.addVelocity = src.addVelocity;
		}

		//------------------------
		// Follow
		//------------------------
		if (copyFollow)
		{
			param.playerAloowMove = src.playerAloowMove;
			param.baseSpeed = src.baseSpeed;
			param.playerNearStop = src.playerNearStop;
			param.playerNearAloowStop = src.playerNearAloowStop;
			param.addVelocity = src.addVelocity;
			param.playerBaseNear = src.playerBaseNear;
			param.maxMoveSpeed = src.maxMoveSpeed;
			param.minMoveSpeed = src.minMoveSpeed;
			param.moveStartTime = src.moveStartTime;
			param.moveFinishTime = src.moveFinishTime;
		}

		//------------------------
		// Rush
		//------------------------
		if (copyRush)
		{
			param.rushMove = src.rushMove;
			param.rushAfterSpeed = src.rushAfterSpeed;
			param.rushTime = src.rushTime;
			param.rushColl = src.rushColl;
			param.addRushCollScale = src.addRushCollScale;
			param.rushSoundRightFoot = src.rushSoundRightFoot;
			param.rushSoundLeftFoot = src.rushSoundLeftFoot;
		}

		//------------------------
		// Rotate
		//------------------------
		if (copyRotate)
		{
			param.rotateMove = src.rotateMove;
			param.angleMoveAmout = src.angleMoveAmout;
		}

		//------------------------
		// Jump
		//------------------------
		if (copyJump)
		{
			param.jump = src.jump;
			param.jumpSpeed = src.jumpSpeed;
			param.addGravity = src.addGravity;
			param.jumpStartTime = src.jumpStartTime;
			param.groundEffectStartTime = src.groundEffectStartTime;
			param.groundShakeCamera = src.groundShakeCamera;
			param.groundShakeTime = src.groundShakeTime;
		}

		//------------------------
		// ShockWave
		//------------------------
		if (copyShock)
		{
			param.shockWave = src.shockWave;
			param.shockWaveSpeed = src.shockWaveSpeed;
			param.startRange = src.startRange;
		}

		//------------------------
		// Throw
		//------------------------
		if (copyThrow)
		{
			param.throwObject = src.throwObject;
			param.throwAttackData = src.throwAttackData;
			param.armThrow = src.armThrow;
			param.armFrameNum = src.armFrameNum;
			param.throwStartTime = src.throwStartTime;
			param.throwObjectApperaTime = src.throwObjectApperaTime;
			param.throwObjectNum = src.throwObjectNum;
			param.objectApperaPosition = src.objectApperaPosition;
			param.intervalTime = src.intervalTime;
			param.intervalTimeSub = src.intervalTimeSub;
			param.maxIntervalTime = src.maxIntervalTime;
			param.minIntervalTime = src.minIntervalTime;
		}

		//------------------------
		// Camera
		//------------------------
		if (copyCamera)
		{
			param.attackCameraBossLook = src.attackCameraBossLook;
			param.cameraChangeSpeed = src.cameraChangeSpeed;
		}

		//------------------------
		// Trail
		//------------------------
		if (copyTrail)
		{
			param.useTrail = src.useTrail;
			param.trailRightHand = src.trailRightHand;
		}
	}
}

void BossAttackDataSerializer::DrawRayColliderInfo(const char* label, BossAttackBase::RayColliderInfo& r)
{
	if (ImGui::TreeNode(label))
	{
		ImGui::DragFloat("StartPos", &r.rayStartPos, 0.1f);
		ImGui::DragFloat("FinishPos", &r.rayFinishPos, 0.1f);

		ImGui::TreePop();
	}
}

void BossAttackDataSerializer::DrawDountColliderInfo(const char* label, BossAttackBase::DountColliderInfo& d)
{
	if (ImGui::TreeNode(label))
	{
		ImGui::DragFloat("InnerRadius", &d.inRadius, 0.1f);
		ImGui::DragFloat("OuterRadius", &d.outRadius, 0.1f);

		ImGui::TreePop();
	}
}

void BossAttackDataSerializer::DrawAddThrowObjects(std::map<std::string, BossRockManager::BossThrowObjectData>& throwObjectsData)
{
	//------------------------------------
	// ■ 選択用
	//------------------------------------
	static int selectIndex = -1;
	static std::vector<std::string> keys;

	keys.clear();
	for (auto& [key, _] : throwObjectsData)
	{
		keys.push_back(key);
	}

	//------------------------------------
	// ■ Combo（選択）
	//------------------------------------
	if (!keys.empty())
	{
		std::vector<const char*> items;
		for (auto& k : keys) items.push_back(k.c_str());

		ImGui::Combo("ThrowObject Select", &selectIndex, items.data(), (int)items.size());
	}
	else
	{
		ImGui::Text("No Data");
	}

	static char newID[128] = "";

	ImGui::InputText("New ID", newID, sizeof(newID));
	ImGui::Checkbox("Effect", &isEffect);

	if (ImGui::Button("Add ThrowObject"))
	{
		std::string id = newID;

		if (!id.empty() && throwObjectsData.count(id) == 0)
		{
			BossRockManager::BossThrowObjectData data;

			data.id = id;
			data.modelName = id;
			data.isEffect = isEffect;

			throwObjectsData[id] = data;

			rockManager->AddJsonData(data);

			selectIndex = (int)throwObjectsData.size() - 1;

			newID[0] = '\0';
			throwObjectsData = rockManager->GetThrowObjectsData();
		}
	}

	//------------------------------------
	// ■ 編集
	//------------------------------------
	if (selectIndex < 0 || selectIndex >= keys.size())
		return;

	std::string key = keys[selectIndex];
	auto t = throwObjectsData[key];

	ImGui::Separator();
	ImGui::Text("Edit : %s", key.c_str());

	char idBuf[128];
	strcpy_s(idBuf, t.id.c_str());

	if (ImGui::InputText("ID##Edit", idBuf, sizeof(idBuf)))
	{
		std::string newKey = idBuf;

		if (newKey != key && throwObjectsData.count(newKey) == 0)
		{
			auto node = throwObjectsData.extract(key);
			node.key() = newKey;
			throwObjectsData.insert(std::move(node));
			//rockManager->ChangeJsonData();
			selectIndex = -1;
			return;
		}

		t.id = newKey;
		t.modelName = newKey;
	}

	//------------------------------------
	// ■ モデル
	//------------------------------------
	char modelBuf[128];
	strcpy_s(modelBuf, t.modelName.c_str());

	if (ImGui::InputText("ModelName", modelBuf, sizeof(modelBuf)))
	{
		t.modelName = modelBuf;
	}

	ImGui::DragInt("ModelData", &t.modelData, 1);

	//------------------------------------
	// ■ Transform
	//------------------------------------
	DrawTransform("ModelTransform", t.modelTransform);
	DrawTransform("PushTransform", t.pushTransform);

	//------------------------------------
	// ■ その他
	//------------------------------------
	ImGui::Checkbox("IsEffect", &t.isEffect);
}