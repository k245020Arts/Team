#include "EnemyManager.h"
#include "../Component/MeshRenderer/MeshRenderer.h"
#include "../Component/Transform/Transform.h"
#include "../Component/MeshRenderer2D/MeshRenderer2D.h"
#include "../Player/Player.h"
#include "../Component/Physics/Physics.h"
#include "../Camera/Camera.h"
#include "../Component/Hierarchy/Hierarchy.h"
#include "../Common/LoadManager.h"
#include "../Component/Collider/sphereCollider.h"
#include "../Component/Animator/Animator.h"
#include "TrashEnemy/enemy.h"
#include "../Weapon/WeaponManager.h"
#include "../Component/Shaker/Shaker.h"
#include "../Component/Object/Object2D.h"
#include "../Component/Guage/Guage.h"
#include "Boss/Boss.h"
#include "../Component/Collider/rayCollider.h"
#include "../Component/Collider/ModelCollider.h"
#include "../Component/Shadow/Shadow.h"
#include "../Common/Easing.h"

//#define VERSION2D
//#define DOT_MODE

EnemyManager::EnemyManager()
{
	//CreateEnemy();
	enemy.clear();
	player = nullptr;
	SetDrawOrder(-10);
	cameraTargetObj = nullptr;

}

EnemyManager::~EnemyManager()
{
	enemy.clear();
	chara.clear();
}

void EnemyManager::Update()
{
	
}

void EnemyManager::Draw()
{
}

void EnemyManager::DebugDrawCamera(Camera* camera)
{
	
}

std::list<BaseObject*> EnemyManager::GetEnemy()
{
	return enemy;
}

std::list<BaseObject*>::iterator EnemyManager::GetItr()
{
	auto e = enemy.begin();
	return e;
}

void EnemyManager::CreateEnemy()
{
	
	//e->Start(enemy);
	/*chara.emplace_back(en);
	
	enemy.emplace_back(e);*/
}

void EnemyManager::PlayerObjPointer()
{
	
	for (auto itr = enemy.begin(); itr != enemy.end(); itr++) {
		Enemy* e = (*itr)->Component()->GetComponent<Enemy>();
		//e->PlayerPointerSet(obj);
		//e->Start((*itr));
		(*itr)->SetDrawOrder(-5);

		Object2D* guage = new Object2D();

		guage->Init(VECTOR2F(970, 0), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 0.5f), "enemyHpGuage");

		(*itr)->AddChild(guage);

		Guage* g = guage->Component()->AddComponent<Guage>();
		g->GuageDrawReady<Enemy>(Load::LoadImageGraph(Load::IMAGE_PATH + "playerHpGuage", ID::PLAYER_HP_GUAGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F, Guage::BAR_MODE::HP);
	}
	
	player->Component()->GetComponent<Player>()->TargetObjSet(*enemy.begin());
	player->Component()->GetComponent<Player>()->HitObjectSet(*enemy.begin());
	FindGameObjectWithTag<Object3D>("CAMERA_OBJ")->Component()->GetComponent<Camera>()->TargetSet(*enemy.begin());
}

void EnemyManager::CreateBoss()
{
	/*Object3D* obj = FindGameObjectWithTag<Object3D>("PLAYER");
	player = obj;*/
	Object3D* boss;
	boss = new Object3D();
	float bSize = 8.0f;
	boss->Init(Transform(VECTOR3(0, 450, 2000), VZero, VECTOR3(bSize, bSize - 1, bSize)), "Boss");

	SphereCollider* collider = boss->Component()->AddComponent<SphereCollider>();
	CollsionInfo info;
	info.parentTransfrom = boss->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = false;
	info.tag = CollsionInformation::Tag::BOSS;
	info.size = 1.0f;
	collider->CollsionAdd(info, Transform(VECTOR3(0, 50, 0), VZero, VECTOR3(350.0f, 1.0f, 1.0f)));
	//当たり判定を生成（やられ判定）
	/*SphereCollider* collider2 = boss->Component()->AddComponent<SphereCollider>();
	CollsionInfo info2;
	info2.parentTransfrom = boss->GetTransform();
	info2.shape = CollsionInformation::SPHERE;
	info2.oneColl = false;
	info2.tag = CollsionInformation::Tag::BOSS;
	info.size = 1.0f;
	collider2->CollsionAdd(info2, Transform(VECTOR3(0, 150, 0), VZero, VECTOR3(250.0f, 1.0f, 1.0f)));*/
	RayCollider* collider3 = boss->Component()->AddComponent<RayCollider>();
	info.shape = CollsionInformation::RAY;
	info.tag = CollsionInformation::B_FLOOR;
	collider3->RaySet(info, Transform(VECTOR3(0, 200, 0), VZero, VECTOR3(1.0f, 10.0, 1.0)), Transform(VECTOR3(0, -0, 0), VZero, VECTOR3(1.0f, 1, 1)));

	ModelCollider* collider4 = boss->Component()->AddComponent<ModelCollider>();
	info.shape = CollsionInformation::MODEL;
	info.tag = CollsionInformation::BOSS;
	collider4->ModelColliderSet(info, Transform(VECTOR3(0, 50, 0), VZero, VECTOR3(0.35f, 1.0f, 0.35f)), Load::LoadModel(Load::MODEL_PATH + "wall", ID::BOSS_PUSH));


	Shaker* shaker = boss->Component()->AddComponent<Shaker>();

	MeshRenderer* m = boss->Component()->AddComponent<MeshRenderer>();
	m->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "BossModel", ID::B_MODEL));
	m->RotationMesh(0, 180.0f * DegToRad);

	Boss* b = boss->Component()->AddComponent<Boss>();

	Physics* physics = boss->Component()->AddComponent<Physics>();
	physics->Start(VECTOR3(0.0f, -1500.0f, 0.0f), BossInformation::BASE_FIRCTION);

	Animator* anim = boss->Component()->AddComponent<Animator>();
	anim->BaseModelSet(Load::GetHandle(ID::B_MODEL), 1);

	anim->AddFile(ID::B_IDOL, "B_IDLE", true, 1.0f);
	anim->AddFile(ID::B_COOLTIME, "B_IDLE2", true, 1.0f);
	anim->AddFile(ID::B_RUN, "B_WALK", true, 1.2f, 30.0f, 45.0f);
	anim->AddFile(ID::B_N_ATTACK1, "B_ATTACK1", false, 1.2f, 25.0f, 60.0f);
	anim->AddFile(ID::B_N_ATTACK2, "B_ATTACK2", false, 1.2f, 25.0f, 60.0f);
	anim->AddFile(ID::B_N_ATTACK3, "B_ATTACK3", false, 1.2f, 40.0f, 55.0f);
	anim->AddFile(ID::B_N_ATTACK4, "B_ATTACK4", false, 1.2f, 20.0f, 55.0f);
	anim->AddFile(ID::B_N_ATTACK5, "B_ATTACK5", false, 1.2f, 20.0f, 55.0f);
	anim->AddFile(ID::B_N_ATTACK6, "B_ATTACK6", false, 1.2f, 50.0f, 65.0f);
	anim->AddFile(ID::B_S_ATTACK1, "B_SATTACK1", false, 1.0f, 50.0f, 60.0f);
	anim->AddFile(ID::B_S_ATTACK2, "B_SATTACK2", true, 1.0f, 0.0f, 100.0f);
	anim->AddFile(ID::B_S_ATTACK2_STOP, "B_SATTACK2_STOP", false, 1.0f);
	anim->AddFile(ID::B_S_ATTACK2_BEFORE, "B_SATTACK2_BEFORE", false, 1.0f, 0.0f, 100.0f);
	anim->AddFile(ID::B_S_ATTACK1_SMALL, "B_SATTACK1_SAMLL", false, 1.3f, 40.0f, 55.0f);
	anim->AddFile(ID::BOSS_DIE, "B_DIE", false, 1.2f);
	anim->AddFile(ID::B_ROAR, "B_ROAR", false, 1.2f, 30.0f, 40.0f);
	anim->AddFile(ID::B_DUSH, "B_RUN", true, 1.2f, 30.0f, 40.0f);
	anim->AddFile(ID::B_THREAT, "B_THREAT", false, 1.2f, 10.0f, 70.0f);
	anim->AddFile(ID::BOSS_DAMAGE, "B_DAMAGE", false, 1.8f, 10.0f, 70.0f);
	//anim->SetMaxFrame(ID::B_N_ATTACK1, 50.0f);


	b->Start(boss);

	Object2D* guage = new Object2D();

	guage->Init(VECTOR2F(915.0f, 70.0f), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 1.0f), "Boss_HpBar_RedBack");

	boss->AddChild(guage);

	Guage* g = guage->Component()->AddComponent<Guage>();
	g->GuageDrawReady<Boss>(Load::LoadImageGraph(Load::IMAGE_PATH + "Boss_HpBar_RedBack", ID::PLAYER_HP_GUAGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F, Guage::BAR_MODE::HP);
	g->EdgeDrawReady(Load::LoadImageGraph(Load::IMAGE_PATH + "Boss_HpBar_Frame", ID::BOSS_HP_EDGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F, Transform(VECTOR3(915.0f, 70.0f, 0.0f), VZero, VECTOR3(1.0f, 1.0f, 0.0f)));
	
	
	//bossList.emplace_back(b);

	Object3D* shadow = new Object3D();
	shadow->Init(Transform(VECTOR3(0.0f, -20.0f, 0.0f), VZero, VECTOR3(boss->GetTransform()->scale.x - 2.0f, 0.1f, boss->GetTransform()->scale.z - 2.0f)), "BossShadow");
	Shadow* s = shadow->Component()->AddComponent<Shadow>();
	s->Start();

	RayCollider* collider5 = shadow->Component()->AddComponent<RayCollider>();
	info.shape = CollsionInformation::RAY;
	info.tag = CollsionInformation::SHADOW;
	collider5->RaySet(info, Transform(VECTOR3(0, 50, 0), VZero, VECTOR3(1.0f, 1.0, 1.0)), Transform(VECTOR3(0, -s->GetMaxDist(), 0), VZero, VECTOR3(1.0f, 1, 1)));
	boss->AddChild(shadow);

	player->Component()->GetComponent<Player>()->TargetObjSet(*enemy.begin());
	player->Component()->GetComponent<Player>()->HitObjectSet(*enemy.begin());
	//FindGameObjectWithTag<Object3D>("CAMERA_OBJ")->Component()->GetComponent<Camera>()->TargetSet(*enemy.begin());
}

void EnemyManager::FindPlayer()
{
	Object3D* obj = FindGameObjectWithTag<Object3D>("PLAYER");
	player = obj;
}

void EnemyManager::AddList(EnemyBase* _enemy, BaseObject* _obj)
{
	chara.emplace_back(_enemy);
	enemy.emplace_back(_obj);
}

void EnemyManager::RemoveList(EnemyBase* _enemy, BaseObject* _obj)
{

	//死んでた時にカメラのロックオンされた敵がいたのならロックオンを解除
	if (cameraTargetObj != nullptr) {
		if (_obj == cameraTargetObj) {
			TargetCancel(nullptr);
		}
	}

	for (auto itr = chara.begin(); itr != chara.end();) {
		if (*itr == _enemy) {
			itr = chara.erase(itr);
		}
		else {
			itr++;
		}
	}

	
	for (auto itrObj = enemy.begin(); itrObj != enemy.end();) {
		if (*itrObj == _obj) {
			itrObj = enemy.erase(itrObj);
		}
		else {
			itrObj++;
		}
	}
	
}

bool EnemyManager::ChangeCameraRockOn(Camera* camera,bool _right)
{
	return ChangeCameraRockOn(camera, _right, true,false);
}

bool EnemyManager::ChangeCameraRockOn(Camera* camera, bool _right,bool _min)
{
	return ChangeCameraRockOn(camera, _right, _min, false);
}

bool EnemyManager::ChangeCameraRockOn(Camera* camera, bool _right, bool _min, bool _debugMode)
{
	if (chara.size() <= 1) {
		return false;
	}
	const float CAM_LONG_DISTANCE = 100000.0f;
	float distance = 1000000;
	VECTOR3 dir = camera->GetTarget() - camera->GetCameraTransform()->position;
	VECTOR3 camPos = camera->GetCameraTransform()->position;
	VECTOR3 camFront = camera->GetCameraTransform()->Forward();
	dir = dir.Normalize();
	auto keepItr = chara.end();
	auto lastItr = chara.end();
	float invertMinMax = 0.0f;
	if (_min) {
		invertMinMax = 1000.0f;;
	}
	else {
		invertMinMax = -1000.0f;;
	}
	float dist2DMax = 10000.0f;
	auto itrs = chara.begin();
	
	float dotMax = 0.0f;
	for (; itrs != chara.end(); itrs++) {
		if ((*itrs)->GetLastTarget()) {
			lastItr = itrs; //最後にロックオンされていた敵のイテレーター
			break;
		}
	}
	VECTOR3 tarPos;
	VECTOR3 fowardPos;
	VECTOR3 lastPos2D;
	//これまでターゲットに入ってなかったらプレイヤーの位置を参照
	if (itrs == chara.end()) {
		tarPos = player->GetTransform()->position;
		fowardPos = player->GetTransform()->Forward();

	}
	else {
		tarPos = (*lastItr)->GetBaseObject()->GetTransform()->position;
		fowardPos = (*lastItr)->GetBaseObject()->GetTransform()->Forward();

		/*tarPos = player->GetTransform()->position;
		fowardPos = player->GetTransform()->Forward();*/
	}
	VECTOR3 targetEnemyPos = tarPos;

	VECTOR3 targetFrontPos = camFront;
	lastPos2D = ConvWorldPosToScreenPos(targetEnemyPos);
	VECTOR3 camDir;
	if (_right) {
		camDir = targetFrontPos * MGetRotY(90.0f * DegToRad);
	}
	else {
		camDir = targetFrontPos * MGetRotY(-90.0f * DegToRad);
	}

	int num = 0;
	if (!_debugMode) {
		int b = 0;
	}
	//一番近い敵を検出
	for (auto itr = chara.begin(); itr != chara.end(); itr++) {
		if ((*itr)->GetLastTarget()) {
			lastItr = itr; //前にロックオンされていた敵のイテレーター
			num++;
			continue;
		}
		VECTOR3 enemyPos = (*itr)->GetBaseObject()->GetTransform()->position;

		VECTOR3 enemyFrontPos = enemyPos * (*itr)->GetBaseObject()->GetTransform()->Forward();

		camPos.y = 0.0f;
		enemyPos.y = 0.0f;

		VECTOR3 enemyTarget = enemyPos - camPos;

		//DrawLine3D(camPos, enemyPos, 0xffffff);

		//Enemy側から見た右はプレイヤーに対してだと左側になるのでEnemyの左側を判定する
		//VECTOR3 enemyRight = enemyPos * VECTOR3(1, 0, 0);

		camDir.y = 0;
		enemyTarget.y = 0;


		//targetDir.z = 0.0f;

		float camDot = VDot(camDir, enemyTarget);
		//右側か左側の指定した方向に対して0以上なら通るようにする
		if (camDot < 0.0f * DegToRad) {
			num++;
			continue;
		}
#ifdef VERSION2D
		//VECTOR3 target = camPos - enemyPos;
		////より右側に近い敵を判定するために内積を取る。
		//target.y = 0.0f;
		//camFront.y = 0.0f;

		//DOTO 右ベクトル的に一番短い敵のポジションを判定
		/*VECTOR3 target = enemyPos - targetEnemyPos;
		target.y = 0;*/

		VECTOR3 pos2D = ConvWorldPosToScreenPos(enemyPos);

		float dist2D = lastPos2D.x - pos2D.x;

		DrawLine3D(targetEnemyPos, enemyPos, 0xff000f);

		/*VECTOR3 sideDir = targetDir.Normalize();*/

		/*float sideDist = VDot(target, sideDir);*/

		float dist = VECTOR3(camPos - targetEnemyPos).Size();
		//距離が一定以上あったら無視
		if (dist >= 100000) {
			continue;
		}
		distance = dist;
		if (fabs(dist2D) < dist2DMax) {
			dist2DMax = fabs(dist2D);
			keepItr = itr;
		}

		#elseifdef DOT_MODE

			VECTOR3 dotEnemyPos = enemyPos - camPos;
		dotEnemyPos.y = 0.0f;
		camFront.y = 0;
		DrawLine3D(camPos, enemyPos, 0xff000f);
		float dist = VECTOR3(camPos - targetEnemyPos).Size();

		float dot = VDot(camFront.Normalize(), dotEnemyPos.Normalize());
		if (dist >= 100000) {
			continue;
		}
		distance = dist;

		if (dot > dotMax) {
			dotMax = dot;
			keepItr = itr;
		}
#else

		float score = 0.0f;



		VECTOR3 dotEnemyPos = enemyPos - camPos;
		dotEnemyPos.y = 0.0f;
		camFront.y = 0;
		//DrawLine3D(camPos, enemyPos, 0xff000f);

		float frontDot = VDot(camFront.Normalize(), dotEnemyPos.Normalize());
		float frontScore = (1.0f - frontDot) * 1.0f;


		float dist = VECTOR3(camPos - targetEnemyPos).Size();

		float sideDot = VDot(camDir.Normalize(), dotEnemyPos.Normalize());
		float sideScore = sideDot * 0.5f;

		float distScore = (dist / CAM_LONG_DISTANCE) * 0.3f;

		score = frontScore + sideScore + distScore;

		if (_min) {
			if (score < invertMinMax) {
				invertMinMax = score;
				keepItr = itr;
			}
		}
		else {
			if (score > invertMinMax) {
				invertMinMax = score;
				keepItr = itr;
			}
		}
		
		//VECTOR3 pos2D = ConvWorldPosToScreenPos(enemyPos);
		//DrawFormatString(pos2D.x, pos2D.y,0xffffff,"front: %.2f  side: %.2f  dist: %.2f  total: %.2f",frontScore, sideScore, distScore, score);
#endif 
		num++;
	}
	if (_debugMode) {
		return false;
	}
	//一定距離離れているとターゲットに入れない
	if (keepItr != chara.end()) {
		if (lastItr != chara.end()) {
			(*lastItr)->LastTargetOut();
		}
		SetCameraRockOnObject((*keepItr), camera);
		return true;
	}
	//右にいる敵がいなかったら、左にいる一番評価値が低い敵にロックオンを起動させる。
	bool next = (_right) ? false : true;
	bool rockOn = ChangeCameraRockOn(camera, next, false, false);
	my_assert(rockOn, "ロックオンの変更が失敗しました。");
	return true;
}

bool EnemyManager::TargetCancel(Camera* camera)
{
	for (auto itr = chara.begin(); itr != chara.end(); itr++) {
		if ((*itr)->GetBaseObject() == cameraTargetObj) {
			(*itr)->LastTargetOut();
			player->Component()->GetComponent<Player>()->TargetObjSet(nullptr);
			cameraTargetObj = nullptr;
			return true;
		}
	}
	return false;
}

void EnemyManager::JustAvoidTargetChange(BaseObject* _obj)
{
	for (auto itr = chara.begin(); itr != chara.end(); itr++) {
		if ((*itr)->GetBaseObject() == _obj) {
			(*itr)->LastTargetIn();
			cameraTargetObj = _obj;
			player->Component()->GetComponent<Player>()->TargetObjSet((*itr)->GetBaseObject());
			continue;
		}
		if ((*itr)->GetLastTarget()) {
			(*itr)->LastTargetOut();
			continue;
		}
	}
}

EnemyBase* EnemyManager::PlayerNearEnemy()
{
	float minDist = 100000.0f;
	EnemyBase* nearEnemy = nullptr;
	for (auto itrs = chara.begin(); itrs != chara.end(); itrs++) {
		VECTOR3 dir = (*itrs)->GetBaseObject()->GetTransform()->position - player->GetTransform()->position;
		float dist = dir.Size();
		if (dist <= minDist) {
			minDist = dist;
			nearEnemy = (*itrs);
		}
	}
	return nearEnemy;
}

void EnemyManager::NearEnemyAlpha(VECTOR3 camPos)
{
	for (auto itr = chara.begin(); itr != chara.end(); itr++) {
		VECTOR3 dist = (*itr)->GetBaseObject()->GetTransform()->position - camPos;
		if (dist.Size() <= 1000.0f) {
			float rate = dist.Size() / 1000.0f;
			float alpha = Easing::Lerp(-700.0f, 255.0f, rate);
			if (alpha < 0) {
				alpha = 0;
			}
			(*itr)->SetAlpha((int)alpha);
		}
		else {
			(*itr)->SetAlpha(255);
		}
	}
}

Transform EnemyManager::NearEnemyPos(const VECTOR3& _pos)
{
	if (chara.size() == 0) {
		return Transform();
	}
	float nearDist = 10000.0f;
	Transform nearTransform = *(*chara.begin())->GetBaseObject()->GetTransform();
	for (auto itr = chara.begin(); itr != chara.end(); itr++) {
		VECTOR3 dist = (*itr)->GetBaseObject()->GetTransform()->position - _pos;
		if (dist.Size() <= nearDist) {
			nearTransform = *(*itr)->GetBaseObject()->GetTransform();
			nearDist = dist.Size();
		}
	}
	return nearTransform;
}

void EnemyManager::SetCameraRockOnObject(EnemyBase* _enemy, Camera* _camera)
{
	_enemy->LastTargetIn();
	player->Component()->GetComponent<Player>()->TargetObjSet(_enemy->GetBaseObject());
	_camera->TargetSet(_enemy->GetBaseObject());
	cameraTargetObj = _enemy->GetBaseObject();
}

void EnemyManager::CameraRockOnStart(Camera* _camera)
{
	EnemyBase* chara = PlayerNearEnemy();
	SetCameraRockOnObject(chara, _camera);
}

EnemyAttackChangeCameraDirection EnemyManager::BossAttackCamera(Camera* camera, const Transform& _targetTransform)
{
	//ボスが攻撃を始めたらこの関数を呼ぶ
	//カメラの視野が90°より敵側に向いていなかったら敵の方向に向けて左側に分けるか右側に分けるかを決定する。
	VECTOR3 dir = camera->GetTarget() - camera->GetCameraTransform()->position;
	dir = dir.Normalize();

	VECTOR3 target = _targetTransform.position - player->GetTransform()->position;
	float dist = target.Size();

	if (VDot(dir, target.Normalize()) > cosf(90.0f * DegToRad)) {
		return EnemyAttackChangeCameraDirection::NONE;
	}

	VECTOR3 right = target.Normalize() * MGetRotY(90 * DegToRad);
	VECTOR3 tar = player->GetTransform()->position - _targetTransform.position;
	tar.y = 0;

	if (VDot(right, tar) > 0) {
		
		return EnemyAttackChangeCameraDirection::RIGHT;
	}
	else {
		
		return EnemyAttackChangeCameraDirection::LEFT;
	}
}

bool EnemyManager::CameraInEnemy()
{
	for (auto itr = chara.begin(); itr != chara.end(); itr++) {
		VECTOR3 pos = (*itr)->GetBaseObject()->GetTransform()->position;
		if (!CheckCameraViewClip(pos)) {
			return true;
		}
	}
	return false;
}

void EnemyManager::CanPlayerSpecialHit()
{
	for (auto itr = chara.begin(); itr != chara.end(); itr++) {
		(*itr)->SpecialAttackHit();
	}
}


	/*for (auto itr = enemy.begin(); itr != enemy.end(); itr++) {
		CharaBase* e;
		if ((*itr)->GetTag() == "ENEMY") {
			e = (*itr)->Component()->GetComponent<Enemy>();
		}
		else {
			e = (*itr)->Component()->GetComponent<Boss>();
		}
		
		if (e == nullptr) {
			continue;
		}
		if (e->GetLastTarget()) {
			lastItr = itr;
			continue;
		}
		VECTOR3 target = (*itr)->GetTransform()->position - player->GetTransform()->position;
		float dist = target.Size();
		
		if (VDot(dir.Normalize(), target.Normalize()) < cosf(60.0f * DegToRad)) {
			continue;
		}
		if (distance > dist) {
			distance = dist;
			keepItr = itr;
		}
	}

	if (distance < 100000) {
		
		if ((*lastItr)->GetTag() == "ENEMY") {
			(*lastItr)->Component()->GetComponent<Enemy>()->LastTargetOut();
		}
		else {
			(*lastItr)->Component()->GetComponent<Boss>()->LastTargetOut();
		}
		if ((*keepItr)->GetTag() == "ENEMY") {
			(*keepItr)->Component()->GetComponent<Enemy>()->LastTargetIn();
		}
		else {
			(*keepItr)->Component()->GetComponent<Boss>()->LastTargetIn();
		}
		player->Component()->GetComponent<Player>()->TargetObjSet(*keepItr);
		camera->TargetSet(*keepItr);
		return true;
	}
	if ((*lastItr)->GetTag() == "ENEMY") {
		(*lastItr)->Component()->GetComponent<Enemy>()->LastTargetOut();
	}
	else {
		(*lastItr)->Component()->GetComponent<Boss>()->LastTargetOut();
	}
	player->Component()->GetComponent<Player>()->TargetObjSet(nullptr);*/
