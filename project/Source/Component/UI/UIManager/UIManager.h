#pragma once
#include "../../../../Library/GameObject.h"

class UIManager : public GameObject
{
public:
	UIManager();
	~UIManager();

	void Update()override;
	void Draw()override;
	void ButtonUISet();

private:
	
};
