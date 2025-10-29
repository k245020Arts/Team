#include "playerManager.h"
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
#include "EnemyManager.h"
#include "LoadManager.h"
#include "sphereCollider.h"
#include "Animator.h"
#include "ComponentLight.h"
#include "Shaker.h"
#include "MotionBlur.h"
#include "Object2D.h"
#include "Guage.h"
#include "rayCollider.h"

PlayerManager::PlayerManager()
{
	playerPointer = nullptr;

}

PlayerManager::~PlayerManager()
{

}

void PlayerManager::Update()
{
	
}

void PlayerManager::Draw()
{

}

void PlayerManager::CreatePlayer()
{
	playerPointer = new Object3D();
	//playerPointer->Init(VECTOR3(300, 100, 1500), VZero, VECTOR3(3.0f, 3.0f,3.0f), "PLAYER");
	playerPointer->Init(VECTOR3(300, 100, 1500), VZero, VECTOR3(3.0f ,3.0f,3.0f), "PLAYER");
	//‚â‚ç‚ê”»’è‚Ì’Ç‰Á
	ColliderBase* collider = playerPointer->Component()->AddComponent<SphereCollider>();
	CollsionInfo info;
	info.parentTransfrom = playerPointer->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = false;
	info.tag = CollsionInformation::Tag::PLAYER;
	info.size = 1.0f;
	collider->CollsionAdd(info, Transform(VECTOR3(0, 0, 0), VZero, VECTOR3(150.0f, 0, 0)));
	//‚â‚ç‚ê”»’è‚Ì’Ç‰Á
	ColliderBase* collider2 = playerPointer->Component()->AddComponent<SphereCollider>();
	collider2->CollsionAdd(info, Transform(VECTOR3(0, 100, 0), VZero, VECTOR3(200.0f, 0, 0)));

	RayCollider* collider3 = playerPointer->Component()->AddComponent<RayCollider>();
	info.shape = CollsionInformation::RAY;
	info.tag = CollsionInformation::P_FLOOR;
	collider3->RaySet(info, Transform(VECTOR3(0, 100, 0), VZero, VECTOR3(1.0f, 1.0, 1.0)), Transform(VECTOR3(0, -10, 0), VZero, VECTOR3(1.0f, 1, 1)));

	Shaker* shaker = playerPointer->Component()->AddComponent<Shaker>();
	
	MeshRenderer* me2 = playerPointer->Component()->AddComponent<MeshRenderer>();
	
	Physics* physics = playerPointer->Component()->AddComponent<Physics>();
	Player* player = playerPointer->Component()->AddComponent<Player>();
	
	physics->Start(PlayerInformation::BASE_GRAVITY, PlayerInformation::BASE_INTERIA);
	/*Camera* camera = playerPointer->Component()->AddComponent<Camera>();
	camera->Start(FindGameObject<EnemyManager>()->GetEnemy());*/
	//me2->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "exo_red", ID::P_MODEL));
	me2->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "Player4", ID::P_MODEL));
	me2->RotationMesh(0, 180.0f * DegToRad);

	Animator* anim = playerPointer->Component()->AddComponent<Animator>();
	anim->BaseModelSet(Load::GetHandle(ID::P_MODEL),"mixamorig:Hips");
	anim->AddFile(ID::IDType::P_ANIM_IDOL, "P_IDOL_M3", true, 1.0f);
	anim->AddFile(ID::IDType::P_ANIM_RUN, "P_RUN_M_1", true, 1.3f);
	anim->AddFile(ID::IDType::P_ANIM_AVOID, "P_AVOID_M_1", false, 1.0f,0.0f,5.0f);
	anim->AddFile(ID::IDType::P_ANIM_JUST_AVOID, "P_JUST_AVOID_M_1", false, 1.5f,10.0f,25.0f);
	anim->AddFile(ID::IDType::P_DAMAGE, "P_DAMAGE_M_1", false, 2.0f,0.0f,10.0f);
	anim->AddFile(ID::IDType::P_FALL, "P_FALL_M_1", true, 1.0f,0.0f,10.0f);
	anim->AddFile(ID::IDType::P_GETUP, "P_GETUP_M_1", false, 1.0f,0.0f,10.0f);
	//anim->AddFile(ID::IDType::P_ANIM_ATTACK1, "P_ATTACK05", false, 1.2f,7.0f,15.0f);
	anim->AddFile(ID::IDType::P_ANIM_ATTACK1, "P_ATTACK01_M_1", false, 1.8f,20.0f,35.0f);
	anim->AddFile(ID::IDType::P_ANIM_JUST_AVOID_ATTACK1, "P_ATTACK11", false, 9.0f,7.0f,25.0f);
	//anim->AddFile(ID::IDType::P_ANIM_ATTACK2, "P_ATTACK06", false, 1.2f, 7.0f, 15.0f);
	anim->AddFile(ID::IDType::P_ANIM_ATTACK2, "P_ATTACK02_M_1", false, 1.6f, 10.0f, 30.0f);
	//anim->AddFile(ID::IDType::P_ANIM_JUST_AVOID_ATTACK2, "P_ATTACK44", false, 0.7f, 4.0f, 21.0f);
	anim->AddFile(ID::IDType::P_ANIM_JUST_AVOID_ATTACK2, "P_ATTACK11_M_1", false, 0.7f, 4.0f, 21.0f);
	//anim->AddFile(ID::IDType::P_ANIM_ATTACK3, "P_ATTACK03", false, 1.2f, 9.0f, 15.0f);
	anim->AddFile(ID::IDType::P_ANIM_ATTACK3, "P_ATTACK03_M_1", false, 1.6f, 15.0f, 25.0f);
	//anim->AddFile(ID::IDType::P_ANIM_JUST_AVOID_ATTACK3, "P_ATTACK22", false, 0.8f, 9.0f, 20.0f);
	anim->AddFile(ID::IDType::P_ANIM_JUST_AVOID_ATTACK3, "P_ATTACK05_M_1", false, 1.5f, 23.0f, 30.0f);
	//anim->AddFile(ID::IDType::P_ANIM_ATTACK4, "P_ATTACK04", false, 2.0f, 9.0f, 12.0f);
	anim->AddFile(ID::IDType::P_ANIM_ATTACK4, "P_ATTACK04_M_1", false, 2.6f, 30.0f, 42.0f);
	anim->AddFile(ID::IDType::P_ANIM_JUST_AVOID_ATTACK4, "P_ATTACK46", false, 2.0f, 9.0f, 12.0f);
	anim->AddFile(ID::IDType::P_DIE, "P_DIE_M_1", false, 0.5f, 9.0f, 12.0f);

	
	MotionBlur* blur = playerPointer->Component()->AddComponent<MotionBlur>();

	blur->Start(Load::GetHandle(ID::P_MODEL));

	/*ComponentLight* componentLoght = playerPointer->Component()->AddComponent<ComponentLight>();
	componentLoght->SpotLightHandleStart(VECTOR3(0, 0, 0), VECTOR3(0.0f, 0.0f, 0.0f), DX_PI_F, DX_PI_F / 2, 4000, 0.0f, 0.002f, 0.0f);*/

	player->Start(playerPointer);

	Object2D* guage = new Object2D();
	guage->Init(VECTOR2F(970, 1000), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 0.5f), "playerHpGuage");

	playerPointer->AddChild(guage);

	Guage* g = guage->Component()->AddComponent<Guage>();
	g->GuageDrawReady<Player>(Load::LoadImageGraph(Load::IMAGE_PATH + "playerHpGuage", ID::PLAYER_HP_GUAGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F);

}
