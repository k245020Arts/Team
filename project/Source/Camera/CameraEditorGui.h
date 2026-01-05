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
	VECTOR3 baseTarget;
	int cutNum;
	std::string cutSceneName;

	CutSceneSpece::CutScene cutScene;

	
};