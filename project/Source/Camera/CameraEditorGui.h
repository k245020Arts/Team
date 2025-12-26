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
};