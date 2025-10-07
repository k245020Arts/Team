#pragma once
#include "../Library/GameObject.h"
#include "vector"

class Object3D;

class StageManager : public GameObject
{
public:
	StageManager();
	~StageManager();
	void Update() override;
	void Draw() override;
	void CreateStage();
	void CreateWall();

private:
	
	std::vector<Object3D*> maps;
	Object3D* stage;
	int modelHandle;
};
