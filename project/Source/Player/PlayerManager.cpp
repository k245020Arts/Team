#include "playerManager.h"
#include "../Component/Object/Object3D.h"
#include "../Component/ComponentManager.h"
#include "../Component/MeshRenderer/MeshRenderer.h"
#include "../Component/Transform/Transform.h"
#include "../Component/MeshRenderer2D/MeshRenderer2D.h"
#include "player.h"
#include "../Component/Physics/Physics.h"
#include "../Camera/Camera.h"
#include "../Component/Hierarchy/Hierarchy.h"
#include "../State/StateManager.h"
#include "../Enemy/EnemyManager.h"
#include "../Common/LoadManager.h"
#include "../Component/Collider/SphereCollider.h"
#include "../Component/Animator/Animator.h"
#include "../Component/Light/ComponentLight.h"
#include "../Component/Shaker/Shaker.h"
#include "../Component/MotionBlur/MotionBlur.h"
#include "../Component/Object/Object2D.h"
#include "../Component/Guage/Guage.h"
#include "../Component/Collider/RayCollider.h"
#include "../Component/Shadow/Shadow.h"

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
	playerPointer->SetDrawOrder(-5);
	//playerPointer->Init(VECTOR3(300, 100, 1500), VZero, VECTOR3(3.0f, 3.0f,3.0f), "PLAYER");
	playerPointer->Init(VECTOR3(300,300, -2000), VZero, VECTOR3(3.0f ,3.0f,3.0f), "PLAYER");
	//‚â‚ç‚ê”»’è‚Ì’Ç‰Á
	ColliderBase* collider = playerPointer->Component()->AddComponent<SphereCollider>();
	CollsionInfo info;
	info.parentTransfrom	= playerPointer->GetTransform();
	info.shape				= CollsionInformation::SPHERE;
	info.oneColl			= false;
	info.tag				= CollsionInformation::Tag::PLAYER;
	info.size				= 1.0f;
	collider->CollsionAdd(info, Transform(VECTOR3(0, 0, 0), VZero, VECTOR3(150.0f, 0, 0)));

	//‚â‚ç‚ê”»’è‚Ì’Ç‰Á
	ColliderBase* collider2 = playerPointer->Component()->AddComponent<SphereCollider>();
	collider2->CollsionAdd(info, Transform(VECTOR3(0, 100, 0), VZero, VECTOR3(200.0f, 0, 0)));
	RayCollider* collider3	= playerPointer->Component()->AddComponent<RayCollider>();
	info.shape				= CollsionInformation::RAY;
	info.tag				= CollsionInformation::P_FLOOR;
	collider3->RaySet(info, Transform(VECTOR3(0, 100, 0), VZero, VECTOR3(1.0f, 1.0, 1.0)), Transform(VECTOR3(0, -10, 0), VZero, VECTOR3(1.0f, 1, 1)));

	Shaker* shaker		= playerPointer->Component()->AddComponent<Shaker>();
	
	MeshRenderer* me2	= playerPointer->Component()->AddComponent<MeshRenderer>();
	
	Physics* physics	= playerPointer->Component()->AddComponent<Physics>();
	Player* player		= playerPointer->Component()->AddComponent<Player>();
	
	physics->Start(PlayerInformation::BASE_GRAVITY, PlayerInformation::BASE_INTERIA);
	/*Camera* camera = playerPointer->Component()->AddComponent<Camera>();
	camera->Start(FindGameObject<EnemyManager>()->GetEnemy());*/
	//me2->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "exo_red", ID::P_MODEL));
	me2->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "Player4", ID::P_MODEL));
	me2->RotationMesh(0, 180.0f * DegToRad);

	Animator* anim = playerPointer->Component()->AddComponent<Animator>();
	anim->BaseModelSet(Load::GetHandle(ID::P_MODEL),"mixamorig:Hips");
	anim->AddFile(ID::IDType::P_ANIM_IDOL,					"P_IDOL_M3", true, 1.0f);
	anim->AddFile(ID::IDType::P_ANIM_RUN,					"P_RUN_M_1", true, 1.5f);
	anim->AddFile(ID::IDType::P_ANIM_AVOID,					"P_AVOID_M_1", false, 2.0f,0.0f,5.0f);
	anim->AddFile(ID::IDType::P_ANIM_JUST_AVOID,			"P_JUST_AVOID_M_1", false, 1.5f,10.0f,25.0f);
	anim->AddFile(ID::IDType::P_DAMAGE,						"P_DAMAGE_M_1", false, 2.0f,0.0f,10.0f);
	anim->AddFile(ID::IDType::P_FALL,						"P_FALL_M_1", true, 1.0f,0.0f,10.0f);
	anim->AddFile(ID::IDType::P_GETUP,						"P_GETUP_M_1", false, 1.0f,0.0f,10.0f);
	anim->AddFile(ID::IDType::P_ANIM_ATTACK1,				"P_ATTACK01_M_1", false, 2.3f,20.0f,35.0f);
	anim->AddFile(ID::IDType::P_ANIM_JUST_AVOID_ATTACK1,	"P_ATTACK11", false, 9.0f,7.0f,25.0f);
	anim->AddFile(ID::IDType::P_ANIM_ATTACK2,				"P_ATTACK02_M_1", false, 2.1f, 10.0f, 30.0f);
	anim->AddFile(ID::IDType::P_ANIM_JUST_AVOID_ATTACK2,	"P_ATTACK11_M_1", false, 0.7f, 4.0f, 21.0f);
	anim->AddFile(ID::IDType::P_ANIM_ATTACK3,				"P_ATTACK03_M_1", false, 2.1f, 15.0f, 25.0f);
	anim->AddFile(ID::IDType::P_ANIM_JUST_AVOID_ATTACK3,	"P_ATTACK05_M_1", false, 1.5f, 23.0f, 30.0f);
	anim->AddFile(ID::IDType::P_ANIM_ATTACK4,				"P_ATTACK04_M_1", false, 3.1f, 30.0f, 42.0f);
	anim->AddFile(ID::IDType::P_ANIM_JUST_AVOID_ATTACK4,	"P_ATTACK46", false, 2.0f, 9.0f, 12.0f);
	anim->AddFile(ID::IDType::P_DIE,						"P_DIE_M_1", false, 0.5f, 9.0f, 12.0f);
	anim->AddFile(ID::IDType::P_TURN_ANIM,					"P_TURN", false, 1.5f, 9.0f, 12.0f);
	anim->SetMaxFrame(ID::P_GETUP, 53.0f);
	
	MotionBlur* blur = playerPointer->Component()->AddComponent<MotionBlur>();

	blur->Start(Load::GetHandle(ID::P_MODEL));

	/*ComponentLight* componentLoght = playerPointer->Component()->AddComponent<ComponentLight>();
	componentLoght->SpotLightHandleStart(VECTOR3(0, 0, 0), VECTOR3(0.0f, 0.0f, 0.0f), DX_PI_F, DX_PI_F / 2, 4000, 0.0f, 0.002f, 0.0f);*/

	player->Start(playerPointer);

	Object3D* shadow = new Object3D();
	shadow->Init(Transform(VECTOR3(0.0f, -100.0f, 0.0f), VZero, VECTOR3(playerPointer->GetTransform()->scale.x - 1.0f, 0.1f, playerPointer->GetTransform()->scale.z - 1.0f)), "PlayerShadow");
	Shadow* s = shadow->Component()->AddComponent<Shadow>();
	s->Start();

	RayCollider* collider4 = shadow->Component()->AddComponent<RayCollider>();
	info.shape = CollsionInformation::RAY;
	info.tag = CollsionInformation::SHADOW;
	collider4->RaySet(info, Transform(VECTOR3(0, 50, 0), VZero, VECTOR3(1.0f, 1.0, 1.0)), Transform(VECTOR3(0, -s->GetMaxDist(), 0), VZero, VECTOR3(1.0f, 1, 1)));

	playerPointer->AddChild(shadow);

	
	Object2D* guage = new Object2D();
	guage->Init(VECTOR2F(950.0f, 950.0f), VECTOR2F(0.0f, 0.0f), VECTOR2F(0.44f, 0.35f), "playerHp");

	playerPointer->AddChild(guage);

	Guage* g = guage->Component()->AddComponent<Guage>();
	g->EdgeDrawReady(Load::LoadImageGraph(Load::IMAGE_PATH + "playerHpEdge", ID::HP_EDGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F,Transform(VECTOR3(915.0f,950.0f,0.0f),VZero,VECTOR3(1.3f,1.0f,0.0f)));
	g->GuageDrawReady<Player>(Load::LoadImageGraph(Load::IMAGE_PATH + "playerHp", ID::PLAYER_HP_GUAGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F);

	
}
