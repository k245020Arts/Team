#include "TrashEnemyManager.h"
#include "TrashEnemy.h"
#include "../../Common/LoadManager.h"
#include "../../Component/MeshRenderer/MeshRenderer.h"
#include "../../../ImGui/imgui.h"
#include "../../Component/MeshRenderer/MeshRenderer.h"
#include "../../Component/Transform/Transform.h"
#include "../../Component/MeshRenderer2D/MeshRenderer2D.h"
#include "../../Player/Player.h"
#include "../../Component/Physics/Physics.h"
#include "../../Camera/Camera.h"
#include "../../Component/Hierarchy/Hierarchy.h"
#include "../../Common/LoadManager.h"
#include "../../Component/Collider/sphereCollider.h"
#include "../../Component/Animator/Animator.h"
#include "../TrashEnemy/Enemy.h"
#include "../../Weapon/WeaponManager.h"
#include "../../Component/Shaker/Shaker.h"
#include "../../Component/Object/Object2D.h"
#include "../../Component/Guage/Guage.h"
#include "../Boss/Boss.h"
#include "../../Component/Collider/rayCollider.h"
#include "../../Component/Collider/ModelCollider.h"
#include "../../Common/Random.h"

TrashEnemyManager::TrashEnemyManager()
{
	player = FindGameObjectWithTag<Object3D>("PLAYER");
	camera = FindGameObjectWithTag<Object3D>("CAMERA_OBJ")->Component()->GetComponent<Camera>();

	WayPointOffset();

	comboRequest = false;
	attackCounter = 0;
	standbyCounter = 0;

	debugWaypoint = false;

	searchCounter = 1.0f;
}

TrashEnemyManager::~TrashEnemyManager()
{
}

void TrashEnemyManager::Update()
{
	if (enemies.empty())
		return;
	Separation();
	PlayerWayPoint();
	//CloseWayPoint();
	attackCounter++;
	int enemiesMax = (int)enemies.size();
	for (auto itr = enemies.begin(); itr != enemies.end(); )
	{
		if (attackCounter >= 200 /*&& (*itr)->GetNumber()== attackCounter*/)
		{
			(*itr)->AttackON();
			attackCounter = 0;
			/*attackCounter++;
			if (attackCounter >= enemiesMax)
				attackCounter = 0;*/
		}
		if ((*itr)->GetNumber() > enemiesMax)
			(*itr)->AddAttackID(-1);

		//連携攻撃のときにその敵が準備完了したかどうか
		if ((*itr)->GetStandby())
			standbyCounter++;
		//敵全員が準備完了したら攻撃に移る
		if ( standbyCounter == enemiesMax)
		{
			AllChangeState(StateID::T_ENEMY_RUN_S);
			standbyCounter = 0;
			break;
		}
		
		//雑魚敵が死んでたらlistから削除する
		if (!(*itr)->GetActive())
		{
			(*itr)->GetEnemyObj()->DestroyMe();
			//次の要素のイテレータが返る
			itr = enemies.erase(itr);
		}
		else
			++itr;
	}

	//CloseWeyPoint(VECTOR3(0, 0, 0));
}

void TrashEnemyManager::Draw()
{
    ImguiDraw();
	/*if (!debugWaypoint)
		return;*/

	
	for (auto itr : wayPoint)
	{
		if (!itr.active)
			continue;
		DrawSphere3D(itr.position, 40.0f, 32, GetColor(255, 0, 0), GetColor(255, 255, 255), itr.active);
	}
}

void TrashEnemyManager::CreateEnemy(VECTOR3 _pos, float enemySpawnCounter)
{
    for (int i = 0; i < enemySpawnCounter; i++)
    {
		if (enemies.size() >= 4)
			break;

		// 個別のenemyを作る
		Object3D* e;
		e = new Object3D();
		e->Init(EnemyInformation::BASE_POS, VZero, VECTOR3(2.5f, 2.5f, 2.5f), "ENEMY");
		//当たり判定を生成（やられ判定）
		SphereCollider* collider = e->Component()->AddComponent<SphereCollider>();
		CollsionInfo info;
		info.parentTransfrom = e->GetTransform();
		info.shape = CollsionInformation::SPHERE;
		info.oneColl = false;
		info.tag = CollsionInformation::Tag::ENEMY;
		info.size = 1.0f;
		collider->CollsionAdd(info, Transform(VECTOR3(0, 0, 0), VZero, VECTOR3(350.0f, 1.0f, 1.0f)));
		//当たり判定を生成（やられ判定）
		SphereCollider* collider2 = e->Component()->AddComponent<SphereCollider>();
		CollsionInfo info2;
		info2.parentTransfrom = e->GetTransform();
		info2.shape = CollsionInformation::SPHERE;
		info2.oneColl = false;
		info2.tag = CollsionInformation::Tag::ENEMY;
		info.size = 1.0f;
		collider2->CollsionAdd(info2, Transform(VECTOR3(0, 150, 0), VZero, VECTOR3(250.0f, 1.0f, 1.0f)));
		RayCollider* collider3 = e->Component()->AddComponent<RayCollider>();
		info.shape = CollsionInformation::RAY;
		info.tag = CollsionInformation::E_FLOOR;
		collider3->RaySet(info, Transform(VECTOR3(0, 200, 0), VZero, VECTOR3(1.0f, 1.0, 1.0)), Transform(VECTOR3(0, -100, 0), VZero, VECTOR3(1.0f, 1, 1)));

		Shaker* shaker = e->Component()->AddComponent<Shaker>();

		MeshRenderer* me = e->Component()->AddComponent<MeshRenderer>();
		int handle = MV1DuplicateModel( Load::LoadModel(Load::MODEL_PATH + "Ch45_nonPBR", ID::IDType::E_MODEL));
		me->ModelHandle(handle);
		me->RotationMesh(1, DX_PI_F);

		Animator* anim = e->Component()->AddComponent<Animator>();
		anim->BaseModelSet(handle, 1);
		anim->AddFile(ID::TE_IDOL, "E_IDOL", true, 1.0f);
		anim->AddFile(ID::TE_RUN, "E_RUN", true, 1.0f);
		anim->AddFile(ID::TE_ATTACK, "E_ATTACK1", false, 1.2f, 25.0f, 40.0f);
		anim->AddFile(ID::TE_ATTACK2, "E_ATTACK2", false, 1.0f, 25.0f, 30.0f);
		anim->AddFile(ID::E_DAMAGE, "E_DAMAGE", false, 1.0f);
		anim->AddFile(ID::E_DIE, "E_DEAD", false, 1.0f);
		
		anim->Play(ID::TE_IDOL);

		Physics* physics = e->Component()->AddComponent<Physics>();
		physics->Start(VECTOR3(0.0f, -150.0f, 0.0f), VECTOR3(3000.0f, 3000.0f, 3000.0f));

        // 個別のTrashEnemyを追加
        TrashEnemy* t = e->Component()->AddComponent<TrashEnemy>();
		t->Start(e);
        // enemiesに登録（個別インスタンス）
        enemies.emplace_back(t);

        // 位置を決める
        const float R_MAX = 1000.0f;
        float rangeX = (float)GetRand(R_MAX * 2) - R_MAX;
		float rangeY = (float)GetRand(R_MAX * 2) - R_MAX;
        VECTOR3 pos = VECTOR3(rangeX, 0, rangeY);
		//ポジションをセット
		t->CreateTrashEnemy(pos/*CloseWeyPoint(VZero)*/, i);
		//hp表示
		Object2D* guage = new Object2D();

		guage->Init(VECTOR2F(150, 115), VECTOR2F(0.0f, 0.0f), VECTOR2F(0.1f, 0.1f), "TrashEnemyHpGuage");

		e->AddChild(guage);

		Guage* g = guage->Component()->AddComponent<Guage>();
		g->EdgeDrawReady(Load::LoadImageGraph(Load::IMAGE_PATH + "bossHpEdge1", ID::BOSS_HP_EDGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F, Transform(VECTOR3(915.0f, 120.0f, 0.0f), VZero, VECTOR3(0.2f, 0.2f, 0.2f)));
		g->GuageDrawReady<TrashEnemy>(Load::LoadImageGraph(Load::IMAGE_PATH + "playerHp", ID::PLAYER_HP_GUAGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F);
		g->WorldToScreenMode(true, VECTOR3(0, 500, 0));
    }

	//Cooperate(StateID::COOPERATEATTACK1);
}

void TrashEnemyManager::ImguiDraw()
{
    ImGui::Begin("TrashEnemyManager");

	if (ImGui::Button("enemySpwn"))
		CreateEnemy(VZero, 4);
	if (ImGui::Button("ack1"))
		Cooperate(StateID::COOPERATEATTACK1);

	for (auto& itr : enemies)
	{
		//ImGui::Text("enemiesGetStandby: %d", itr->GetStandby());
		//ImGui::RadioButton("enemy", &debugButton, 0);
	}
	if (ImGui::Button("waypoint"))
	{
		if (debugWaypoint)
			debugWaypoint = false;
		else
			debugWaypoint = true;
	}

    ImGui::End();
}

void TrashEnemyManager::Cooperate(StateID::State_ID _id)
{
	//const float RANGE = 2000.0f; // プレイヤー中心の半径
	const float BIAS_FOV = -180 * DegToRad; // プレイヤーの向きへ寄せる幅（0で無効）

	VECTOR3 playerPos = player->GetTransform()->position;
	float playerRot = camera->GetCameraTransform()->rotation.y;

	int count = enemies.size();
	int index = 0;

	CloseWayPoint();

	//for (auto& e : enemies)
	//{
	//	//敵にセット
	//	//e->SetTargetPos(CloseWeyPoint(e->GetPos()), StateID::T_ENEMY_RUN_S);

	//	index++;
	//}
}

void TrashEnemyManager::AllChangeState(StateID::State_ID _id)
{
	for (auto& itr : enemies)
	{
		itr->ChangeState(_id);
	}
}

//void TrashEnemyManager::SavePos()
//{
//	int enemiesMax = enemies.size();
//	VECTOR3 playerPos = player->GetTransform()->position;
//	float playerRot = camera->GetCameraTransform()->rotation.y;
//	for (int i = 0; i < enemiesMax; i++)
//	{
//		const float RANGE = 1200.0f;			// プレイヤー中心の半径
//		const float BIAS_FOV = -180 * DegToRad; // プレイヤーの向きへ寄せる幅（0で無効）
//
//		//均等に割って円形に配置
//		float angle = (2.0f * DX_PI_F) * (float)i / (float)enemiesMax;
//		//円形の基本方向
//		//VECTOR3 dir = VECTOR3(cosf(angle), 0, sinf(angle));
//		//プレイヤーの向きを基準に回転させる
//		float finalAngle = angle + playerRot;
//		//回転を反映した方向
//		VECTOR3 rotatedDir = VECTOR3(cosf(finalAngle), 0, sinf(finalAngle));
//		//プレイヤーからの絶対座標
//		VECTOR3 target = playerPos + rotatedDir * RANGE;
//		//target.y = 0;
//
//		//savePos.emplace_back(target);
//	}
//}

void TrashEnemyManager::WayPointOffset()
{
	const int waypointMax = 8;
	const float RANGE = 1200.0f;// プレイヤー中心の半径

	for (int i = 0; i < waypointMax; i++)
	{
		//均等に割って円形に配置
		float angle = (2.0f * DX_PI_F) * (float)i / (float)waypointMax;

		//回転を反映した方向
		VECTOR3 rotatedDir = VECTOR3(cosf(angle), 0, sinf(angle));
		//プレイヤーからの絶対座標
		VECTOR3 target = rotatedDir * RANGE;
		target.y = 0;

		wayPointOffsets.emplace_back(target);
	}
}

void TrashEnemyManager::PlayerWayPoint()
{
	searchCounter += Time::DeltaTimeRate();
	if (searchCounter < 1)
		return;

	searchCounter = 0;
	wayPoint.clear();

	VECTOR3 playerPos = player->GetTransform()->position;

	for (auto& itr : wayPointOffsets)
	{
		wayPoint.emplace_back(WayPoint(itr + playerPos, true));
	}
}

void TrashEnemyManager::CloseWayPoint()
{
	VECTOR3 position = camera->GetCameraTransform()->position;
	position.y = 0;
	//正面べく
	VECTOR3 frontVec = VECTOR3(0, 0, 1) * MGetRotY(camera->GetCameraTransform()->rotation.y);
	float counter = 0;
	VECTOR3 savePos = INFINITY;
	for (auto enemy : enemies)
	{
		for (auto& itr : wayPoint)
		{
			if (counter == 0)
			{
				VECTOR3 vec = itr.position - position;
				//内積
				float dotProduct = VDot(frontVec, vec.Normalize());
				if (dotProduct > cosf(45 * DegToRad))
					itr.active = true;
				else
					itr.active = false;
			}
			if (itr.active)
			{
				VECTOR3 a = itr.position - enemy->GetPos();
				if (savePos.Size() > a.Size())
					savePos = a;
			}
		}
		counter = 1;
		enemy->SetTargetPos(savePos, StateID::T_ENEMY_RUN_S);
	}
}

void TrashEnemyManager::Separation()
{
	VECTOR pos1 = { 0,0,0 };
	VECTOR pos2 = { 0,0,0 };
	const float E_SIZE = 500;
	for (auto& itr1 : enemies)
	{
		for (auto& itr2 : enemies)
		{
			if (itr1 == itr2)
				continue;

			pos1 = itr1->GetPos();
			pos2 = itr2->GetPos();
			VECTOR3 vec = pos1 - pos2;
			VECTOR3 vec2 = pos2 - pos1;

			vec.y = 0;
			vec2.y = 0;

			//エネミーの分散
			if (vec.Size() <= E_SIZE)
			{
				itr1->AddPos(vec.Normalize());
				itr2->AddPos(vec2.Normalize());
				/*itr1->AddPos(VNorm(pos1 - pos2));
				itr2->AddPos(VNorm(pos2 - pos1));*/
			}
		}
	}
}