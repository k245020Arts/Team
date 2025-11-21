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
#include "EnemyState/AttackState/EnemyAttack1.h"


TrashEnemy::TrashEnemy()
{
	tag = Function::GetClassNameC<TrashEnemy>();
}

TrashEnemy::~TrashEnemy()
{

}

void TrashEnemy::Update()
{
}

void TrashEnemy::Draw()
{
}

void TrashEnemy::Start()
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

	enemyBaseComponent.state->CreateState<T_EnemyIdol>(GetID(TE_IDOL));
	enemyBaseComponent.state->CreateState<T_EnemyRun>(GetID(TE_RUN));
	enemyBaseComponent.state->CreateState<EnemyAttack1>(GetID(E_ATTACK1));

	enemyBaseComponent.state->SetComponent<TrashEnemy>(this);
	enemyBaseComponent.state->StartState(TE_IDOL);
	enemyBaseComponent.weapon = FindGameObject<WeaponManager>();
}

void TrashEnemy::CreateTrashEnemy(VECTOR3 _pos)
{
	obj->GetTransform()->position = _pos;
	Start();
}
