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
	/// <summary>
	/// ステージの生成
	/// </summary>
	void CreateStage();
	/// <summary>
	/// ステージの壁の生成
	/// </summary>
	void CreateWall();
	/// <summary>
	/// ステージの床の生成
	/// </summary>
	void CreateFloor();

private:
	
	std::vector<Object3D*> maps;
	Object3D* stage;
	int modelHandle;
};
