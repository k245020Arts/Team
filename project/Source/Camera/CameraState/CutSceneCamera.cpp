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
    first = false;
    firstPos = VZero;
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
    VECTOR3 endPos = cut.camera.endPos;
    if (posTransfrom != nullptr) {
        endPos = cut.camera.endPos * posTransfrom->GetRotationMatrix();
    }
   
    VECTOR3 movePos = VZero; 

    switch (cut.ease)
    {
    case CutSceneSpece::EaseType::Linear:
        movePos = Easing::Lerp(endPos, firstPos, t);
        break;
    case CutSceneSpece::EaseType::In:
        movePos = Easing::EaseIn(endPos, firstPos, t);
        break;
    case CutSceneSpece::EaseType::Out:
        movePos = Easing::EaseOut(endPos, firstPos, t);
        break;
    case CutSceneSpece::EaseType::InOut:
        movePos = Easing::EaseInOut(endPos, firstPos, t);
        break;
    }
    
    if (first || cut.followPosName == PLAYER_POS_NAME || cut.followPosName == ENEMY_POS_NAME) {
        if (posTransfrom != nullptr) {
            camera->cameraComponent.cameraTransform->position = posTransfrom->position + movePos;
            keepPos = posTransfrom->position;
        }
    }
    else {
        camera->cameraComponent.cameraTransform->position = keepPos + movePos;
    }
    
    Transform* targetTransfrom = PlayerEnemyWorldToPos(cut.followPosTarget);
    
    if (cut.followPosName == PLAYER_FIRST_POS_NAME || cut.followPosName == ENEMY_FIRST_POS_NAME) {
        if (first) {
            camera->target = targetTransfrom->position + (cut.camera.target * targetTransfrom->GetRotationMatrix());
            keepTarget = targetTransfrom->position;
        }
        else {
            camera->target = keepTarget + cut.camera.target;
            //keepTarget = targetTransfrom->position;
        }
    }
    else {
        camera->target = targetTransfrom->position + cut.camera.target;
        //keepTarget = targetTransfrom->position;
    }
   

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
            firstPos = endPos;
            first = true;
        }
    }
    first = false;
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
    first = true;
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
    if (_name == PLAYER_POS_NAME || _name == PLAYER_FIRST_POS_NAME) {
        return camera->cameraComponent.player.transform;
    }
    else if (_name == ENEMY_POS_NAME || _name == ENEMY_FIRST_POS_NAME) {
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
