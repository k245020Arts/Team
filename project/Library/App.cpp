#include "App.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Time.h"
#include <DxLib.h>
#include "../Source/Common/LoadManager.h"
#include "../Source/Common/Singleton/SingleTonUpdater.h"

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
	SingleTonUpdater::Update();
}

void AppDraw()
{
	Time::Refresh();
	SceneManager::Draw();
	ObjectManager::Draw(); 
	SingleTonUpdater::Draw();
}

void AppRelease()
{
	// DontDestroyフラグを全解除してから終了
	/*for (GameObject* obj : ObjectManager::GetAllObject()) {
		obj->DontDestroyOnSceneChange(false);
	}*/

	Time::Release();
	SceneManager::Release();
	ObjectManager::Release();
	SingleTonUpdater::Release();
	Load::AllDelete();

}

bool AppIsExit()
{
	return SceneManager::IsExit();
}