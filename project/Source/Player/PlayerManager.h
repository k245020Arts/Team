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
	void CreatePlayer();

	void GameSceneChangeState();

private:

	Object3D* playerPointer;

	GameManager* gameManager;
	StateManager* stateManager;

};
