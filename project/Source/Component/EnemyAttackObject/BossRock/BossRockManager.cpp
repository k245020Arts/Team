#include "BossRockManager.h"
#include "BossRock.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Component/Shadow/Shadow.h"
#include "../../../Enemy/Boss/Boss.h"
#include "../../../State/StateManager.h"
#include "../../../Common/Easing.h"
#include "../../../Enemy/Boss/BossState/BossStatus.h"
#include "../../../Camera/Camera.h"

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

void BossRockManager::CreateRock(int _createNum)
{
	//for (int i = 0; i < _createNum; i++) {
		Object3D* rock = new Object3D();
		// プレイヤー現在位置取得
		VECTOR3 playerPos = boss->enemyBaseComponent.playerObj->GetTransform()->position;
		// 岩の落下位置を円形にランダムにずらす
		VECTOR3 rockPos;
		int num = 0;
		do {
			float angle = 180.0f * DegToRad * 2.0f * GetRand(1000) / 1000.0f;
			float radius = 1800.0f;
			rockPos = playerPos + VECTOR3(cosf(angle) * radius, 8000.0f, sinf(angle) * radius);
			num++;
		} while (!IsFreePos(rockPos, 300.0f) && num < 20);
		rock->Init(Transform(rockPos, VECTOR3(0.0f, 0.0f, 0.0f), VOne * 1.0f), "bossRock");
		SetRockComponent(rock, VECTOR3(0, -2000, 0), VECTOR3(0, 1500, 0));
		
	//}
}

void BossRockManager::CreateLastRock()
{
	Object3D* rock = new Object3D();
	// プレイヤー現在位置取得
	VECTOR3 playerPos = boss->enemyBaseComponent.playerObj->GetTransform()->position;
	VECTOR3 rockPos = playerPos + VECTOR3(0.0f, 15000.0f,0.0f);
	rock->Init(Transform(rockPos, VECTOR3(0.0f, 0.0f, 0.0f), VOne * 3.0f), "bossRock");
	SetRockComponent(rock, VECTOR3(0, -3000, 0), VECTOR3(0, 1500, 0));
}

void BossRockManager::PushList(BossRock* _obj)
{
	rocks.push_back(_obj);
}

void BossRockManager::RemoveList(BossRock* _obj)
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

void BossRockManager::SetRockComponent(BaseObject* _base, const VECTOR3& _gravity, const VECTOR3& _fir)
{
	BossRock* bossRock = _base->Component()->AddComponent<BossRock>();
	Physics* phy = _base->Component()->AddComponent<Physics>();
	phy->Start(_gravity,_fir);
	bossRock->StartCollAdd(CollsionInformation::BOSS_ROCK_ATTACK, Transform());;

	bossRock->SetRockModel();
	//Shadow* shadow = _base->Component()->AddComponent<Shadow>();
	/*Object3D* shadow = new Object3D();
	shadow->Init(Transform(VECTOR3(0.0f, -20.0f, 0.0f), VZero, VECTOR3(_base->GetTransform()->scale.x + 4.0f, 0.1f, _base->GetTransform()->scale.z + 4.0f)), "BossShadow");
	Shadow* s = shadow->Component()->AddComponent<Shadow>();
	s->Start();*/
	boss->obj->AddChild(_base, false);
}