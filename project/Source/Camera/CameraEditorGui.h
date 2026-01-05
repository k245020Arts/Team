#pragma once
#include "Camera.h"

class CameraEditorGui
{
public:
	CameraEditorGui();
	CameraEditorGui(Camera* _camera);
	~CameraEditorGui();

	void EditorWindow();

private:
	Camera* camera;
	std::string fileName;
	VECTOR3 basePosition;
	VECTOR3 offset;
	std::string posName;
	std::string targetName;
	int cutNum;
	std::string cutSceneName;

	const std::string PLAYER_POS_NAME = "PlayerPos";
	const std::string ENEMY_POS_NAME = "EnemyPos";
	const std::string WORLD_POS_NAME = "WorldPos";

	const std::string PLAYER_TARGET_NAME = "PlayerTarget";
	const std::string ENEMY_TARGET_NAME = "EnemyTarget";
	const std::string WORLD_TARGET_NAME = "WorldTARGET";
};