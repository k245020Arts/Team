#include "UIManager.h"
#include "../ButtonUI.h"
#include "../../Object/BaseObject.h"
#include "../../Transform/Transform.h"
#include "../../ComponentManager.h"
#include "../../../Common/LoadManager.h"

UIManager::UIManager()
{
	SetDrawOrder(-20000);
	Load::LoadImageGraph(Load::IMAGE_PATH + "Mark_Avoid2", ID::A_BUTTON);
	Load::LoadImageGraph(Load::IMAGE_PATH + "Mark_Normal", ID::B_BUTTON);
	Load::LoadImageGraph(Load::IMAGE_PATH + "Mark_Special", ID::X_BUTTON);
	Load::LoadImageGraph(Load::IMAGE_PATH + "Number_01", ID::DAMAGE_UI_BUTTON);
}

UIManager::~UIManager()
{
	uiObjects.clear();
}

void UIManager::Update()
{
}

void UIManager::Draw()
{
	if (!draw) {
		return;
	}
	for (auto itr = uiObjects.begin(); itr != uiObjects.end(); itr++) {
		(*itr)->SetDraw(true);
		(*itr)->Draw();
		(*itr)->SetDraw(false);
	}
}

void UIManager::ButtonUISet()
{
	Object2D* abutton = new Object2D();
	abutton->Init(Transform(VECTOR3(1650, 1000, 0), VZero, VOne), "AButton");
	ButtonUI* a = abutton->Component()->AddComponent<ButtonUI>();
	a->Start(ButtonUI::A_BUTTON, Load::GetHandle(ID::A_BUTTON));
	abutton->SetDrawOrder(-1000);

	Object2D* bbutton = new Object2D();
	bbutton->Init(Transform(VECTOR3(1750, 900, 0), VZero, VOne), "BButton");
	ButtonUI* b = bbutton->Component()->AddComponent<ButtonUI>();
	b->Start(ButtonUI::B_BUTTON, Load::GetHandle(ID::B_BUTTON));
	bbutton->SetDrawOrder(-1000);

	Object2D* xbutton = new Object2D();
	xbutton->Init(Transform(VECTOR3(1550, 900, 0), VZero, VOne), "XButton");
	ButtonUI* c = xbutton->Component()->AddComponent<ButtonUI>();
	c->Start(ButtonUI::X_BUTTON, Load::GetHandle(ID::X_BUTTON));
	xbutton->SetDrawOrder(-1000);
}

void UIManager::UIPush(BaseObject* _base)
{
	uiObjects.emplace_back(_base);
}

void UIManager::UIDelete(BaseObject* _base)
{
	for (auto itr = uiObjects.begin(); itr != uiObjects.end(); itr++) {
		if (*itr == _base) {
			uiObjects.erase(itr);
			return;
		}
	}
}
