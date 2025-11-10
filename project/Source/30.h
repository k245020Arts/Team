#pragma once
#include "../Library/GameObject.h"

class Object3D;

class PlayerManager : public GameObject
{
public:
	PlayerManager();
	~PlayerManager();
	void Update() override;
	void Draw() override;
	void CreatePlayer();

private:

	Object3D* playerPointer;

};
