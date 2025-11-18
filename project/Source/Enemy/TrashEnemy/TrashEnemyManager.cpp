#include "TrashEnemyManager.h"
#include "TrashEnemy.h"
#include "../../Common/LoadManager.h"
#include "../../Component/MeshRenderer/MeshRenderer.h"

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
}

void TrashEnemyManager::CreateEnemy(VECTOR3 _pos, float enemySpawnCounter)
{
    //Transformの設定
    Object3D* obj = FindGameObjectWithTag<Object3D>("PLAYER");
    //player = obj;
    Object3D* enemy;
    enemy = new Object3D();
    float size = 1.0f;
    enemy->Init(Transform(VZero, VZero, VECTOR3(size, size , size)), "TrashEnemy");

    //Shaker* shaker = enemy->Component()->AddComponent<Shaker>();

    //モデルの設定
    MeshRenderer* m = enemy->Component()->AddComponent<MeshRenderer>();
    m->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "Ch45_nonPBR", ID::E_MODEL));
    m->RotationMesh(0, 180.0f * DegToRad);

    //コンポーネントを指定された分作る
    for (int i = 0; i < enemySpawnCounter; i++)
        enemies.push_back(enemy->Component()->AddComponent<TrashEnemy>());
    //生成
    for (auto itr : enemies)
    {
        itr->CreateTrashEnemy(_pos);
    }
}


