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

TrashEnemyManager::TrashEnemyManager()
{

}

TrashEnemyManager::~TrashEnemyManager()
{
}

void TrashEnemyManager::Update()
{
}

void TrashEnemyManager::Draw()
{
    ImguiDraw();
}

void TrashEnemyManager::CreateEnemy(VECTOR3 _pos, float enemySpawnCounter)
{
    for (int i = 0; i < enemySpawnCounter; i++)
    {
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
		me->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "Ch45_nonPBR", ID::IDType::E_MODEL));
		me->RotationMesh(1, DX_PI_F);

		Animator* anim = e->Component()->AddComponent<Animator>();
		anim->BaseModelSet(Load::GetHandle(ID::E_MODEL), 1);
		anim->AddFile(ID::TE_ATTACK, "B_ATTACK3", false, 0.7f, 11.0f, 18.0f);
		/*anim->AddFile(ID::E_ANIM_IDOL, "E_IDOL", true, 1.0f);
		anim->AddFile(ID::E_DAMAGE, "E_DAMAGE", false, 1.0f);
		anim->AddFile(ID::E_FALL, "E_FALL", true, 1.0f);
		anim->AddFile(ID::E_GETUP, "E_GETUP", false, 2.0f);
		anim->AddFile(ID::E_RUN, "E_RUN", true, 1.0f);
		anim->AddFile(ID::IDType::E_DIE, "E_DIE", false, 0.5f, 9.0f, 12.0f);*/
		
		//anim->Play(ID::E_ANIM_IDOL);

		Physics* physics = e->Component()->AddComponent<Physics>();
		physics->Start(VECTOR3(0.0f, -150.0f, 0.0f), VECTOR3(3000.0f, 3000.0f, 3000.0f));

        // 個別のTrashEnemyを追加
        TrashEnemy* t = e->Component()->AddComponent<TrashEnemy>();
		t->Start(e);
        // enemiesに登録（個別インスタンス）
        enemies.emplace_back(t);

        // 位置を決める
        const float R_MAX = 1000.0f;
        float range = GetRand(R_MAX * 2) - R_MAX;
        VECTOR3 pos = VECTOR3(range, 0, range);
        t->CreateTrashEnemy(_pos + pos);
    }
}

void TrashEnemyManager::ImguiDraw()
{
    ImGui::Begin("TrashEnemyManager");

    ImGui::Text("enemies: %d", enemies.size());

    ImGui::End();
}
