#include "PlayScene.h"
#include "Object3D.h"
#include "ComponentManager.h"
#include "MeshRenderer.h"
#include "transform.h"
#include "MeshRenderer2D.h"
#include "player.h"
#include "Physics.h"
#include "camera.h"
#include "Hierarchy.h"
#include "stageManager.h"
#include "playerManager.h"
#include "EnemyManager.h"
#include "LoadManager.h"
#include "CollsionManager.h"
#include "Debug.h"
#include "weaponManager.h"
#include "light.h"
#include "cameraManager.h"
#include "EffectManager.h"
#include "SoundManager.h"
#include "skyManager.h"
#include "BlurScreen.h"
#include "FadeTransitor.h"
#include "PushTransitor.h"
#include "WipeTransitor.h"
#include "StripTransitor.h"
#include "TitleTransitor.h"
#include "IrisTransitor.h"
#include "GameManager.h"
#include "BossManager.h"
//#include "YAMLReader.h"

int PlayScene::count = 0;

//YAML_CONVERT_STRUCT(P, name, level, items)

PlayScene::PlayScene()
{
	//Debug::DebugLog(std::to_string(GetTrackedMemoryUsage()));
	Load::LoadModel(Load::MODEL_PATH + "RPG-Character", ID::P_MODEL);
	Hierachy* hierachy = new Hierachy();
	//Debug::DebugLog(std::to_string(GetTrackedMemoryUsage()));
	new EffectManager();
	//Debug::DebugLog(std::to_string(GetMemory()));
	
	BlurScreen* blur =  new BlurScreen();
	//Debug::DebugLog(std::to_string(GetTrackedMemoryUsage()));
	StageManager* stageManager = new StageManager();
	//Debug::DebugLog(std::to_string(GetTrackedMemoryUsage()));
	
	EnemyManager* enemy =  new EnemyManager();
	//Debug::DebugLog(std::to_string(GetTrackedMemoryUsage()));
	PlayerManager* player = new PlayerManager();
//	Debug::DebugLog(std::to_string(GetTrackedMemoryUsage()));
	WeaponManager* weapon = new WeaponManager();
	//Debug::DebugLog(std::to_string(GetTrackedMemoryUsage()));
	CollsionManager* collManager =  new CollsionManager();
	//Debug::DebugLog(std::to_string(GetTrackedMemoryUsage()));
	Light* light = new Light();
	//Debug::DebugLog(std::to_string(GetTrackedMemoryUsage()));
	CameraManager* cameraManager = new CameraManager();
	//Debug::DebugLog(std::to_string(GetTrackedMemoryUsage()));
	SkyManager* sky = new SkyManager();
	//Debug::DebugLog(std::to_string(GetTrackedMemoryUsage()));
	GameManager* gameManager = new GameManager();

	BossManager* bossManager = new BossManager();
	
	stageManager->CreateStage();
	enemy->CreateEnemy();
	cameraManager->CreateCamera();
	player->CreatePlayer();
	enemy->PlayerObjPointer();
	cameraManager->ParentObj("PLAYER");
	enemy->CreateBoss();
	weapon->CreatePlayerWeapon();
	weapon->CreateEnemyWeapon();
	collManager->SetDrawOrder(-90000);
	hierachy->SetDrawOrder(-100000);
	sky->CreateSky();
	gameManager->CreateNum();
	

	//YamlReader ya;
	//std::vector<P> pl;
	//ya.SaveVector<P>("ala", pl);
	FindGameObject<SoundManager>()->PlaySceneLoad();

	blur->SetDrawOrder(-500);

	//m->TextureHandle(LoadGraph("data/texture/101backGround.png"),MeshRenderer2D::DRAW_RECT_ROTA_GRAPH,VECTOR3(500,500,500),VZero,VZero);
}

PlayScene::~PlayScene()
{
	Load::AllDelete();
	Debug::ClearLogger();
}

void PlayScene::Update()
{
	if (CheckHitKey(KEY_INPUT_T)) {
		FindGameObject<FadeTransitor>()->StartTransitor("TITLE", 0.05f);
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
	
}

void PlayScene::Draw()
{
	/*DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));*/
}
