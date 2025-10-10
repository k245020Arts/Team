#include "BossManager.h"
#include "transform.h"
#include "MeshRenderer.h"
#include "ComponentManager.h"
#include "LoadManager.h"
#include "sphereCollider.h"
#include "Shaker.h"

BossManager::BossManager()
{
}

BossManager::~BossManager()
{
}

void BossManager::CreateBoss()
{
	boss = new Object3D();
	boss->Init(Transform(VECTOR3(0,450,0), VZero, VECTOR3(5, 5, 5)), "Boss");

	SphereCollider* collider = boss->Component()->AddComponent<SphereCollider>();
	CollsionInfo info;
	info.parentTransfrom = boss->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = false;
	info.tag = CollsionInformation::Tag::BOSS;
	info.size = 1.0f;
	collider->CollsionAdd(info, Transform(VECTOR3(0, 0, 0), VZero, VECTOR3(350.0f, 1.0f, 1.0f)));
	//“–‚½‚è”»’è‚ð¶¬i‚â‚ç‚ê”»’èj
	SphereCollider* collider2 = boss->Component()->AddComponent<SphereCollider>();
	CollsionInfo info2;
	info2.parentTransfrom = boss->GetTransform();
	info2.shape = CollsionInformation::SPHERE;
	info2.oneColl = false;
	info2.tag = CollsionInformation::Tag::BOSS;
	info.size = 1.0f;
	collider2->CollsionAdd(info2, Transform(VECTOR3(0, 150, 0), VZero, VECTOR3(250.0f, 1.0f, 1.0f)));


	Shaker* shaker = boss->Component()->AddComponent<Shaker>();

	MeshRenderer* m =  boss->Component()->AddComponent<MeshRenderer>();
	m->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "BossModel", ID::B_MODEL));
	m->RotationMesh(1, DX_PI_F);

	
}
