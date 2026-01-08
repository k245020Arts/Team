#pragma once
#include "Camera.h"

class CameraEditorGui
{
public:
	CameraEditorGui();
	CameraEditorGui(Camera* _camera);
	~CameraEditorGui();

	void EditorWindow();

	bool IsUsingCutScene(int cutNum, const nlohmann::json& root);

	void SavePopUp(bool _using);

	void DataSave();

	void LoadCutScene();

private:
	Camera* camera;
	std::string fileName;
	VECTOR3 basePosition;
	VECTOR3 offset;
	VECTOR3 baseTarget;
	int cutNum;
	std::string cutSceneName;

	CutSceneSpece::CutScene cutScene;

	char cutSceneFileName[128];

	int  currentLoadedCut;
	bool isLoaded;
	std::string lastFileName;
	
};