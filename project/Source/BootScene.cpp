#include "BootScene.h"
#include "../Source/Common/InputManager/InputManager.h"
#include "../Source/Common/LoadManager.h"
#include "../Source/Component/Collider/CollsionManager.h"
#include "../Source/Common/Sound/SoundManager.h"
#include "../Source/Common/Debug/Debug.h"
#include "../Source/Common/Transitor/TransitorManager.h"
#include "../Source/Common/Transitor/FadeTransitor.h"
#include "Common/Random.h"

BootScene::BootScene()
{
	InputManager* input			= new InputManager(true,true,false);
	Load::Init();
	ID::Init();
	Sound_ID::InitID();
	Debug::InitDebug(input);
	Random::Init();
	SoundManager*sound			=  new SoundManager();
	TransitorManager* transitor = new TransitorManager();
	new CollsionManager();
	one = 0;
}

BootScene::~BootScene()
{
}

void BootScene::Update()
{
	if (one++ >= 1) {
		FindGameObject<FadeTransitor>()->StartTransitor("PLAY", 1.0f);
		SceneManager::ChangeScene("TITLE"); // ‹N“®‚ªI‚í‚Á‚½‚çTitle‚ğ•\¦
	}
	
}

void BootScene::Draw()
{
}
