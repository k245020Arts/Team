#pragma once
#include "CameraStateBase.h"

class TrashEnemyManager;
class Player;

class RangedEnemyCamera :public CameraStateBase
{
public:
	RangedEnemyCamera();
	~RangedEnemyCamera();

	void Update()override;
	void Draw()override;
	void Start()override;
	void Finish()override;

private:
	TrashEnemyManager* tEnemyManager;
	Player* player;

	//カメラがプレイヤーとどんだけ離れるか
	const float CamPos = 700.0f;
	//カメラがどこにいるべきか計算する関数
	void UpdateCamPos(Camera* _camera);
	//カメラの挙動
	void UpdeteCamMove(Camera* _camera);
	//カメラのY軸の挙動
	void CamRotYMove(Camera* _camera);

	//少しずらしたいからプレイヤーに補正をかける数字
	const VECTOR3 PlayerPosOffset = VZero; /*VECTOR3(150.0f, 0.0f, 0.0f);*/

	VECTOR3 enemyPos;
	VECTOR3 pPos;
	VECTOR3 copyCamPos;

	float angle;
	bool isAngleSet;
	bool a;
};