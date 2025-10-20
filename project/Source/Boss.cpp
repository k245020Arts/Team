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
#include "BossRun.h"
#include "BossNormalAttack1.h"
#include "BossNormalAttack2.h"
#include "BossNormalAttack3.h"
#include "BossSpecialAttack1.h"

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
	EnemyBase::Update();
}

void Boss::Draw()
{
}

void Boss::Start(Object3D* _obj) 
{
	enemyBaseComponent.state = obj->Component()->AddComponent<StateManager>();
	enemyBaseComponent.playerObj = FindGameObjectWithTag<Object3D>("PLAYER");

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
	enemyBaseComponent.state->CreateState<BossRun>(GetID(B_RUN));
	enemyBaseComponent.state->CreateState<BossNormalAttack1>(GetID(B_N_ATTACK1));
	enemyBaseComponent.state->CreateState<BossNormalAttack2>(GetID(B_N_ATTACK2));
	enemyBaseComponent.state->CreateState<BossNormalAttack3>(GetID(B_N_ATTACK3));
	enemyBaseComponent.state->CreateState<BossSpecialAttack1>(GetID(B_S_ATTACK1));
	/*eCom.state->NodeDrawReady();*/

	enemyBaseComponent.state->SetComponent<Boss>(this);

	enemyBaseComponent.state->StartState(B_IDOL);
}

void Boss::LookPlayer()
{
	//プレイヤーのポジションを格納させる
	VECTOR3 targetPos = enemyBaseComponent.playerObj->GetTransform()->position;
	//Playerの方をゆっくり向く
	VECTOR3 distance = targetPos - bossTransform->position;

	float direction = -atan2f(distance.z, distance.x) - 0.5f * DX_PI_F;
	float sign = direction - bossTransform->rotation.y;

	sign -= floorf(sign / DX_PI_F / 2) * DX_PI_F * 2;
	const float LOOK_SPEED = 0.2f;
	if (sign > DX_PI_F)
		sign -= 2 * DX_PI_F;
	if (sign > LOOK_SPEED)
		bossTransform->rotation.y += LOOK_SPEED;
	else if (sign < -LOOK_SPEED)
		bossTransform->rotation.y -= LOOK_SPEED;
	else
		bossTransform->rotation.y = direction;
}
