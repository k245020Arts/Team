#pragma once
#include "../../Library/GameObject.h"

class Object3D;
class GameManager;
class StateManager;

class PlayerManager : public GameObject
{
public:
	PlayerManager();
	~PlayerManager();
	void Update() override;
	void Draw() override;
	/// <summary>
	/// プレイシーンのプレイヤーの生成
	/// </summary>
	void CreatePlayer();

	
	/// <summary>
	/// タイトルシーンのプレイヤーの生成
	/// </summary>
	void CreateTitlePlayer();

private:

	Object3D* playerPointer;

	GameManager* gameManager;
	StateManager* stateManager;
	void GameSceneChangeState();
};
