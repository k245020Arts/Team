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
	abutton->Init(Transform(VECTOR3(1550, 800, 0), VZero, VOne), "AButton");
	ButtonUI* a = abutton->Component()->AddComponent<ButtonUI>();
	a->Start(ButtonUI::A_BUTTON, LoadGraph("data/image/Mark_Avoid.png"));

}
