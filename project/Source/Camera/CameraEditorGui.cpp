#include "CameraEditorGui.h"
#include "../../ImGui/imgui.h"
#include "../Common/JsonReader.h"

namespace ne {
	void to_json(VECTOR3& j, JSON& _json) {
		_json = JSON{ {"x",j.x}, {"y",j.y}, {"z",j.z} };
	}

	void from_json(VECTOR3& j, JSON& json) {
		json.at("x").get_to(j.x);
		json.at("y").get_to(j.y);
		json.at("z").get_to(j.z);

	}
}


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
	ImGui::DragFloat3("target", &camera->target.x, 1.0f, 0.0f, 5000.0f);
	ImGui::DragInt("cutNum", &cutNum, 1, 0, 20);
	if (ImGui::Button("player_position")) {
		//camera->cameraComponent.cameraTransform->position = camera->cameraComponent.player.transform->position;
		basePosition = camera->cameraComponent.player.transform->position;
		posName = PLAYER_POS_NAME;
		camera->target = camera->cameraComponent.cameraTransform->position + camera->cameraComponent.player.transform->Forward() * 2000.0f;
	}
	if (ImGui::Button("enemy_transform")) {
		basePosition = camera->cameraComponent.target.transform->position;
		posName = ENEMY_POS_NAME;
		camera->target = camera->cameraComponent.cameraTransform->position + camera->cameraComponent.target.transform->Forward() * 2000.0f;
	}
	if (ImGui::Button("world_position")) {
		posName = WORLD_POS_NAME;
	}
	ImGui::DragFloat3("offset", &offset.x, 1.0f, 0.0f, 10000.0f);
	if (ImGui::Button("player_target")) {
		camera->target = camera->cameraComponent.player.transform->position;
		targetName = PLAYER_TARGET_NAME;
	}
	if (ImGui::Button("enemy_target")) {
		camera->target = camera->cameraComponent.target.transform->position;
		targetName = ENEMY_TARGET_NAME;
	}
	if (ImGui::Button("world_target")) {
		posName = WORLD_TARGET_NAME;
	}
	//ToDo : Imguiでカットシーンの名前を入力できるようにする。
	camera->cameraComponent.cameraTransform->position = basePosition + offset;
	
	
	if (ImGui::Button("now_save")) {
		JsonReader json;
		JSON j;
		//ToDo : Vector3をJsonDataに変換
		ne::from_json(basePosition, j);
		ne::from_json(camera->target, j);
		ne::from_json(offset, j);
		json.Save("data/json/test.json", j);;
	}

	
	ImGui::End();
}