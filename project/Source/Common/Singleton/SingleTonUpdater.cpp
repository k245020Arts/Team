#include "SingleTonUpdater.h"
#include "../Sound/SoundManager.h"
#include "../Effect/EffectManager.h"
#include "../InputManager/InputManager.h"

SingleTonUpdater::SingleTonUpdater()
{

}

SingleTonUpdater::~SingleTonUpdater()
{
}

void SingleTonUpdater::Update()
{
	SoundManager::GetInstance()->Update();
	EffectManager::GetInstance()->Update();
	InputManager::GetInstance()->Update();
}

void SingleTonUpdater::Draw()
{
	SoundManager::GetInstance()->Draw();
	EffectManager::GetInstance()->Draw();
}

void SingleTonUpdater::Release()
{
	SoundManager::GetInstance()->Destroy();
	EffectManager::GetInstance()->Destroy();
	InputManager::GetInstance()->Destroy();
}