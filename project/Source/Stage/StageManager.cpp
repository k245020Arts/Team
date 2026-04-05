#include "stageManager.h"
#include "../Component/Component.h"
#include "../Component/MeshRenderer/MeshRenderer.h"
#include "stage.h"
#include "../Component/ComponentManager.h"
#include "../Common/ResourceLoader.h"
#include "Wall.h"
#include "../Component/Collider/ModelCollider.h"
#include "../Player/Player.h"
#include "../Player/PlayerState/PlayerStateManager.h"
#include "../Component/Object/Object3D.h"
#include "Sky.h"
#include "../Component/Collider/AABBCollider.h"
#include "StageSelectData.h"
#include "../GameControler/GameControler.h"

#define NEW_STAGE

StageManager::StageManager()
{
	//modelHandle = MV1LoadModel("data/model/stage10.mv1");
	//CreateStage();
#ifdef NEW_STAGE
	
#else
	Load::LoadModel(Load::MODEL_PATH +  "new_Stage6", ID::S_MODEL);
#endif // NEW_STAGE

	Load("data/json/StageData/StageModelData.json");
	
	stage = nullptr;
	modelHandle = -1;
}

StageManager::~StageManager()
{
	//MV1DeleteModel(modelHandle);
}

void StageManager::Update()
{
	/*GameControler* gameControler = FindGameObject<GameControler>();
	if (gameControler != nullptr) {
		if (gameControler->GetStateNumber() != GameControler::GameState::BEFORE) {
			assert(false);
		}
	}*/
	
	
}

void StageManager::Draw()
{
}

void StageManager::CreateStage()
{
	
	stage = new Object3D();
	
#ifdef NEW_STAGE
	stage->Init(stageModelData[StageSelectData::GetInstance()->GetNowStageData().stageModelID].transform, "STAGE");
#else
	stage->Init(VECTOR3(100, 0, 100), VZero, VECTOR3(10, 10, 10), "STAGE");
#endif
	
	
	MeshRenderer* mesh = stage->Component()->AddComponent<MeshRenderer>();
	std::string mapName = stageModelData[StageSelectData::GetInstance()->GetNowStageData().stageModelID].mapFile;
	ResourceLoad::LoadModel(ResourceLoad::MODEL_PATH +  mapName, ID::PLAY_SCENE_BACKGROUND_MODEL);
	mesh->ModelHandle(ResourceLoad::GetHandle(ID::PLAY_SCENE_BACKGROUND_MODEL));
	Stage* stageComp = stage->Component()->AddComponent<Stage>();
	CreateWall();

	/*int num = 0;
	for (int x = 0; x < 10; x++) {
		for (int z = 0; z < 10; z++) {
			num++;
			Object3D* obj = new Object3D();
			obj->Init(VECTOR3(1000 * x, -100, 1000 * z), VZero, VECTOR3(10, 10, 10), "STAGE" + std::to_string(num));
			MeshRenderer* mesh = obj->Component()->AddComponent<MeshRenderer>();
			mesh->ModelHandle(modelHandle);
			Stage* stage = obj->Component()->AddComponent<Stage>();
		}
	}*/

}

void StageManager::CreateWall()
{
	//見えない壁のセット
	//ステージの範囲
	for (int i = 0; i < 4; i++) {
		Transform transform = Transform();
		switch (i)
		{
		case 0:
			transform = Transform(VECTOR3(WALL_EDGE_POS, 100, 100), VZero, VOne* VECTOR3(28.0f, 50, 120.0f));
			break;
		case 1:
			transform = Transform(VECTOR3(-WALL_EDGE_POS, 100, 100), VZero, VOne * VECTOR3(28.0f, 50, 120.0f));
			break;
		case 2:
			transform = Transform(VECTOR3(0, 100, -WALL_EDGE_POS), VZero, VOne * VECTOR3(120.0f, 50, 28.0f));
			break;
		case 3:
			transform = Transform(VECTOR3(0, 100, WALL_EDGE_POS), VZero, VOne * VECTOR3(120.0f, 50, 28.0f));
			break;
		//case 4:
			//transform = Transform(VECTOR3(0, -100, 0), VZero, VOne * VECTOR3(100.0f, 1, 100.0f));
			break;
		default:
			break;
		}
		Object3D* obj = new Object3D();
		obj->Init(transform, "WALL" + std::to_string(i));
		/*MeshRenderer* mesh = obj->Component()->AddComponent<MeshRenderer>();
		mesh->ModelHandle(Load::GetHandle(ID::WALL));
		mesh->DrawLocalPosition();*/
		Wall* wall = obj->Component()->AddComponent<Wall>();
		wall->ModelSet(ResourceLoad::GetHandle(ID::WALL));
		ModelCollider* c = obj->Component()->AddComponent<ModelCollider>();
		CollsionInfo info;
		info.oneColl = false;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::MODEL;
		info.tag = CollsionInformation::WALL;
		c->ModelColliderSet(info, Transform(VZero, VZero, VOne), MV1DuplicateModel(ResourceLoad::GetHandle(ID::WALL)));
		stage->AddChild(obj,false);
	}
	/*Object3D* obj = new Object3D();
	obj->Init(, "WALL1");
	Object3D* obj2 = new Object3D();
	obj2->Init(, "WALL2");
	Object3D* obj3 = new Object3D();
	obj3->Init(, "WALL3");
	Object3D* obj4 = new Object3D();
	obj4->Init(, "WALL4");*/
	CreateFloor();
}

void StageManager::CreateFloor()
{
	
	//for (int i = 0; i < 4; i++) {
	Transform transform;
	transform = Transform(VECTOR3(0.0f, -1000.0f, 0.0f), VZero, VOne * VECTOR3(500.0f, 10.0f, 500.0f));

	Object3D* obj = new Object3D();
	obj->Init(transform, "FLOOR");
	/*MeshRenderer* mesh = obj->Component()->AddComponent<MeshRenderer>();
	mesh->ModelHandle(Load::GetHandle(ID::WALL));
	mesh->DrawLocalPosition();*/
	ModelCollider* c = obj->Component()->AddComponent<ModelCollider>();
	CollsionInfo info;
	info.oneColl = false;
	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::MODEL;
	info.tag = CollsionInformation::FLOOR;
	c->ModelColliderSet(info, Transform(VZero, VZero, VOne), MV1DuplicateModel(ResourceLoad::GetHandle(ID::WALL)));
	stage->AddChild(obj, false);
	//}
	
	AABBCollider* aabb = obj->Component()->AddComponent<AABBCollider>();
	info.shape = CollsionInformation::AABB;
	info.tag = CollsionInformation::FLOOR_AABB;
	VECTOR3 minPos = VECTOR3(-WALL_EDGE_POS, -100.0f, -WALL_EDGE_POS);
	aabb->SetUpAABBCollsion(info, minPos, VECTOR3(WALL_EDGE_POS, 100.0f, WALL_EDGE_POS));

	//Transform transform2;
	//transform2 = Transform(CUT_SCENE_POS + VECTOR3(0, -1000, 0), VZero, VOne * VECTOR3(500.0f, 10.0f, 500.0f));

	//Object3D* obj2 = new Object3D();
	//obj2->Init(transform2, "FLOOR2");
	///*MeshRenderer* mesh = obj->Component()->AddComponent<MeshRenderer>();
	//mesh->ModelHandle(Load::GetHandle(ID::WALL));
	//mesh->DrawLocalPosition();*/
	//CollsionInfo info2;
	//info2.oneColl = false;
	//info2.parentTransfrom = obj2->GetTransform();
	//info2.shape = CollsionInformation::MODEL;
	//info2.tag = CollsionInformation::FLOOR;

	//ModelCollider* c2 = obj2->Component()->AddComponent<ModelCollider>();
	//c2->ModelColliderSet(info2, Transform(VZero, VZero, VOne), MV1DuplicateModel(Load::GetHandle(ID::WALL)));
	//stage->AddChild(obj2, false);
}