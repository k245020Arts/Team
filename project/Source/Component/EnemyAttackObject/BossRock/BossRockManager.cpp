#include "BossRockManager.h"
#include "BossRockBase.h"
#include "BossRock.h"
#include "BossThrowRock.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Component/Shadow/Shadow.h"
#include "../../../Enemy/Boss/Boss.h"
#include "../../../State/StateManager.h"
#include "../../../Common/Easing.h"
#include "../../../Component/Shaker/Shaker.h"
#include "../../../Enemy/Boss/BossState/BossStatus.h"
#include "../../../Camera/Camera.h"
#include "../../../Common/Random.h"
#include "../../MeshRenderer/MeshRenderer.h"
#include "../../../Common/ResourceLoader.h"

BossRockManager::BossRockManager() :  BossRockManager(nullptr)
{
}

BossRockManager::BossRockManager(Boss* _boss)
{
	if (_boss == nullptr) {
		my_error_assert("ボスのポインターが入っていません");
	}
	
	boss = _boss;
	rocks.clear();

	JsonReader json;
	json.Load("data/json/ThrowObject/ThrowObjects.json");
	auto j = json.Data();

	for (const auto& elem : j["ThrowObjectsData"])
	{
		BossThrowObjectData data = elem.get<BossThrowObjectData>();

		if (data.id.empty()) {
			continue;
		}

		throwObjectsData[data.id] = data;
	}
	ResourceLoad::LoadModel("BossAttackRock1",ID::ROCK_1);
	ResourceLoad::LoadModel("BossAttackRock2",ID::ROCK_2);
	ResourceLoad::LoadModel("BossAttackRock3",ID::ROCK_3);
}

BossRockManager::~BossRockManager()
{
	rocks.clear();
}

void BossRockManager::Update()
{


}

void BossRockManager::Draw()
{

}

void BossRockManager::CreateThrowObject(const BossAttackBase::ThrowObjectAttackData& _data, int _index, int _total, float _rotateAngle)
{
	Object3D* throwObject = new Object3D();
	SetRockComponent(throwObject, _data.baseGravity, _data.baseFirction, _data);
	if (_data.throwToFallToPlayer || _data.throwToFall) {
		VECTOR3 center = VZero;
		VECTOR3 dir = VZero;
		if (_data.throwToFallToPlayer) {
			auto playerObj = boss->enemyBaseComponent.playerObj;
			VECTOR3 playerPos = playerObj->GetTransform()->position;

			Physics* playerPhy = playerObj->Component()->GetComponent<Physics>();

			VECTOR3 playerVel = VZero;
			if (playerPhy != nullptr) {
				playerVel = playerPhy->GetVelocity();
			}

			float futureTime = Random::GetFloat(0.3f, 0.8f);
			//予測値	
			VECTOR3 futurePos = playerPos + playerVel * futureTime;

			center = Easing::Lerp(playerPos, futurePos, 0.9f);
		}
		else {
			center = _data.thorwStartPos;
		}
		dir.y = 0;
		dir.Normalize();
		Camera* cam = boss->enemyBaseComponent.camera;
		VECTOR3 camForward = cam->GetCameraTransform()->Forward();
		VECTOR3 camRight = camForward * MGetRotY(90.0f * DegToRad);

		float limitAngle = 180.0f * DegToRad * 0.45f;

		float baseAngle = (2.0f * 180.0f * DegToRad / _total) * _index;
		float ramdom = Random::GetFloat(-0.25f, 0.25f);

		float finalAngle = std::clamp(baseAngle + _rotateAngle + ramdom, -limitAngle, limitAngle);

		dir = camForward * cosf(finalAngle) + camRight * sinf(finalAngle);

		float wave = sinf(_index * 0.8f) * 350.0f;
		float radius = 1000.0f + wave + Random::GetFloat(-200.0f, 200.0f);
		VECTOR3 startPos = center + dir * radius;
		if (_data.randomHeight) {
			//VECTOR3 startPos = center + dir * radius;
			//8000,12000
			float offset = Random::GetFloat(_data.minHeight, _data.maxHeight);
			startPos.y = center.y + offset;
		}
		

		VECTOR3 rotation = VZero;
		//rotation.z = 45.0f * DegToRad;
		throwObject->GetTransform()->position = startPos;
		throwObject->GetTransform()->rotation = rotation;

		VECTOR3 gravity = VECTOR3(0, _data.throwFallGravity, 0);
		if (_data.randomSpeed) {
			//1500,3000
			gravity.y = Random::GetFloat(_data.minSpeed, _data.maxSpeed);
			gravity.y *= -1.0f;
		}

		Physics* phy = throwObject->Component()->GetComponent<Physics>();
		phy->SetGravity(gravity);
	}
	else if (_data.freeDir) {
		throwObject->GetTransform()->position = _data.thorwStartPos;
	}
	else if (_data.armThrow) {
		Physics* phy = throwObject->Component()->GetComponent<Physics>();
		phy->Start(VZero, VZero);
	}
	if (_data.randomBlast) {
		bool doBlast = true;
		//0.1
		if (Random::GetFloat(0.0f, 1.0f) < _data.randomBlastRate)
		{
			doBlast = false;
		}

		BossRockBase* rockComp = throwObject->Component()->GetComponent<BossRockBase>();
		rockComp->SetCanBlast(doBlast);
	}
	/*else if (_data.blastCan) {
		BossRockBase* rockComp = throwObject->Component()->GetComponent<BossRockBase>();
		rockComp->SetCanBlast(true);
	}*/
	BossRockBase* rock = throwObject->Component()->GetComponent<BossRockBase>();
	rock->Start(_data);
}

VECTOR3 BossRockManager::GetPushCollSize(BossAttackBase::ThrowObjectAttackData _data)
{
	return throwObjectsData[_data.throwObjectID].pushTransform.scale;
}

void BossRockManager::CreateRock(int _index,int _total,float _rotateAngle)
{
    Object3D* rock = new Object3D();

   

	//斜め
	/*float angleZ = 45.0f * DegToRad;
	VECTOR3 normal = VECTOR3(cosf(angleZ), 0, sinf(angleZ));
	normal.Normalize();

	float speed = 6000.0f;
	rock->Component()->GetComponent<Physics>()->AddGravity(speed * normal);*/
}

void BossRockManager::CreateLastRock()
{
	//Object3D* rock = new Object3D();
	//// プレイヤー現在位置取得
	//VECTOR3 playerPos = boss->enemyBaseComponent.playerObj->GetTransform()->position;
	//VECTOR3 rockPos = playerPos + VECTOR3(0.0f, 15000.0f,0.0f);
	//rock->Init(Transform(rockPos, VECTOR3(0.0f, 0.0f, 0.0f), VOne * 3.0f), "bossRock");
	//SetRockComponent(rock, VECTOR3(0, -3000, 0), VECTOR3(0, 1500, 0));
}

void BossRockManager::PushList(BossRockBase* _obj)
{
	rocks.push_back(_obj);
}

void BossRockManager::RemoveList(BossRockBase* _obj)
{
	for (auto itr = rocks.begin(); itr != rocks.end();) {
		if (*itr == _obj) {
			rocks.erase(itr);
			return;
		}
		else {
			itr++;
		}
	}
}

bool BossRockManager::IsFreePos(const VECTOR3& _pos, float _minDist)
{
	for (auto rock : rocks) {
		VECTOR3 diff = rock->GetBaseObject()->GetTransform()->position - _pos;
		if (diff.Size() < _minDist) {
			return false;
		}
	}
	return true;
}

void BossRockManager::ShakeCamera()
{
	boss->enemyBaseComponent.camera->CameraPerspectiveShakeStart(2.0f, 0.1f);
}

void BossRockManager::ThrowStart()
{
	for (auto rock : rocks) {
		if (!rock->GetThrowObjectsData().armThrow) {
			continue;
		}
		rock->ThrowRockStart(boss->enemyBaseComponent.playerObj);
	}

}

void BossRockManager::CreateThrow(VECTOR3& _addPos)
{
	//Object3D* rock = new Object3D();
	//rock->Init(Transform(), "bossRock");
	//BossThrowRock* throwRock = rock->Component()->AddComponent<BossThrowRock>();
	//Physics* phy = rock->Component()->AddComponent<Physics>();
	//Shaker* shaker = rock->Component()->AddComponent<Shaker>();
	//
	//phy->Start(VZero, VZero);//
	//throwRock->CreateThrowRock(_addPos);

	//throwRock->SetRockModel();
	//boss->obj->AddChild(rock, false);
}

void BossRockManager::DropRockStart()
{
	for (auto rock : rocks) {
		if (!rock->GetThrowObjectsData().playerAttackObjectDrop) {
			continue;
		}
		rock->DropObject();
	}
}

VECTOR3 BossRockManager::GetRockPos(VECTOR3 _pos)
{
	VECTOR3 pos = VECTOR3(INFINITY, INFINITY, INFINITY);

	for (auto rock : rocks)
	{
		VECTOR3 vec = rock->GetBaseObject()->GetTransform()->position - _pos;
		if (pos.Size() > vec.Size())
			pos = rock->GetBaseObject()->GetTransform()->position;
	}

	return pos;
}

std::list<VECTOR3> BossRockManager::GetAllRockPos()
{
	std::list<VECTOR3> _pos;

	for (auto rock : rocks)
		_pos.push_back(rock->GetBaseObject()->GetTransform()->position);
	return _pos;
}

void BossRockManager::SetRockComponent(Object3D* _base, const VECTOR3& _gravity, const VECTOR3& _fir, const BossAttackBase::ThrowObjectAttackData& _data)
{
	_base->Init(Transform(),"bossThrowObject");
	BossRockBase* bossRock = _base->Component()->AddComponent<BossRockBase>();
	
	Physics* phy = _base->Component()->AddComponent<Physics>();
	phy->Start(_gravity, _fir);
	//bossRock->StartCollAdd(CollsionInformation::BOSS_ROCK_ATTACK, Transform());;
	Shaker* shaker = _base->Component()->AddComponent<Shaker>();
	//bossRock->SetRockModel();
	MeshRenderer* mesh = _base->Component()->AddComponent<MeshRenderer>();
	mesh->ModelHandle(ResourceLoad::GetHandle(ID::StringToID(throwObjectsData[_data.throwObjectID].modelName)));
	_base->GetTransform()->scale = throwObjectsData[_data.throwObjectID].modelTransform.scale;
	//Shadow* shadow = _base->Component()->AddComponent<Shadow>();
	/*Object3D* shadow = new Object3D();
	shadow->Init(Transform(VECTOR3(0.0f, -20.0f, 0.0f), VZero, VECTOR3(_base->GetTransform()->scale.x + 4.0f, 0.1f, _base->GetTransform()->scale.z + 4.0f)), "BossShadow");
	Shadow* s = shadow->Component()->AddComponent<Shadow>();
	s->Start();*/
	boss->obj->AddChild(_base, false);
}

//void BossRockManager::SetRockComponent(BaseObject* _base, const VECTOR3& _gravity, const VECTOR3& _fir)
//{
//	
//}