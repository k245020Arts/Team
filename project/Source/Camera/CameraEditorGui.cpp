#include "CameraEditorGui.h"
#include "../../ImGui/imgui.h"
#include "../Common/JsonReader.h"

CameraEditorGui::CameraEditorGui() : CameraEditorGui(nullptr)
{
	
}

CameraEditorGui::CameraEditorGui(Camera* _camera)
{
	camera = _camera;
	cutSceneName = "";
	basePosition = VZero;
	offset = VZero;
	cutNum = 0;
}

CameraEditorGui::~CameraEditorGui()
{
}

void CameraEditorGui::EditorWindow()
{
    ImGui::Begin("CutScene Editor");

    // =============================
    // File & Slot Info
    // =============================
    ImGui::Text("=== CutScene File ===");
    ImGui::InputText("File Name", cutSceneFileName, IM_ARRAYSIZE(cutSceneFileName));

    JsonReader checkjson;
    std::string filePath = std::string("data/json/") + cutSceneFileName + ".json";
    checkjson.Load(filePath);
    nlohmann::json& root = checkjson.Data();

    bool usingCutScene = IsUsingCutScene(cutNum, root);

    ImGui::DragInt("Cut Number", &cutNum, 1, 0, 20);
    ImGui::SameLine();

    if (usingCutScene) {
        ImGui::TextColored(ImVec4(1, 0.3f, 0.3f, 1), "[USING]");
    }
    else {
        ImGui::TextColored(ImVec4(0.3f, 1, 0.3f, 1), "[EMPTY]");
    }

    ImGui::Separator();

    // =============================
    // Camera Transform
    // =============================
    if (ImGui::TreeNode("Camera Transform"))
    {
        ImGui::DragFloat3("Position", &camera->cameraComponent.cameraTransform->position.x, 1.0f);
        ImGui::DragFloat3("Rotation", &camera->cameraComponent.cameraTransform->rotation.x, 1.0f);
        ImGui::TreePop();
    }

    // =============================
    // Follow Position
    // =============================
    if (ImGui::TreeNode("Follow Position"))
    {
        ImGui::DragFloat3("Base Position", &basePosition.x, 1.0f);
        ImGui::DragFloat3("Offset", &offset.x, 1.0f);

        if (ImGui::Button("Player")) {
            basePosition = camera->cameraComponent.player.transform->position;
            cutScene.followPosName = PLAYER_POS_NAME;
        }
        ImGui::SameLine();
        if (ImGui::Button("Enemy")) {
            basePosition = camera->cameraComponent.target.transform->position;
            cutScene.followPosName = ENEMY_POS_NAME;
        }
        ImGui::SameLine();
        if (ImGui::Button("World")) {
            cutScene.followPosName = WORLD_POS_NAME;
        }

        ImGui::Text("Current : %s", cutScene.followPosName.c_str());
        ImGui::TreePop();
    }

    // =============================
    // Target
    // =============================
    if (ImGui::TreeNode("Target"))
    {
        ImGui::DragFloat3("Base Target", &baseTarget.x, 1.0f);
        ImGui::DragFloat3("Target Offset", &cutScene.camera.target.x, 1.0f);

        if (ImGui::Button("Player")) {
            baseTarget = camera->cameraComponent.player.transform->position;
            cutScene.followPosTarget = PLAYER_POS_NAME;
        }
        ImGui::SameLine();
        if (ImGui::Button("Enemy")) {
            baseTarget = camera->cameraComponent.target.transform->position;
            cutScene.followPosTarget = ENEMY_POS_NAME;
        }
        ImGui::SameLine();
        if (ImGui::Button("World")) {
            cutScene.followPosTarget = WORLD_POS_NAME;
        }

        ImGui::Text("Current : %s", cutScene.followPosTarget.c_str());
        ImGui::TreePop();
    }

    // =============================
    // CutScene Params
    // =============================
    if (ImGui::TreeNode("CutScene Parameters"))
    {
        ImGui::DragFloat3("Start Pos", &cutScene.camera.startPos.x, 1.0f);
        ImGui::SameLine();
        if (ImGui::Button("Set Start")) {
            cutScene.camera.startPos = offset;
        }

        ImGui::DragFloat3("End Pos", &cutScene.camera.endPos.x, 1.0f);
        ImGui::SameLine();
        if (ImGui::Button("Set End")) {
            cutScene.camera.endPos = offset;
        }

        ImGui::DragFloat("Duration", &cutScene.duration, 0.1f, 0.0f, 5.0f);

        ImGui::TreePop();
    }

    // =============================
    // Runtime Preview Update
    // =============================
    if (!camera->IsCutScene())
    {
        camera->cameraComponent.cameraTransform->position = basePosition + offset;
        camera->target = baseTarget + cutScene.camera.target;
    }

    ImGui::Separator();

    // =============================
    // Save / Preview
    // =============================
    if (ImGui::Button("Save")) {
        if (usingCutScene) {
            ImGui::OpenPopup("Save Confirm");
        }
        else {
            DataSave();
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Preview")) {
        std::string file = cutSceneFileName + std::string(".json");
        camera->CutSceneChangeState(file);
    }

    SavePopUp(usingCutScene);

    ImGui::End();
}

bool CameraEditorGui::IsUsingCutScene(int cutNum, const nlohmann::json& root)
{
	if (!root.contains("cutScenes")) {
		return false;
	}

	std::string key = "cutScene" + std::to_string(cutNum);
	return root["cutScenes"].contains(key);
}

void CameraEditorGui::SavePopUp(bool _using)
{
	if (ImGui::BeginPopupModal("Save Confirm", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("CutScene %d をセーブしますか？", cutNum);
		
		if (_using) {
			ImGui::Spacing();
			ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "usingCutScene!!!!");
		}

		ImGui::Separator();

		if (ImGui::Button("Yes", ImVec2(120, 0))) {

			DataSave();
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("No", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void CameraEditorGui::DataSave()
{
	std::string filePath = std::string("data/json/") + cutSceneFileName + ".json";

	JsonReader json;
	json.Load(filePath);

	nlohmann::json& root = json.Data();

	if (!root.contains("cutScenes")) {
		root["cutScenes"] = nlohmann::json::object();
	}

	std::string key = "cutScene" + std::to_string(cutNum);
	root["cutScenes"][key] = cutScene;

	json.Save(filePath, root);
}
