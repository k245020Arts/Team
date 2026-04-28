#include "TrashEnemyManager.h"
#include "TrashEnemy.h"
#include "../../Common/ResourceLoader.h"
#include "../../Component/MeshRenderer/MeshRenderer.h"
#include "../../../ImGui/imgui.h"
#include "../../Component/MeshRenderer/MeshRenderer.h"
#include "../../Component/Transform/Transform.h"
#include "../../Component/MeshRenderer2D/MeshRenderer2D.h"
#include "../../Player/Player.h"
#include "../../Component/Physics/Physics.h"
#include "../../Component/Hierarchy/Hierarchy.h"
#include "../../Component/Collider/sphereCollider.h"
#include "../../Component/Animator/Animator.h"
#include "../../Weapon/WeaponManager.h"
#include "../../Component/Shaker/Shaker.h"
#include "../../Component/Object/Object2D.h"
#include "../../Component/Guage/Guage.h"
#include "../Boss/Boss.h"
#include "../../Component/Collider/rayCollider.h"
#include "../../Component/Collider/ModelCollider.h"
#include "../../Common/Random.h"
#include "../../Common/Debug/Debug.h"
#include "../../GameControler/GameControler.h"
#include "TrashEnemyGroup.h"
#include "../../Common/ResourceLoader.h"

TrashEnemyManager::TrashEnemyManager()
{
	player = FindGameObjectWithTag<Object3D>("PLAYER");
	
	enemyGroup = new TrashEnemyGroup;

	for (int i = 0; i < 4; i++)
	{
		stage[i] = FindGameObjectWithTag<Object3D>("WALL" + std::to_string(i));
	}
	
	WayPointOffset();

	comboRequest = false;
	
	standbyCounter = 0;

	debugWaypoint = false;

	searchCounter = 1.0f;
	maxAttackCounter = ATK_COUNTER_MAX * (float)Random::GetReal();

	startRangedAtk = false;
}

TrashEnemyManager::~TrashEnemyManager()
{
}

void TrashEnemyManager::Update()
{
	if (startRangedAtk)
		enemyGroup->RangedEnemyAttack();
}

void TrashEnemyManager::Draw()
{
	if(Debug::GetDebugMode())
		ImguiDraw();
   
	if (!debugWaypoint)
		return;
	
	for (auto itr : wayPoint)
	{
		if (!itr.active)
			continue;
		DrawSphere3D(itr.position, 40.0f, 32, GetColor(255, 0, 0), GetColor(255, 255, 255), itr.active);
	}
}

void TrashEnemyManager::CreateEnemy(VECTOR3 _pos, int enemySpawnCounter)
{
	//同じ種類の敵を何体出すか
	int kindsCounter = enemySpawnCounter / 3;
	//何種類目の敵か
	int numCounter = 0;
	//その種類の敵が何体スポーンしたか
	int spawnCounter = 0;

	int number = 0;

    for (int i = 0; i < enemySpawnCounter; i++)
    {
		// 個別のenemyを作る
		Object3D* e;
		e = new Object3D();
		e->Init(EnemyInformation::BASE_POS, VZero, VECTOR3(2.5f, 2.5f, 2.5f), "ENEMY" + std::to_string(i));
		//当たり判定を生成（やられ判定）
		SphereCollider* collider = e->Component()->AddComponent<SphereCollider>();
		CollsionInfo info;
		info.parentTransfrom = e->GetTransform();
		info.shape = CollsionInformation::SPHERE;
		info.oneColl = false;
		info.tag = CollsionInformation::Tag::ENEMY;
		info.size = 1.0f;//
		collider->CollsionAdd(info, Transform(VECTOR3(0, 100, 0), VZero, VECTOR3(350.0f, 1.0f, 1.0f)));
		
		RayCollider* collider3 = e->Component()->AddComponent<RayCollider>();
		info.shape = CollsionInformation::RAY;
		info.tag = CollsionInformation::E_FLOOR;
		collider3->RaySet(info, Transform(VECTOR3(0, 150, 0), VZero, VECTOR3(1.0f, 1.0, 1.0)), Transform(VECTOR3(0, 1/*-100*/, 0), VZero, VECTOR3(1.0f, 1, 1)));

		Shaker* shaker = e->Component()->AddComponent<Shaker>();

		MeshRenderer* me = e->Component()->AddComponent<MeshRenderer>();
		int handle = MV1DuplicateModel(ResourceLoad::LoadModel("Ch45_nonPBR", ID::IDType::E_MODEL));
		me->ModelHandle(handle,true);
		me->RotationMesh(1, DX_PI_F);

		std::string charaID = "1";
		std::string typeID = "000";

		Animator* anim = e->Component()->AddComponent<Animator>();
		anim->BaseModelSet(handle, 1);
		/*anim->AddFile(ID::TE_IDOL, charaID + typeID + "_IDOL", true, 1.0f);
		anim->AddFile(ID::TE_RUN, charaID + typeID + "_RUN", true, 1.0f);
		anim->AddFile(ID::TE_ATTACK, charaID + typeID + "_ATTACK1", false, 0.8f, 20.0f, 30.0f);
		anim->AddFile(ID::TE_ATTACK2, charaID + typeID + "_ATTACK2", false, 1.0f, 25.0f, 35.0f);
		anim->AddFile(ID::E_DAMAGE, charaID + typeID + "_DAMAGE", false, 1.0f);
		anim->AddFile(ID::E_DIE, charaID + typeID + "_DEAD", false, 2.0f);*/
		ResourceLoad::LoadAnim(charaID + typeID + "_IDOL", ID::TE_IDOL);
		ResourceLoad::LoadAnim(charaID + typeID + "_RUN", ID::TE_RUN);
		ResourceLoad::LoadAnim(charaID + typeID + "_ATTACK1", ID::TE_ATTACK);
		ResourceLoad::LoadAnim(charaID + typeID + "_ATTACK2", ID::TE_ATTACK2);
		ResourceLoad::LoadAnim(charaID + typeID + "_DAMAGE", ID::E_DAMAGE);
		ResourceLoad::LoadAnim(charaID + typeID + "_DEAD", ID::E_DIE);

		anim->AnimDataLoad(charaID, typeID);;
		
		anim->Play(ID::TE_IDOL);

		Physics* physics = e->Component()->AddComponent<Physics>();
		physics->Start(VECTOR3(0.0f, -150.0f, 0.0f), VECTOR3(3000.0f, 3000.0f, 3000.0f));

        // 個別のTrashEnemyを追加
        TrashEnemy* t = e->Component()->AddComponent<TrashEnemy>();
		t->Start(e);
       
        // 位置を決める
        const int R_MAX = 2000;
        float rangeX = (float)GetRand(R_MAX * 2) - R_MAX;
		float rangeY = (float)GetRand(R_MAX * 2) - R_MAX;
        VECTOR3 pos = VECTOR3(rangeX, 3000, rangeY);//

		//敵の種類の数ができるだけ均等にするための処理
		if (spawnCounter >= kindsCounter)
		{
			numCounter++;
			spawnCounter = 0;
		}
		//
		number++;
		if (number >= wayPointOffsets.size())
			number = 0;

		//ポジションをセット
		t->CreateTrashEnemy(_pos + pos, numCounter, number);
		spawnCounter++;
		//hp表示
		Object2D* guage = new Object2D();

		guage->Init(VECTOR2F(150, 115), VECTOR2F(0.0f, 0.0f), VECTOR2F(0.2f, 0.2f), "TrashEnemyHpGuage");

		e->AddChild(guage);

		Guage* g = guage->Component()->AddComponent<Guage>();
		/*g->EdgeDrawReady(Load::LoadImageGraph(Load::IMAGE_PATH + "bossHpEdge1", ID::BOSS_HP_EDGE), MeshRenderer2D::DRAW_BILLBOARD, Transform(VECTOR3(915.0f, 120.0f, 0.0f), VZero, VECTOR3(0.2f, 0.2f, 0.2f)));
		g->GuageDrawReady<TrashEnemy>(Load::LoadImageGraph(Load::IMAGE_PATH + "playerHp", ID::PLAYER_HP_GUAGE), MeshRenderer2D::DRAW_BILLBOARD,Guage::BAR_MODE::HP);*/
		g->EdgeDrawReady(ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "bossHpEdge1", ID::BOSS_HP_EDGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F, Transform(VECTOR3(915.0f, 120.0f, 0.0f), VZero, VECTOR3(0.2f, 0.2f, 0.2f)));
		g->GuageDrawReady<TrashEnemy>(ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "playerHp", ID::PLAYER_HP_GUAGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F, Guage::BAR_MODE::HP);
		g->WorldToScreenMode(true, VECTOR3(0, 700, 0));

		//enemyGroup->SetMeleeEnemy(t);
		enemyGroup->SetRangedEnemy(t);
    }
}

int TrashEnemyManager::GetEnemySize() const
{
	return enemyGroup->GetEnemySize();
}

int TrashEnemyManager::GetActiveEnemy() const
{
	return enemyGroup->GetActiveEnemy();
}

void TrashEnemyManager::ImguiDraw()
{
    ImGui::Begin("TrashEnemyManager");

	ImGui::Text("enemiesSize: %d", enemyGroup->GetActiveEnemy());

	if (ImGui::Button("enemySpwn"))
		CreateEnemy(VZero, 1);
	if (ImGui::Button("ack1"))
		Cooperate();
	if (ImGui::Button("ack2"))
		startRangedAtk = true;

	if (ImGui::Button("waypoint"))
	{
		if (debugWaypoint)
			debugWaypoint = false;
		else
			debugWaypoint = true;
	}
	for (auto& way : wayPoint)
	{
		if (!way.active)
			continue;
	}

    ImGui::End();
}

void TrashEnemyManager::Cooperate()
{
	PlayerWayPoint();

	enemyGroup->CloseWayPoint(wayPoint);
}

void TrashEnemyManager::WayPointOffset()
{
	const int WaypointMax = 8;
	const float Range = 1500.0f;// プレイヤー中心の半径

	for (int i = 0; i < WaypointMax; i++)
	{
		//均等に割って円形に配置
		float angle = (2.0f * DX_PI_F) * (float)i / (float)WaypointMax;

		//回転を反映した方向
		VECTOR3 rotatedDir = VECTOR3(cosf(angle), 0, sinf(angle));
		//プレイヤーからの絶対座標
		VECTOR3 target = rotatedDir * Range;
		target.y = 0;

		wayPointOffsets.emplace_back(target);
	}
}

void TrashEnemyManager::PlayerWayPoint()
{
	searchCounter = 0;
	wayPoint.clear();
	
	VECTOR3 playerPos = player->GetTransform()->position;
	
	for (auto& itr : wayPointOffsets)
	{
		wayPoint.emplace_back(WayPoint(itr + playerPos, true));
	}

	//enemyGroup->CloseWayPoint(wayPoint);
}

bool TrashEnemyManager::StageWall(VECTOR3 _pos)
{
	float size = 13000.0f;
	/*for (auto* itr : stage)*/
	//{
		if (/*itr->GetTransform()->position.x*/ size < _pos.x)
			return true;
		else if (/*itr->GetTransform()->position.x*/ -size > _pos.x)
			return true;
		else if (/*itr->GetTransform()->position.z*/ size < _pos.z)
			return true;
		else if (/*itr->GetTransform()->position.z*/ -size > _pos.z)
			return true;
		else
			return false;
	//}
}

void TrashEnemyManager::Cooperate2Move()
{

}

std::vector<VECTOR3> TrashEnemyManager::GetWayPointPosition()
{
	std::vector<VECTOR3> _pos;
	PlayerWayPoint();

	for (auto itr : wayPoint)
	{
		_pos.push_back(itr.position);
	}

	return _pos;
}
