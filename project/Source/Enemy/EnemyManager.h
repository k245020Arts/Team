#pragma once
#include ".././../Library/GameObject.h"
#include <list>
#include "../Camera/CameraInformation.h"

class Object3D;
class BaseObject;
class Camera;
class EnemyBase;
class Transform;
class Boss;

class EnemyManager : public GameObject
{
public:
	EnemyManager();
	~EnemyManager();
	void Update() override;
	void Draw() override;

	std::list<Object3D*> GetEnemy();
	std::list<Object3D*>::iterator GetItr();

	void CreateEnemy();
	void PlayerObjPointer();

	void CreateBoss();

	bool PlayerDistance(Camera* camera);
	bool TargetCancel(Camera* camera);
	void JustAvoidTargetChange(Object3D* _obj);

	void NearEnemyAlpha(VECTOR3 camPos);
	Transform NearEnemyPos(const VECTOR3& _pos);

	EnemyAttackChangeCameraDirection BossAttackCamera(Camera* camera,const Transform& _targetTransform);

private:
	
	std::list<Object3D*> enemy;
	Object3D* player;
	std::list<EnemyBase*> chara;
	BaseObject* cameraTargetObj;
	//std::list<Boss> bossList;
};