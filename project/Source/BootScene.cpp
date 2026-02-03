#include "BootScene.h"
#include "../Source/Common/InputManager/InputManager.h"
#include "../Source/Common/LoadManager.h"
#include "../Source/Component/Collider/CollsionManager.h"
#include "../Source/Common/Sound/SoundManager.h"
#include "../Source/Common/Debug/Debug.h"
#include "../Source/Common/Transitor/TransitorManager.h"
#include "../Source/Common/Transitor/FadeTransitor.h"
#include "Common/Random.h"
#include "../Source/Common/Fead/Fead.h"

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
	new Fead();
	//Load::SetAsync(false);
	Load::LoadModel(Load::MODEL_PATH + "stage_s", ID::S_MODEL, true);
	Load::LoadModel(Load::MODEL_PATH + "cube", ID::WALL, true);
}

BootScene::~BootScene()
{
}

void BootScene::Update()
{
	
	FindGameObject<FadeTransitor>()->StartTransitor("TITLE", 1.0f);
	SceneManager::ChangeScene("TITLE"); // ãNìÆÇ™èIÇÌÇ¡ÇΩÇÁTitleÇï\é¶
	
}

void BootScene::Draw()
{
}
