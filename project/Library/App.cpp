#include "App.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Time.h"
#include <DxLib.h>
#include "../Source/Common/LoadManager.h"

void AppInit()
{
	ObjectManager::Init();
	SceneManager::Init();
	Time::Init();
}

void AppUpdate()
{
	SceneManager::Update();
	ObjectManager::Update();
}

void AppDraw()
{
	Time::Refresh();
	SceneManager::Draw();
	ObjectManager::Draw();
}

void AppRelease()
{
	Time::Release();
	SceneManager::Release();
	ObjectManager::Release();
	Load::AllDelete();
}

bool AppIsExit()
{
	return SceneManager::IsExit();
}