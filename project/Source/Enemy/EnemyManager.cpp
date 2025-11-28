#include "EnemyManager.h"
#include "../Component/MeshRenderer/MeshRenderer.h"
#include "../Component/Transform/Transform.h"
#include "../Component/MeshRenderer2D/MeshRenderer2D.h"
#include "../Player/Player.h"
#include "../Component/Physics/Physics.h"
#include "../Camera/Camera.h"
#include "../Component/Hierarchy/Hierarchy.h"
#include "../Common/LoadManager.h"
#include "../Component/Collider/sphereCollider.h"
#include "../Component/Animator/Animator.h"
#include "TrashEnemy/enemy.h"
#include "../Weapon/WeaponManager.h"
#include "../Component/Shaker/Shaker.h"
#include "../Component/Object/Object2D.h"
#include "../Component/Guage/Guage.h"
#include "Boss/Boss.h"
#include "../Component/Collider/rayCollider.h"
#include "../Component/Collider/ModelCollider.h"
#include "../Component/Shadow/Shadow.h"

EnemyManager::EnemyManager()
{
	//CreateEnemy();
	enemy.clear();
	player = nullptr;
}

EnemyManager::~EnemyManager()
{
	enemy.clear();
	chara.clear();
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

std::list<Object3D*>::iterator EnemyManager::GetItr()
{
	auto e = enemy.begin();
	return e;
}

void EnemyManager::CreateEnemy()
{
	
	//e->Start(enemy);
	/*chara.emplace_back(en);
	
	enemy.emplace_back(e);*/
}

void EnemyManager::PlayerObjPointer()
{
	
	for (auto itr = enemy.begin(); itr != enemy.end(); itr++) {
		Enemy* e = (*itr)->Component()->GetComponent<Enemy>();
		//e->PlayerPointerSet(obj);
		e->Start((*itr));
		(*itr)->SetDrawOrder(-1);

		Object2D* guage = new Object2D();

		guage->Init(VECTOR2F(970, 0), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 0.5f), "enemyHpGuage");

		(*itr)->AddChild(guage);

		Guage* g = guage->Component()->AddComponent<Guage>();
		g->GuageDrawReady<Enemy>(Load::LoadImageGraph(Load::IMAGE_PATH + "playerHpGuage", ID::PLAYER_HP_GUAGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F);
	}
	
	player->Component()->GetComponent<Player>()->TargetObjSet(*enemy.begin());
	player->Component()->GetComponent<Player>()->HitObjectSet(*enemy.begin());
	FindGameObjectWithTag<Object3D>("CAMERA_OBJ")->Component()->GetComponent<Camera>()->TargetSet(*enemy.begin());
}

void EnemyManager::CreateBoss()
{
	Object3D* obj = FindGameObjectWithTag<Object3D>("PLAYER");
	player = obj;
	Object3D* boss;
	boss = new Object3D();
	float bSize = 8.0f;
	boss->Init(Transform(VECTOR3(0, 450, 2000), VZero, VECTOR3(bSize, bSize - 1, bSize)), "Boss");

	SphereCollider* collider = boss->Component()->AddComponent<SphereCollider>();
	CollsionInfo info;
	info.parentTransfrom = boss->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = false;
	info.tag = CollsionInformation::Tag::BOSS;
	info.size = 1.0f;
	collider->CollsionAdd(info, Transform(VECTOR3(0, 50, 0), VZero, VECTOR3(350.0f, 1.0f, 1.0f)));
	//当たり判定を生成（やられ判定）
	/*SphereCollider* collider2 = boss->Component()->AddComponent<SphereCollider>();
	CollsionInfo info2;
	info2.parentTransfrom = boss->GetTransform();
	info2.shape = CollsionInformation::SPHERE;
	info2.oneColl = false;
	info2.tag = CollsionInformation::Tag::BOSS;
	info.size = 1.0f;
	collider2->CollsionAdd(info2, Transform(VECTOR3(0, 150, 0), VZero, VECTOR3(250.0f, 1.0f, 1.0f)));*/
	RayCollider* collider3 = boss->Component()->AddComponent<RayCollider>();
	info.shape = CollsionInformation::RAY;
	info.tag = CollsionInformation::B_FLOOR;
	collider3->RaySet(info, Transform(VECTOR3(0, 200, 0), VZero, VECTOR3(1.0f, 10.0, 1.0)), Transform(VECTOR3(0, -0, 0), VZero, VECTOR3(1.0f, 1, 1)));

	ModelCollider* collider4 = boss->Component()->AddComponent<ModelCollider>();
	info.shape = CollsionInformation::MODEL;
	info.tag = CollsionInformation::BOSS;
	collider4->ModelColliderSet(info, Transform(VECTOR3(0, 50, 0), VZero, VECTOR3(0.35f, 1.0f, 0.35f)), Load::LoadModel(Load::MODEL_PATH + "wall",ID::BOSS_PUSH));


	Shaker* shaker = boss->Component()->AddComponent<Shaker>();

	MeshRenderer* m = boss->Component()->AddComponent<MeshRenderer>();
	m->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "BossModel", ID::B_MODEL));
	m->RotationMesh(0, 180.0f * DegToRad);

	Boss* b = boss->Component()->AddComponent<Boss>();

	Physics* physics = boss->Component()->AddComponent<Physics>();
	physics->Start(VECTOR3(0.0f, -1500.0f, 0.0f), VECTOR3(3000.0f, 3000.0f, 3000.0f));

	Animator* anim = boss->Component()->AddComponent<Animator>();
	anim->AddFile(ID::B_IDOL, "B_IDLE", true,1.0f);
	anim->AddFile(ID::B_COOLTIME, "B_IDLE2", true, 1.0f);
	anim->AddFile(ID::B_RUN, "B_WALK", true, 1.0f, 30.0f, 45.0f);
	anim->AddFile(ID::B_N_ATTACK1, "B_ATTACK1", false,1.2f,25.0f,60.0f);
	anim->AddFile(ID::B_N_ATTACK2, "B_ATTACK2", false,1.2f,25.0f,60.0f);
	anim->AddFile(ID::B_N_ATTACK3, "B_ATTACK3", false,1.2f,40.0f,55.0f);
	anim->AddFile(ID::B_N_ATTACK4, "B_ATTACK4", false,1.0f,40.0f,55.0f);
	anim->AddFile(ID::B_N_ATTACK5, "B_ATTACK5", false,1.0f,40.0f,55.0f);
	anim->AddFile(ID::B_N_ATTACK6, "B_ATTACK6", false,1.0f,40.0f,55.0f);
	anim->AddFile(ID::B_S_ATTACK1, "B_SATTACK1", false,1.0f,50.0f,60.0f);
	anim->AddFile(ID::B_S_ATTACK2, "B_SATTACK2", true,1.0f,0.0f,100.0f);
	anim->AddFile(ID::B_S_ATTACK2_STOP, "B_SATTACK2_STOP", false,1.0f);
	anim->AddFile(ID::B_S_ATTACK2_BEFORE, "B_SATTACK2_BEFORE", false,1.0f,0.0f,100.0f);
	anim->AddFile(ID::B_S_ATTACK1_SMALL, "B_SATTACK1_SAMLL", false,1.3f,40.0f,55.0f);
	anim->AddFile(ID::BOSS_DIE, "B_DIE", false,1.2f);
	anim->AddFile(ID::B_ROAR, "B_ROAR", false,1.2f,30.0f,40.0f);
	anim->AddFile(ID::B_DUSH, "B_RUN", true,1.2f,30.0f,40.0f);
	//anim->SetMaxFrame(ID::B_N_ATTACK1, 50.0f);
	anim->BaseModelSet(Load::GetHandle(ID::B_MODEL),1);

	b->Start(boss);

	Object2D* guage = new Object2D();

	guage->Init(VECTOR2F(950, 115), VECTOR2F(0.0f, 0.0f), VECTOR2F(0.5f, 0.5f), "bossHpGuage");

	boss->AddChild(guage);

	Guage* g = guage->Component()->AddComponent<Guage>();
	g->EdgeDrawReady(Load::LoadImageGraph(Load::IMAGE_PATH + "bossHpEdge1", ID::BOSS_HP_EDGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F, Transform(VECTOR3(915.0f, 120.0f, 0.0f), VZero, VECTOR3(1.0f, 1.0f, 0.0f)));
	g->GuageDrawReady<Boss>(Load::LoadImageGraph(Load::IMAGE_PATH + "playerHp", ID::PLAYER_HP_GUAGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F);
	chara.emplace_back(b);
	enemy.emplace_back(boss);

	Object3D* shadow = new Object3D();
	shadow->Init(Transform(VECTOR3(0.0f, -20.0f, 0.0f), VZero, VECTOR3(boss->GetTransform()->scale.x - 2.0f, 0.1f, boss->GetTransform()->scale.z - 2.0f)), "BossShadow");
	Shadow* s = shadow->Component()->AddComponent<Shadow>();
	s->Start();

	RayCollider* collider5 = shadow->Component()->AddComponent<RayCollider>();
	info.shape = CollsionInformation::RAY;
	info.tag = CollsionInformation::SHADOW;
	collider5->RaySet(info, Transform(VECTOR3(0, 50, 0), VZero, VECTOR3(1.0f, 1.0, 1.0)), Transform(VECTOR3(0, -s->GetMaxDist() , 0), VZero, VECTOR3(1.0f, 1, 1)));
	boss->AddChild(shadow);

	player->Component()->GetComponent<Player>()->TargetObjSet(*enemy.begin());
	player->Component()->GetComponent<Player>()->HitObjectSet(*enemy.begin());
	FindGameObjectWithTag<Object3D>("CAMERA_OBJ")->Component()->GetComponent<Camera>()->TargetSet(*enemy.begin());
}

bool EnemyManager::PlayerDistance(Camera* camera)
{
	float distance = 1000000;
	VECTOR3 dir = camera->GetTarget() - camera->GetCameraTransform()->position;
	dir = dir.Normalize();
	auto keepItr = chara.begin();
	auto lastItr = chara.begin();

	//一番近い敵を検出
	for (auto itr = chara.begin(); itr != chara.end(); itr++) {
		if ((*itr)->GetLastTarget()) {
			lastItr = itr; //最後にロックオンされていた敵のイテレーター
			continue;
		}
		VECTOR3 target = (*itr)->GetBaseObject()->GetTransform()->position - player->GetTransform()->position;
		float dist = target.Size();

		if (VDot(dir.Normalize(), target.Normalize()) < cosf(90.0f * DegToRad)) {
			continue;
		}
		if (distance > dist) {
			distance = dist;  //距離を入れる
			keepItr = itr; //このイテレーターがロックオンされるイテレーター
		}
	}

	//一定距離離れているとターゲットに入れない
	if (distance < 100000) {
		(*lastItr)->LastTargetOut();
		(*keepItr)->LastTargetIn();
		
		player->Component()->GetComponent<Player>()->TargetObjSet((*keepItr)->GetBaseObject());
		camera->TargetSet((*keepItr)->GetBaseObject());
		cameraTargetObj = (*keepItr)->GetBaseObject();
		return true;
	}
	(*lastItr)->LastTargetOut();
	cameraTargetObj = nullptr;
	player->Component()->GetComponent<Player>()->TargetObjSet(nullptr);
	return false;
}

bool EnemyManager::TargetCancel(Camera* camera)
{
	for (auto itr = chara.begin(); itr != chara.end(); itr++) {
		if ((*itr)->GetBaseObject() == cameraTargetObj) {
			(*itr)->LastTargetOut();
			player->Component()->GetComponent<Player>()->TargetObjSet(nullptr);
			return true;
		}
	}
	return false;
}

void EnemyManager::JustAvoidTargetChange(Object3D* _obj)
{
	for (auto itr = chara.begin(); itr != chara.end(); itr++) {
		if ((*itr)->GetBaseObject() == _obj) {
			(*itr)->LastTargetIn();
			cameraTargetObj = _obj;
			player->Component()->GetComponent<Player>()->TargetObjSet((*itr)->GetBaseObject());
			continue;
		}
		if ((*itr)->GetLastTarget()) {
			(*itr)->LastTargetOut();
			continue;
		}
	}
}

	/*for (auto itr = enemy.begin(); itr != enemy.end(); itr++) {
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
	player->Component()->GetComponent<Player>()->TargetObjSet(nullptr);*/
