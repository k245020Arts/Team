#include "TrashEnemy.h"
#include "../../Component/Animator/Animator.h"
#include "../../Component/Physics/Physics.h"
#include "../../Common/InputManager/ControllerInputManager.h"
#include "../../Component/Collider/SphereCollider.h"
#include "../../Component/Color/Color.h"
#include "../../Component/Shaker/Shaker.h"
#include "../../../ImGui/imgui.h"
#include "../../Player/Player.h"
#include "../TrashEnemy/EnemyState/EnemyBlowAway.h"
#include "../../Camera/CameraManager.h"
#include "../../Camera/Camera.h"
#include "../../Common/Effect/EffectManager.h"
#include "../../Common/Sound/SoundManager.h"
#include "../../Common/ID/ID.h"//
#include "../../Weapon/WeaponManager.h"
#include "../../State/StateManager.h"
#include "../../GameManager/GameManager.h"
#include "../../Player/PlayerState/AttackState/PlayerAttackStateBase.h"
#include "../../Weapon/SwordEffect.h"
#include "../../Weapon/CharaWeapon.h"
#include "../../Common/LoadManager.h"

#include "TrashEnemyState/T_EnemyIdol.h"
#include "TrashEnemyState/T_EnemyRun.h"
#include "TrashEnemyState/T_EnemyAttack.h"
#include "TrashEnemyState/T_EnemyDead.h"

TrashEnemy::TrashEnemy()
{
	tag = Function::GetClassNameC<TrashEnemy>();

	hp = 100;
	maxHp = hp;
}

TrashEnemy::~TrashEnemy()
{

}

void TrashEnemy::Update()
{
	EnemyBase::Update();
	if (hp <= 0)
		enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_DEAD);
}

void TrashEnemy::Draw()
{
}

void TrashEnemy::Start(Object3D* _obj)
{
	enemyBaseComponent.state = obj->Component()->AddComponent<StateManager>();
	enemyBaseComponent.playerObj = FindGameObjectWithTag<Object3D>("PLAYER");
	pState = enemyBaseComponent.playerObj->Component()->GetComponent<Player>()->GetPlayerStateManager();

	enemyBaseComponent.collider = FindGameObjects<ColliderBase>();

	enemyBaseComponent.anim = obj->Component()->GetComponent<Animator>();
	enemyBaseComponent.physics = obj->Component()->GetComponent<Physics>();
	enemyBaseComponent.control = FindGameObject<ControllerInputManager>();
	enemyBaseComponent.color = obj->Component()->GetComponent<Color>();
	enemyBaseComponent.color->setRGB(Color::Rgb(0.0f, 0.0f, 0.0f, 255.0f));
	enemyBaseComponent.shaker = obj->Component()->GetComponent<Shaker>();
	enemyBaseComponent.effect = FindGameObject<EffectManager>();
	enemyBaseComponent.sound = FindGameObject<SoundManager>();
	enemyBaseComponent.physics = obj->Component()->GetComponent<Physics>();

	using namespace ID;

	enemyBaseComponent.state->CreateState<T_EnemyIdol>("_T_EnemyIdol", StateID::T_ENEMY_IDOL_S);
	enemyBaseComponent.state->CreateState<T_EnemyRun>("_T_EnemyRun", StateID::T_ENEMY_RUN_S);
	enemyBaseComponent.state->CreateState<T_EnemyAttack>("_T_EnemyAttack", StateID::T_ENEMY_ATTACK_S);
	enemyBaseComponent.state->CreateState<T_EnemyDead>("_T_EnemyDead", StateID::T_ENEMY_DEAD);

	enemyBaseComponent.state->SetComponent<TrashEnemy>(this);

	// スタートステートも StateID 化
	enemyBaseComponent.state->StartState(StateID::T_ENEMY_ATTACK_S);
	enemyBaseComponent.weapon = FindGameObject<WeaponManager>();

	chara = obj->Component()->AddComponent<CharaWeapon>();
	chara->ObjectPointer(_obj, 10, ID::E_MODEL, -1);
	chara->SetImage(Load::GetHandle(ID::SWORD_EFFECT_B));
}

void TrashEnemy::CreateTrashEnemy(VECTOR3 _pos)
{
	obj->GetTransform()->position = _pos;
}

void TrashEnemy::LookPlayer()
{
	//プレイヤーのポジションを格納させる
	VECTOR3 targetPos = enemyBaseComponent.playerObj->GetTransform()->position;
	VECTOR3 distance = targetPos - obj->GetTransform()->position;
	//向くべき角度
	float direction = -atan2f(distance.z, distance.x) - 0.5f * DX_PI_F;
	//その角度とどれだけ差があるか
	float sign = direction - obj->GetTransform()->rotation.y;
	//切り捨てして180の境界線を無くす
	sign -= floorf(sign / DX_PI_F / 2) * DX_PI_F * 2.0f;
	if (sign > DX_PI_F)
		sign -= 2 * DX_PI_F;
	//向くスピード(ラジアン)
	const float LOOK_SPEED = 0.1;
	//Playerの方をゆっくり向く
	if (sign > LOOK_SPEED)
		obj->GetTransform()->rotation.y += LOOK_SPEED;
	else if (sign < -LOOK_SPEED)
		obj->GetTransform()->rotation.y -= LOOK_SPEED;
	else
		obj->GetTransform()->rotation.y = direction;
}

void TrashEnemy::Trail()
{
	chara->CreateSwordEffect(VECTOR3(70, 0, -50), VECTOR3(120, 0, 50), 200.0f, 10.0f, 00.0f, 155.0f, 28, 0.5f);
}
