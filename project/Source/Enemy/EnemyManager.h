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

	std::list<BaseObject*> GetEnemy();
	std::list<BaseObject*>::iterator GetItr();

	void CreateEnemy();
	void PlayerObjPointer();

	void CreateBoss();
	void FindPlayer();

	void AddList(EnemyBase* _enemy, BaseObject* _obj);

	void RemoveList(EnemyBase* _enemy, BaseObject* _obj);

	bool PlayerDistance(Camera* camera);
	bool TargetCancel(Camera* camera);
	void JustAvoidTargetChange(BaseObject* _obj);

	void NearEnemyAlpha(VECTOR3 camPos);
	Transform NearEnemyPos(const VECTOR3& _pos);

	EnemyAttackChangeCameraDirection BossAttackCamera(Camera* camera,const Transform& _targetTransform);

private:
	
	std::list<BaseObject*> enemy;
	Object3D* player;
	std::list<EnemyBase*> chara;
	BaseObject* cameraTargetObj;
	//std::list<Boss> bossList;
};