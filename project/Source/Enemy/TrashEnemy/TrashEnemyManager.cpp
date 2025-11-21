#include "TrashEnemyManager.h"
#include "TrashEnemy.h"
#include "../../Common/LoadManager.h"
#include "../../Component/MeshRenderer/MeshRenderer.h"
#include "../../../ImGui/imgui.h"

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
        Object3D* enemy = new Object3D();
        const float SIZE = 3.0f;
        enemy->Init(Transform(VZero, VZero, VECTOR3(SIZE, SIZE, SIZE)), "TrashEnemy");

        MeshRenderer* m = enemy->Component()->AddComponent<MeshRenderer>();
        m->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "Ch45_nonPBR", ID::E_MODEL));
        m->RotationMesh(0, 180.0f * DegToRad);

        // 個別のTrashEnemyを追加
        TrashEnemy* t = enemy->Component()->AddComponent<TrashEnemy>();

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
   // gl::clear(Color(0, 0, 0));

    ImGui::Begin("TrashEnemyManager");

    ImGui::Text("enemies: %d", enemies.size());

    ImGui::End();
}
