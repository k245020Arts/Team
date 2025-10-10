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

Boss::Boss()
{
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
	bossComponent.state = obj->Component()->AddComponent<StateManager>();



	bossTransform = obj->GetTransform();

	bossComponent.boss = this;
	bossComponent.anim = obj->Component()->GetComponent<Animator>();
	bossComponent.physics = obj->Component()->GetComponent<Physics>();
	bossComponent.control = FindGameObject<ControllerInputManager>();
	bossComponent.color = obj->Component()->GetComponent<Color>();
	bossComponent.color->setRGB(Color::Rgb(0.0f, 0.0f, 0.0f, 255.0f));
	bossComponent.shaker = obj->Component()->GetComponent<Shaker>();
	bossComponent.effect = FindGameObject<EffectManager>();
	bossComponent.sound = FindGameObject<SoundManager>();

	bossComponent.camera = FindGameObject<CameraManager>()->GetCamera()->Component()->GetComponent<Camera>();
	//bossComponent.weapon = FindGameObject<WeaponManager>();

	attackColl = nullptr;
	collName = "b_attack";

	bossComponent.gameManager = FindGameObject<GameManager>();

	using namespace ID;



	/*eCom.state->NodeDrawReady();*/

	bossComponent.state->SetComponent<Boss>(this);

	bossComponent.state->StartState(E_ANIM_IDOL);
}
