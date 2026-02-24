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
#include "../../../Common/Random.h"

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
void BossRockManager::CreateRock(int _index,int _total,float _rotateAngle)
{
    Object3D* rock = new Object3D();

    auto playerObj = boss->enemyBaseComponent.playerObj;
    VECTOR3 playerPos = playerObj->GetTransform()->position;

    Physics* playerPhy =  playerObj->Component()->GetComponent<Physics>();

    VECTOR3 playerVel = VZero;
	if (playerPhy != nullptr) {
		playerVel = playerPhy->GetVelocity();
	}

    float futureTime = Random::GetFloat(0.3f, 0.8f);
	//予測値	
    VECTOR3 futurePos = playerPos + playerVel * futureTime;

    VECTOR3 center = Easing::Lerp(playerPos, futurePos, 0.9f);

    Camera* cam = boss->enemyBaseComponent.camera;
    VECTOR3 camForward = cam->GetCameraTransform()->Forward();
    VECTOR3 camRight = camForward * MGetRotY(90.0f * DegToRad);

    float limitAngle = 180.0f * DegToRad * 0.45f;

    float baseAngle = (2.0f * 180.0f * DegToRad / _total) * _index;
    float ramdom = Random::GetFloat(-0.25f, 0.25f);

    float finalAngle = std::clamp(baseAngle + _rotateAngle + ramdom,-limitAngle, limitAngle);

    VECTOR3 dir = camForward * cosf(finalAngle) + camRight * sinf(finalAngle);

    dir.y = 0;
    dir.Normalize();

    float wave = sinf(_index * 0.8f) * 350.0f;
    float radius = 1000.0f + wave + Random::GetFloat(-200.0f, 200.0f);

    VECTOR3 startPos = center + dir * radius;
	float offset = Random::GetFloat(8000.0f, 12000.0f);
    startPos.y = center.y + offset;

	VECTOR3 rotation = VZero;
	rotation.z = 45.0f * DegToRad;

    rock->Init(Transform(startPos, rotation, VOne),"bossRock");

    VECTOR3 gravity = VECTOR3(0, -2000.0f, 0);
	gravity.y = Random::GetFloat(1500.0f, 3000.0f);
	gravity.y *= -1.0f;

    SetRockComponent(rock,gravity,VZero);

	bool doBlast = true;
	if (Random::GetFloat(0.0f, 1.0f) < 0.1f)
	{
		doBlast = false;
	}

	BossRock* rockComp = rock->Component()->GetComponent<BossRock>();
	rockComp->SetCanBlast(doBlast);

	//斜め
	/*float angleZ = 45.0f * DegToRad;
	VECTOR3 normal = VECTOR3(cosf(angleZ), 0, sinf(angleZ));
	normal.Normalize();

	float speed = 6000.0f;
	rock->Component()->GetComponent<Physics>()->AddGravity(speed * normal);*/
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