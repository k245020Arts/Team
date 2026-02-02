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
class GameManager;

class EnemyManager : public GameObject
{
public:
	EnemyManager();
	~EnemyManager();
	void Update() override;
	void Draw() override;

	void DebugDrawCamera(Camera* camera);

	void CreateEnemy();
	void PlayerObjPointer();

	void CreateBoss();
	void FindPlayer();

	void AddList(EnemyBase* _enemy, BaseObject* _obj);

	void RemoveList(EnemyBase* _enemy, BaseObject* _obj);

	bool ChangeCameraRockOn(Camera* camera, bool _right);
	bool ChangeCameraRockOn(Camera* camera, bool _right, bool _min);
	bool ChangeCameraRockOn(Camera* camera,bool _right, bool _min,bool _debugMode);
	bool TargetCancel(Camera* camera);
	void JustAvoidTargetChange(BaseObject* _obj);

	EnemyBase* PlayerNearEnemy();

	void NearEnemyAlpha(VECTOR3 camPos);
	Transform NearEnemyPos(const VECTOR3& _pos);

	void SetCameraRockOnObject(EnemyBase* _enemy, Camera* _camera);
	void CameraRockOnStart(Camera* _camera);

	EnemyAttackChangeCameraDirection BossAttackCamera(Camera* camera,const Transform& _targetTransform);
	/// <summary>
	/// Enemy‚·‚×‚Ä‚ªƒJƒƒ‰‚ª“ü‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©‚ğ’²‚×‚é
	/// </summary>
	/// <returns></returns>
	bool CameraInEnemy();

	void CanPlayerSpecialHit();

	void SleepAllEnemy(bool _sleep);

	void GameSceneChangeState();

private:
	
	std::list<BaseObject*> enemy;
	Object3D* player;
	std::list<EnemyBase*> chara;
	BaseObject* cameraTargetObj;
	//std::list<Boss> bossList;
	GameManager* gameManager;
};