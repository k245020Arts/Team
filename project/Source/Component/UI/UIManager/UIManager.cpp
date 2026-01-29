#include "UIManager.h"
#include "../ButtonUI.h"
#include "../../Object/BaseObject.h"
#include "../../Transform/Transform.h"
#include "../../ComponentManager.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

void UIManager::Update()
{
}

void UIManager::Draw()
{
}

void UIManager::ButtonUISet()
{
	Object2D* abutton = new Object2D();
	abutton->Init(Transform(VECTOR3(1650, 1000, 0), VZero, VOne), "AButton");
	ButtonUI* a = abutton->Component()->AddComponent<ButtonUI>();
	a->Start(ButtonUI::A_BUTTON, LoadGraph("data/image/Mark_Avoid2.png"));
	abutton->SetDrawOrder(-1000);

	Object2D* bbutton = new Object2D();
	bbutton->Init(Transform(VECTOR3(1750, 900, 0), VZero, VOne), "BButton");
	ButtonUI* b = bbutton->Component()->AddComponent<ButtonUI>();
	b->Start(ButtonUI::B_BUTTON, LoadGraph("data/image/Mark_Normal.png"));
	bbutton->SetDrawOrder(-1000);

	Object2D* xbutton = new Object2D();
	xbutton->Init(Transform(VECTOR3(1550, 900, 0), VZero, VOne), "XButton");
	ButtonUI* c = xbutton->Component()->AddComponent<ButtonUI>();
	c->Start(ButtonUI::X_BUTTON, LoadGraph("data/image/Mark_Special.png"));
	xbutton->SetDrawOrder(-1000);
}
