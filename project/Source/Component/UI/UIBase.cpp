#include "UIBase.h"
#include "UIManager/UIManager.h"

UIBase::UIBase()
{
	uiManager = FindGameObject<UIManager>();
}

UIBase::~UIBase()
{
	uiManager->UIDelete(obj);
}

void UIBase::Awake()
{
	uiManager->UIPush(obj);
	obj->SetDraw(false);
}
