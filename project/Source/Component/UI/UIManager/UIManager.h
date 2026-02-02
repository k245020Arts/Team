#pragma once
#include "../../../../Library/GameObject.h"

class BaseObject;

class UIManager : public GameObject
{
public:
	UIManager();
	~UIManager();

	void Update()override;
	void Draw()override;
	void ButtonUISet();

	void UIPush(BaseObject* _base);
	void UIDelete(BaseObject* _base);

	void SetUIDraw(bool _draw) { draw = _draw; }

private:
	std::list<BaseObject*> uiObjects;
	bool draw;
};
