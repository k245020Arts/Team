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
	ImGui::Begin("editor_mode");
	ImGui::DragFloat3("cameraPosition", &camera->cameraComponent.cameraTransform->position.x, 1.0f, 0.0f, 5000.0f);
	ImGui::DragFloat3("cameraRotation", &camera->cameraComponent.cameraTransform->rotation.x, 1.0f, 0.0f, 5000.0f);
	ImGui::DragFloat3("baseTarget", &baseTarget.x, 1.0f, 0.0f, 5000.0f);
	ImGui::DragInt("cutNum", &cutNum, 1, 0, 20);
	if (ImGui::Button("player_position")) {
		//camera->cameraComponent.cameraTransform->position = camera->cameraComponent.player.transform->position;
		basePosition = camera->cameraComponent.player.transform->position;
		cutScene.followPosName = PLAYER_POS_NAME;
		camera->target = camera->cameraComponent.cameraTransform->position + camera->cameraComponent.player.transform->Forward() * 2000.0f;
	}
	if (ImGui::Button("enemy_transform")) {
		basePosition = camera->cameraComponent.target.transform->position;
		cutScene.followPosName = ENEMY_POS_NAME;
		camera->target = camera->cameraComponent.cameraTransform->position + camera->cameraComponent.target.transform->Forward() * 2000.0f;
	}
	if (ImGui::Button("world_position")) {
		cutScene.followPosName = WORLD_POS_NAME;
	}
	ImGui::DragFloat3("offset", &offset.x, 1.0f, 0.0f, 10000.0f);
	ImGui::DragFloat3("startPos", &cutScene.camera.startPos.x, 1.0f, 0.0f, 10000.0f);
	ImGui::DragFloat3("endPos", &cutScene.camera.endPos.x, 1.0f, 0.0f, 10000.0f);
	ImGui::DragFloat3("target", &cutScene.camera.target.x, 1.0f, 0.0f, 10000.0f);
	if (ImGui::Button("player_target")) {
		baseTarget = camera->cameraComponent.player.transform->position;
		cutScene.followPosTarget = PLAYER_POS_NAME;
	}
	if (ImGui::Button("enemy_target")) {
		baseTarget = camera->cameraComponent.target.transform->position;
		cutScene.followPosTarget = ENEMY_POS_NAME;
	}
	if (ImGui::Button("world_target")) {
		cutScene.followPosTarget = WORLD_POS_NAME;
	}
	//ToDo : Imguiでカットシーンの名前を入力できるようにする。
	camera->cameraComponent.cameraTransform->position = basePosition + offset;
	camera->target = baseTarget + cutScene.camera.target;
	ImGui::DragFloat("MaxTime", &cutScene.duration, 0.1, 0.0f, 5.0f);
	
	if (ImGui::Button("now_save")) {

		JsonReader json;
		json.Load("data/json/test.json");

		nlohmann::json& root = json.Data();

		if (!root.contains("cutScenes")) {
			root["cutScenes"] = nlohmann::json::object();
		}

		std::string key = "cutScene" + std::to_string(cutNum);
		root["cutScenes"][key] = cutScene; 

		json.Save("data/json/test.json", root);
	}

	
	ImGui::End();
}