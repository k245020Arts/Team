#include "stageManager.h"
#include "../Component/Component.h"
#include "../Component/MeshRenderer/MeshRenderer.h"
#include "stage.h"
#include "../Component/ComponentManager.h"
#include "../Common/LoadManager.h"
#include "Wall.h"
#include "../Component/Collider/ModelCollider.h"
#include "../Player/Player.h"
#include "../Player/PlayerState/PlayerStateManager.h"
#include "../Component/Object/Object3D.h"

//#define NEW_STAGE

StageManager::StageManager()
{
	//modelHandle = MV1LoadModel("data/model/stage10.mv1");
	//CreateStage();
#ifdef NEW_STAGE
	Load::LoadModel(Load::MODEL_PATH + "stage_s", ID::S_MODEL);
#else
	Load::LoadModel(Load::MODEL_PATH +  "new_Stage6", ID::S_MODEL);
#endif // NEW_STAGE

	
	Load::LoadModel(Load::MODEL_PATH + "cube", ID::WALL);
	stage = nullptr;
	modelHandle = -1;
}

StageManager::~StageManager()
{
	//MV1DeleteModel(modelHandle);
}

void StageManager::Update()
{

}

void StageManager::Draw()
{
}

void StageManager::CreateStage()
{
	
	stage = new Object3D();
#ifdef NEW_STAGE
	stage->Init(VECTOR3(25000, -5000, 20000), VZero, VECTOR3(10, 10, 10), "STAGE");
#else
	stage->Init(VECTOR3(100, -50, 100), VZero, VECTOR3(10, 10, 10), "STAGE");
#endif
	
	
	MeshRenderer* mesh = stage->Component()->AddComponent<MeshRenderer>();
	mesh->ModelHandle(Load::GetHandle(ID::S_MODEL));
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
		Transform transform;
		switch (i)
		{
		case 0:
			transform = Transform(VECTOR3(10000, 100, 100), VZero, VOne* VECTOR3(4.0f, 50, 100.0f));
			break;
		case 1:
			transform = Transform(VECTOR3(-10000, 100, 100), VZero, VOne * VECTOR3(4.0f, 50, 100.0f));
			break;
		case 2:
			transform = Transform(VECTOR3(0, 100, -10000), VZero, VOne * VECTOR3(100.0f, 50, 4.0f));
			break;
		case 3:
			transform = Transform(VECTOR3(0, 100, 10000), VZero, VOne * VECTOR3(100.0f, 50, 4.0f));
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
		wall->ModelSet(Load::GetHandle(ID::WALL));
		ModelCollider* c = obj->Component()->AddComponent<ModelCollider>();
		CollsionInfo info;
		info.oneColl = false;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::MODEL;
		info.tag = CollsionInformation::WALL;
		c->ModelColliderSet(info, Transform(VZero, VZero, VOne), MV1DuplicateModel( Load::GetHandle(ID::WALL)));
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
	Transform transform;
	transform = Transform(VECTOR3(0, -1000, 0), VZero, VOne * VECTOR3(500.0f, 10.0f, 500.0f));

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
	c->ModelColliderSet(info, Transform(VZero, VZero, VOne), MV1DuplicateModel(Load::GetHandle(ID::WALL)));
	stage->AddChild(obj, false);
}