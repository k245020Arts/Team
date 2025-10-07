#include "BootScene.h"
#include "inputManager.h"
#include "LoadManager.h"
#include "CollsionManager.h"
#include "SoundManager.h"
#include "Debug.h"
#include "TransitorManager.h"

BootScene::BootScene()
{
	InputManager* input = new InputManager(true,true,false);
	Load::Init();
	ID::Init();
	Debug::InitDebug(input);
	SoundManager*sound =  new SoundManager();
	TransitorManager* transitor = new TransitorManager();
	new CollsionManager();
}

BootScene::~BootScene()
{
}

void BootScene::Update()
{
	SceneManager::ChangeScene("TITLE"); // ‹N“®‚ªI‚í‚Á‚½‚çTitle‚ğ•\¦
}

void BootScene::Draw()
{
}
