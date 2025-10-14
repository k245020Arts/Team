#include "Boss.h"
#include "Animator.h"
#include "Physics.h"
#include "controllerInputManager.h"
#include "sphereCollider.h"
#include "color.h"
#include "Shaker.h"
#include "../ImGui/imgui.h"
#include "player.h"
#include "EnemyBlowAway.h"
#include "cameraManager.h"
#include "camera.h"
#include "EffectManager.h"
#include "SoundManager.h"
#include "weaponManager.h"
#include "stateManager.h"
#include "GameManager.h"
#include "BossIdol.h"

Boss::Boss()
{
	debugId = 19;
	tag = Function::GetClassNameC<Boss>();
}

Boss::~Boss()
{
}

void Boss::Update()
{
}

void Boss::Draw()
{
}

void Boss::Start(Object3D* _obj) 
{
	enemyBaseComponent.state = obj->Component()->AddComponent<StateManager>();



	bossTransform = obj->GetTransform();

	//enemyBaseComponent.boss = this;
	enemyBaseComponent.anim = obj->Component()->GetComponent<Animator>();
	enemyBaseComponent.physics = obj->Component()->GetComponent<Physics>();
	enemyBaseComponent.control = FindGameObject<ControllerInputManager>();
	enemyBaseComponent.color = obj->Component()->GetComponent<Color>();
	enemyBaseComponent.color->setRGB(Color::Rgb(0.0f, 0.0f, 0.0f, 255.0f));
	enemyBaseComponent.shaker = obj->Component()->GetComponent<Shaker>();
	enemyBaseComponent.effect = FindGameObject<EffectManager>();
	enemyBaseComponent.sound = FindGameObject<SoundManager>();

	enemyBaseComponent.camera = FindGameObject<CameraManager>()->GetCamera()->Component()->GetComponent<Camera>();
	//enemyBaseComponent.weapon = FindGameObject<WeaponManager>();

	attackColl = nullptr;
	collName = "b_attack";

	enemyBaseComponent.gameManager = FindGameObject<GameManager>();

	using namespace ID;


	enemyBaseComponent.state->CreateState<BossIdol>(GetID(B_IDOL));
	/*eCom.state->NodeDrawReady();*/

	enemyBaseComponent.state->SetComponent<Boss>(this);

	enemyBaseComponent.state->StartState(B_IDOL);
}
