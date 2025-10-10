#include "EnemyManager.h"
#include "MeshRenderer.h"
#include "transform.h"
#include "MeshRenderer2D.h"
#include "player.h"
#include "Physics.h"
#include "camera.h"
#include "Hierarchy.h"
#include "LoadManager.h"
#include "sphereCollider.h"
#include "Animator.h"
#include "enemy.h"
#include "CharaWeapon.h"
#include "Shaker.h"
#include "Object2D.h"
#include "Guage.h"

EnemyManager::EnemyManager()
{
	//CreateEnemy();
	enemy = nullptr;
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Update()
{
	
}

void EnemyManager::Draw()
{
}

Object3D* EnemyManager::GetEnemy()
{
	return enemy;
}

void EnemyManager::CreateEnemy()
{
	enemy = new Object3D();
	enemy->Init(EnemyInformation::BASE_POS, VZero, VECTOR3(0.1, 0.1, 0.1), "ENEMY");
	//当たり判定を生成（やられ判定）
	SphereCollider* collider = enemy->Component()->AddComponent<SphereCollider>();
	CollsionInfo info;
	info.parentTransfrom = enemy->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = false;
	info.tag = CollsionInformation::Tag::ENEMY;
	info.size = 1.0f;
	collider->CollsionAdd(info, Transform(VECTOR3(0, 0, 0), VZero, VECTOR3(350.0f, 1.0f, 1.0f)));
	//当たり判定を生成（やられ判定）
	SphereCollider* collider2 = enemy->Component()->AddComponent<SphereCollider>();
	CollsionInfo info2;
	info2.parentTransfrom = enemy->GetTransform();
	info2.shape = CollsionInformation::SPHERE;
	info2.oneColl = false;
	info2.tag = CollsionInformation::Tag::ENEMY;
	info.size = 1.0f;
	collider2->CollsionAdd(info2, Transform(VECTOR3(0, 150, 0), VZero, VECTOR3(250.0f, 1.0f, 1.0f)));

	
	Shaker* shaker = enemy->Component()->AddComponent<Shaker>();

	MeshRenderer* me = enemy->Component()->AddComponent<MeshRenderer>();
	me->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "RPG-Character", ID::IDType::E_MODEL));
	me->RotationMesh(1, DX_PI_F);

	Animator* anim = enemy->Component()->AddComponent<Animator>();
	anim->AddFile(ID::E_ATTACK1, "E_ATTACK1", false, 0.7f,11.0f,18.0f);
	anim->AddFile(ID::E_ANIM_IDOL, "E_IDOL", true, 1.0f);
	anim->AddFile(ID::E_DAMAGE, "E_DAMAGE", false, 1.0f);
	anim->AddFile(ID::E_FALL, "E_FALL", true, 1.0f);
	anim->AddFile(ID::E_GETUP, "E_GETUP", false, 2.0f);
	anim->AddFile(ID::E_RUN, "E_RUN", true, 1.0f);
	anim->AddFile(ID::IDType::E_DIE, "E_DIE", false, 0.5f, 9.0f, 12.0f);
	anim->BaseModelSet(Load::GetHandle(ID::E_MODEL));
	anim->Play(ID::E_ANIM_IDOL);

	Physics* physics = enemy->Component()->AddComponent<Physics>();
	physics->Start(VECTOR3(0.0f, -150.0f, 0.0f), VECTOR3(3000.0f, 3000.0f, 3000.0f));


	Enemy* e = enemy->Component()->AddComponent<Enemy>();
	//e->Start(enemy);
	
	
}

void EnemyManager::PlayerObjPointer()
{
	Object3D* obj = FindGameObjectWithTag<Object3D>("PLAYER");
	Enemy* e = enemy->Component()->GetComponent<Enemy>();
	e->PlayerPointerSet(obj);
	e->Start(enemy);
	enemy->SetDrawOrder(-1);

	Object2D* guage = new Object2D();

	guage->Init(VECTOR2F(970, 0), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 0.5f), "enemyHpGuage");

	enemy->AddChild(guage);

	Guage* g = guage->Component()->AddComponent<Guage>();
	g->GuageDrawReady<Enemy>(Load::LoadImageGraph(Load::IMAGE_PATH + "playerHpGuage", ID::PLAYER_HP_GUAGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F);
}
