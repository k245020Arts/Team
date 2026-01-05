#include "CutSceneCamera.h"
#include "../../Common/Easing.h"
#include "../../Component/Transform/Transform.h"
#include "../cameraInformation.h"
#include "../../Component/Shaker/Shaker.h"
#include "cameraStateManager.h"
#include "../camera.h"

CutSceneCamera::CutSceneCamera()
{
    string = Function::GetClassNameC<CutSceneCamera>();
    cutSceneIndex = 0;
}

CutSceneCamera::~CutSceneCamera()
{
}

void CutSceneCamera::Update()
{
    Camera* camera = GetBase<Camera>();
    if (camera->cutSceneData.empty()) return;

    
    CutSceneSpece::CutScene& cut = camera->cutSceneData[cutSceneIndex];

    time -= Time::DeltaTimeRate();
    float t = (time / cut.duration);
    t = std::clamp(t, 0.0f, 1.0f);

    // カメラ座標を補間
    Transform* posTransfrom = PlayerEnemyWorldToPos(cut.followPosName);
    VECTOR3 endPos = cut.camera.endPos * posTransfrom->GetRotationMatrix();
    VECTOR3 movePos = Easing::EaseIn(endPos, firstPos, t);
    
    camera->cameraComponent.cameraTransform->position = posTransfrom->position + movePos;
    Transform* targetTransfrom = PlayerEnemyWorldToPos(cut.followPosTarget);
    camera->target = targetTransfrom->position + cut.camera.target;

    camera->CameraRotationSet();
        
    // カット終了時の処理
    if (t <= 0.0f) {
        cutSceneIndex++;
        if (cutSceneIndex >= camera->cutSceneData.size()) {
            camera->cameraComponent.state->ChangeState(StateID::FREE_CAMERA_S);
            cutSceneIndex = 0;
        }
        else {
            time = camera->cutSceneData[cutSceneIndex].duration; // 次のカットへ
            firstPos = camera->cutSceneData[cutSceneIndex - 1].camera.endPos;
        }
    }
}

void CutSceneCamera::Draw()
{
}

void CutSceneCamera::Start()
{
    Camera* camera = GetBase<Camera>();
    if (camera->cutSceneData.empty()) return;

    cutSceneIndex = 0;
    time = camera->cutSceneData[cutSceneIndex].duration;
    firstPos = camera->cutSceneData[cutSceneIndex].camera.startPos;
    camera->isCutScene = true;
}

void CutSceneCamera::Finish()
{
    Camera* camera = GetBase<Camera>();
    camera->isCutScene = false;
}

Transform* CutSceneCamera::PlayerEnemyWorldToPos(std::string _name)
{
    Camera* camera = GetBase<Camera>();
    if (_name == PLAYER_POS_NAME) {
        return camera->cameraComponent.player.transform;
    }
    else if (_name == ENEMY_POS_NAME) {
        return camera->cameraComponent.target.transform;
    }
    else {
        return nullptr;
    }
}

void CutSceneCamera::StateImguiDraw()
{
    Camera* camera = GetBase<Camera>();
    CutSceneSpece::CutScene& cut = camera->cutSceneData[cutSceneIndex];
    ImGui::Text("index", cutSceneIndex);
    ImGui::DragFloat3("startPos",&cut.camera.startPos.x,1.0f,1000.0f,100000.0f);
    ImGui::DragFloat3("EndPos",&cut.camera.endPos.x,1.0f,1000.0f,100000.0f);
       
}
