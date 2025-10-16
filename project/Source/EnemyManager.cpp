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
#include "Boss.h"

EnemyManager::EnemyManager()
{
	//CreateEnemy();
	enemy.clear();
}

EnemyManager::~EnemyManager()
{
	enemy.clear();
}

void EnemyManager::Update()
{
	
}

void EnemyManager::Draw()
{
}

std::list<Object3D*> EnemyManager::GetEnemy()
{
	return enemy;
}

void EnemyManager::CreateEnemy()
{
	Object3D* e;
	e = new Object3D();
	e->Init(EnemyInformation::BASE_POS, VZero, VECTOR3(2.0f, 2.0f,2.0f), "ENEMY");
	//当たり判定を生成（やられ判定）
	SphereCollider* collider = e->Component()->AddComponent<SphereCollider>();
	CollsionInfo info;
	info.parentTransfrom = e->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = false;
	info.tag = CollsionInformation::Tag::ENEMY;
	info.size = 1.0f;
	collider->CollsionAdd(info, Transform(VECTOR3(0, 0, 0), VZero, VECTOR3(350.0f, 1.0f, 1.0f)));
	//当たり判定を生成（やられ判定）
	SphereCollider* collider2 = e->Component()->AddComponent<SphereCollider>();
	CollsionInfo info2;
	info2.parentTransfrom = e->GetTransform();
	info2.shape = CollsionInformation::SPHERE;
	info2.oneColl = false;
	info2.tag = CollsionInformation::Tag::ENEMY;
	info.size = 1.0f;
	collider2->CollsionAdd(info2, Transform(VECTOR3(0, 150, 0), VZero, VECTOR3(250.0f, 1.0f, 1.0f)));

	
	Shaker* shaker = e->Component()->AddComponent<Shaker>();

	MeshRenderer* me = e->Component()->AddComponent<MeshRenderer>();
	me->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "RPG-Character", ID::IDType::E_MODEL));
	me->RotationMesh(1, DX_PI_F);

	Animator* anim = e->Component()->AddComponent<Animator>();
	anim->AddFile(ID::E_ATTACK1, "E_ATTACK1", false, 0.7f,11.0f,18.0f);
	anim->AddFile(ID::E_ANIM_IDOL, "E_IDOL", true, 1.0f);
	anim->AddFile(ID::E_DAMAGE, "E_DAMAGE", false, 1.0f);
	anim->AddFile(ID::E_FALL, "E_FALL", true, 1.0f);
	anim->AddFile(ID::E_GETUP, "E_GETUP", false, 2.0f);
	anim->AddFile(ID::E_RUN, "E_RUN", true, 1.0f);
	anim->AddFile(ID::IDType::E_DIE, "E_DIE", false, 0.5f, 9.0f, 12.0f);
	anim->BaseModelSet(Load::GetHandle(ID::E_MODEL));
	anim->Play(ID::E_ANIM_IDOL);

	Physics* physics = e->Component()->AddComponent<Physics>();
	physics->Start(VECTOR3(0.0f, -150.0f, 0.0f), VECTOR3(3000.0f, 3000.0f, 3000.0f));


	Enemy* en = e->Component()->AddComponent<Enemy>();
	//e->Start(enemy);
	
	enemy.emplace_back(e);
}

void EnemyManager::PlayerObjPointer()
{
	Object3D* obj = FindGameObjectWithTag<Object3D>("PLAYER");
	for (auto itr = enemy.begin(); itr != enemy.end(); itr++) {
		Enemy* e = (*itr)->Component()->GetComponent<Enemy>();
		e->PlayerPointerSet(obj);
		e->Start((*itr));
		(*itr)->SetDrawOrder(-1);

		Object2D* guage = new Object2D();

		guage->Init(VECTOR2F(970, 0), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 0.5f), "enemyHpGuage");

		(*itr)->AddChild(guage);

		Guage* g = guage->Component()->AddComponent<Guage>();
		g->GuageDrawReady<Enemy>(Load::LoadImageGraph(Load::IMAGE_PATH + "playerHpGuage", ID::PLAYER_HP_GUAGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F);
	}
	player = obj;
}

void EnemyManager::CreateBoss()
{
	Object3D* boss;
	boss = new Object3D();
	boss->Init(Transform(VECTOR3(0, 450, 0), VZero, VECTOR3(5, 5, 5)), "Boss");

	SphereCollider* collider = boss->Component()->AddComponent<SphereCollider>();
	CollsionInfo info;
	info.parentTransfrom = boss->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = false;
	info.tag = CollsionInformation::Tag::BOSS;
	info.size = 1.0f;
	collider->CollsionAdd(info, Transform(VECTOR3(0, 0, 0), VZero, VECTOR3(350.0f, 1.0f, 1.0f)));
	//当たり判定を生成（やられ判定）
	SphereCollider* collider2 = boss->Component()->AddComponent<SphereCollider>();
	CollsionInfo info2;
	info2.parentTransfrom = boss->GetTransform();
	info2.shape = CollsionInformation::SPHERE;
	info2.oneColl = false;
	info2.tag = CollsionInformation::Tag::BOSS;
	info.size = 1.0f;
	collider2->CollsionAdd(info2, Transform(VECTOR3(0, 150, 0), VZero, VECTOR3(250.0f, 1.0f, 1.0f)));


	Shaker* shaker = boss->Component()->AddComponent<Shaker>();

	MeshRenderer* m = boss->Component()->AddComponent<MeshRenderer>();
	m->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "BossModel", ID::B_MODEL));
	m->RotationMesh(1, DX_PI_F);

	Boss* b = boss->Component()->AddComponent<Boss>();

	Animator* anim = boss->Component()->AddComponent<Animator>();
	anim->AddFile(ID::B_IDOL, "B_ATTACK3", true);
	anim->BaseModelSet(Load::GetHandle(ID::B_MODEL));

	b->Start(boss);

	enemy.emplace_back(boss);
}

bool EnemyManager::PlayerDistance(Camera* camera)
{
	float distance = 1000000;
	VECTOR3 dir = camera->GetTarget() - camera->GetCameraTransform()->position;
	dir = dir.Normalize();
	auto keepItr = enemy.begin();
	auto lastItr = enemy.begin();

	for (auto itr = enemy.begin(); itr != enemy.end(); itr++) {
		CharaBase* e;
		if ((*itr)->GetTag() == "ENEMY") {
			e = (*itr)->Component()->GetComponent<Enemy>();
		}
		else {
			e = (*itr)->Component()->GetComponent<Boss>();
		}
		
		if (e == nullptr) {
			continue;
		}
		if (e->GetLastTarget()) {
			lastItr = itr;
			continue;
		}
		VECTOR3 target = (*itr)->GetTransform()->position - player->GetTransform()->position;
		float dist = target.Size();
		
		if (VDot(dir.Normalize(), target.Normalize()) < cosf(60.0f * DegToRad)) {
			continue;
		}
		if (distance > dist) {
			distance = dist;
			keepItr = itr;
		}
	}

	if (distance < 100000) {
		
		if ((*lastItr)->GetTag() == "ENEMY") {
			(*lastItr)->Component()->GetComponent<Enemy>()->LastTargetOut();
		}
		else {
			(*lastItr)->Component()->GetComponent<Boss>()->LastTargetOut();
		}
		if ((*keepItr)->GetTag() == "ENEMY") {
			(*keepItr)->Component()->GetComponent<Enemy>()->LastTargetIn();
		}
		else {
			(*keepItr)->Component()->GetComponent<Boss>()->LastTargetIn();
		}
		player->Component()->GetComponent<Player>()->TargetObjSet(*keepItr);
		camera->TargetSet(*keepItr);
		return true;
	}
	if ((*lastItr)->GetTag() == "ENEMY") {
		(*lastItr)->Component()->GetComponent<Enemy>()->LastTargetOut();
	}
	else {
		(*lastItr)->Component()->GetComponent<Boss>()->LastTargetOut();
	}
	player->Component()->GetComponent<Player>()->TargetObjSet(nullptr);
	return false;
}
;
