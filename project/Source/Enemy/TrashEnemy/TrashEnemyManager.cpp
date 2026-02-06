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
#include "../../Common/Debug/Debug.h"

TrashEnemyManager::TrashEnemyManager()
{
	player = FindGameObjectWithTag<Object3D>("PLAYER");
	camera = FindGameObjectWithTag<Object3D>("CAMERA_OBJ")->Component()->GetComponent<Camera>();
	for (int i = 0; i < 4; i++)
	{
		stage[i] = FindGameObjectWithTag<Object3D>("WALL" + std::to_string(i));
	}
	
	WayPointOffset();

	comboRequest = false;
	attackCounter = 0;
	standbyCounter = 0;

	debugWaypoint = false;

	searchCounter = 1.0f;
	maxAttackCounter = ATK_COUNTER_MAX * Random::GetReal();

	cooperateCounter = 0;
}

TrashEnemyManager::~TrashEnemyManager()
{
}

void TrashEnemyManager::Update()
{
	if (enemies.empty())
		return;
	Separation();

	//通常攻撃のカウンター
	attackCounter += Time::DeltaTimeRate();

	for (auto itr = enemies.begin(); itr != enemies.end(); )
	{
		NormalAttackMove((*itr));
	
		CooperateAttackMove((*itr));

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

void TrashEnemyManager::CreateEnemy(VECTOR3 _pos, float enemySpawnCounter)
{
	//同じ種類の敵を何体出すか
	int kindsCounter = enemySpawnCounter / 3;
	//何種類目の敵か
	int numCounter = 0;
	//その種類の敵が何体スポーンしたか
	int spawnCounter = 0;

    for (int i = 0; i < enemySpawnCounter; i++)
    {
		if (enemies.size() >= ENEMIESMAX)
			break;

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
		//当たり判定を生成（やられ判定）
		/*SphereCollider* collider2 = e->Component()->AddComponent<SphereCollider>();
		CollsionInfo info2;
		info2.parentTransfrom = e->GetTransform();
		info2.shape = CollsionInformation::SPHERE;
		info2.oneColl = false;
		info2.tag = CollsionInformation::Tag::ENEMY;
		info.size = 1.0f;
		collider2->CollsionAdd(info2, Transform(VECTOR3(0, 150, 0), VZero, VECTOR3(250.0f, 1.0f, 1.0f)));*/
		RayCollider* collider3 = e->Component()->AddComponent<RayCollider>();
		info.shape = CollsionInformation::RAY;
		info.tag = CollsionInformation::E_FLOOR;
		collider3->RaySet(info, Transform(VECTOR3(0, 150, 0), VZero, VECTOR3(1.0f, 1.0, 1.0)), Transform(VECTOR3(0, 1/*-100*/, 0), VZero, VECTOR3(1.0f, 1, 1)));

		Shaker* shaker = e->Component()->AddComponent<Shaker>();

		MeshRenderer* me = e->Component()->AddComponent<MeshRenderer>();
		int handle = MV1DuplicateModel( Load::LoadModel(Load::MODEL_PATH + "Ch45_nonPBR", ID::IDType::E_MODEL));
		me->ModelHandle(handle,true);
		me->RotationMesh(1, DX_PI_F);

		Animator* anim = e->Component()->AddComponent<Animator>();
		anim->BaseModelSet(handle, 1);
		anim->AddFile(ID::TE_IDOL, "E_IDOL", true, 1.0f);
		anim->AddFile(ID::TE_RUN, "E_RUN", true, 1.0f);
		anim->AddFile(ID::TE_ATTACK, "E_ATTACK1", false, 0.8f, 20.0f, 30.0f);
		anim->AddFile(ID::TE_ATTACK2, "E_ATTACK2", false, 1.0f, 25.0f, 35.0f);
		anim->AddFile(ID::E_DAMAGE, "E_DAMAGE", false, 1.0f);
		anim->AddFile(ID::E_DIE, "E_DEAD", false, 2.0f);
		
		anim->Play(ID::TE_IDOL);

		Physics* physics = e->Component()->AddComponent<Physics>();
		physics->Start(VECTOR3(0.0f, -150.0f, 0.0f), VECTOR3(3000.0f, 3000.0f, 3000.0f));

        // 個別のTrashEnemyを追加
        TrashEnemy* t = e->Component()->AddComponent<TrashEnemy>();
		t->Start(e);
        // enemiesに登録（個別インスタンス）
        enemies.emplace_back(t);

        // 位置を決める
        const float R_MAX = 2000.0f;
        float rangeX = (float)GetRand(R_MAX * 2) - R_MAX;
		float rangeY = (float)GetRand(R_MAX * 2) - R_MAX;
        VECTOR3 pos = VECTOR3(rangeX, 0, rangeY);

		//敵の種類の数ができるだけ均等にするための処理
		if (spawnCounter >= kindsCounter)
		{
			numCounter++;
			spawnCounter = 0;
		}
		//ポジションをセット
		t->CreateTrashEnemy(_pos + pos, numCounter);
		spawnCounter++;
		//hp表示
		Object2D* guage = new Object2D();

		guage->Init(VECTOR2F(150, 115), VECTOR2F(0.0f, 0.0f), VECTOR2F(0.2f, 0.2f), "TrashEnemyHpGuage");

		e->AddChild(guage);

		Guage* g = guage->Component()->AddComponent<Guage>();
		/*g->EdgeDrawReady(Load::LoadImageGraph(Load::IMAGE_PATH + "bossHpEdge1", ID::BOSS_HP_EDGE), MeshRenderer2D::DRAW_BILLBOARD, Transform(VECTOR3(915.0f, 120.0f, 0.0f), VZero, VECTOR3(0.2f, 0.2f, 0.2f)));
		g->GuageDrawReady<TrashEnemy>(Load::LoadImageGraph(Load::IMAGE_PATH + "playerHp", ID::PLAYER_HP_GUAGE), MeshRenderer2D::DRAW_BILLBOARD,Guage::BAR_MODE::HP);*/
		g->EdgeDrawReady(Load::LoadImageGraph(Load::IMAGE_PATH + "bossHpEdge1", ID::BOSS_HP_EDGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F, Transform(VECTOR3(915.0f, 120.0f, 0.0f), VZero, VECTOR3(0.2f, 0.2f, 0.2f)));
		g->GuageDrawReady<TrashEnemy>(Load::LoadImageGraph(Load::IMAGE_PATH + "playerHp", ID::PLAYER_HP_GUAGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F, Guage::BAR_MODE::HP);
		g->WorldToScreenMode(true, VECTOR3(0, 700, 0));
    }

	//Cooperate(StateID::COOPERATEATTACK1);
}

int TrashEnemyManager::GetActiveEnemy()
{
	int _counter = 0;
	float* _hp = 0;
	for (auto& itr : enemies)
	{
		if (itr->GetHp() > _hp)//Activeでやると死んでるモーション挟んでる敵もカウントされるため
			_counter++;
	}

	return _counter;
}

void TrashEnemyManager::ImguiDraw()
{
    ImGui::Begin("TrashEnemyManager");

	ImGui::Text("enemiesSize: %d", enemies.size());

	if (ImGui::Button("enemySpwn"))
		CreateEnemy(VZero, 1);
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
	for (auto& way : wayPoint)
	{
		if (!way.active)
			continue;
		
	}

    ImGui::End();
}

void TrashEnemyManager::Cooperate(StateID::State_ID _id)
{
	CloseWayPoint();
}

void TrashEnemyManager::AllChangeState(StateID::State_ID _id)
{
	for (auto& itr : enemies)
	{
		if (itr->IsMovingToPlayer())
			itr->ChangeState(_id);
		else
			itr->CooperateAtkFinish();
	}
}

void TrashEnemyManager::NormalAttackMove(TrashEnemy* _enemy)
{
	if (_enemy->IsCooperateAtk()||!_enemy->IsAttack())
		return;

	if (attackCounter >= ATK_COUNTER_MIN + maxAttackCounter)
	{
		if (_enemy->IsAttack())
		{
			_enemy->AttackCommand();
			attackCounter = 0;
			maxAttackCounter = ATK_COUNTER_MAX * Random::GetReal();
		}
	}
	else if (attackCounter >= ATK_COUNTER_MAX)
		_enemy->AttackCoolTimeReset();
}

void TrashEnemyManager::CooperateAttackMove(TrashEnemy* _enemy)
{
	if (!_enemy->IsCooperateAtk())
		return;
	int enemiesMax = (int)enemies.size();

	//連携攻撃のときにその敵が準備完了したかどうか
	if (_enemy->GetStandby())
		standbyCounter++;
	if (standbyCounter >= 1)
		cooperateCounter += Time::DeltaTimeRate();
	//敵全員が準備完了するか時間経過で攻撃に移る
	if (standbyCounter == enemiesMax || cooperateCounter >= 3)
	{
		AllChangeState(StateID::T_ENEMY_RUN_S);
		standbyCounter = 0;
		cooperateCounter = 0;
		//break;
	}
}

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
	/*searchCounter += Time::DeltaTimeRate();
	if (searchCounter < 1)
		return;*/

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
	PlayerWayPoint();
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
			//使っていいウェイポイントを一回だけ探す
			if (counter == 0)
			{
				VECTOR3 vec = itr.position - position;
				//内積
				float dotProduct = VDot(frontVec, vec.Normalize());
				//カメラに写ってるか
				if (dotProduct > cosf(45 * DegToRad))
					itr.active = true;
				//カメラに写ってなかったら
				else
					itr.active = false;

				/*counter = 1;*/
			}
			//一番近いウェイポイントを探す
			if (itr.active)
			{
				VECTOR3 vec = itr.position - enemy->GetPos();
				if (savePos.Size() > vec.Size())
					savePos = itr.position/*vec*/;
			}
		}
		enemy->GetWayPoint(savePos, StateID::T_ENEMY_RUN_S);
		counter = 1;
	}
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
			}
		}
	}
}