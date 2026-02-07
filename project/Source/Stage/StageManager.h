#pragma once
#include "../../Library/GameObject.h"
#include "vector"

class Object3D;

static const float WALL_EDGE_POS = 13000.0f;

class StageManager : public GameObject
{
public:
	StageManager();
	~StageManager();
	void Update() override;
	void Draw() override;
	void CreateStage();
	void CreateWall();
	void CreateFloor();

private:
	
	std::vector<Object3D*> maps;
	Object3D* stage;
	int modelHandle;
};
