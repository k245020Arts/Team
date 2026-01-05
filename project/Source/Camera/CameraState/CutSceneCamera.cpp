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
    VECTOR3 movePos = Easing::EaseIn(firstPos, cut.camera.endPos, t);
    camera->cameraComponent.cameraTransform->position = movePos + PlayerEnemyWorldToPos(cut.followPosName);
    camera->target = PlayerEnemyWorldToPos(cut.followPosTarget) + cut.camera.target;

    camera->CameraRotationSet();
        
    // カット終了時の処理
    if (t <= 0.0f) {
        cutSceneIndex++;
        if (cutSceneIndex >= camera->cutSceneData.size()) {
            camera->cameraComponent.state->ChangeState(StateID::FREE_CAMERA_S);
        }
        else {
            time = camera->cutSceneData[cutSceneIndex].duration; // 次のカットへ
            firstPos = camera->cutSceneData[cutSceneIndex].camera.startPos;
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
}

void CutSceneCamera::Finish()
{
}

VECTOR3 CutSceneCamera::PlayerEnemyWorldToPos(std::string _name)
{
    Camera* camera = GetBase<Camera>();
    if (_name == PLAYER_POS_NAME) {
        return camera->cameraComponent.player.transform->position;
    }
    else if (_name == ENEMY_POS_NAME) {
        return camera->cameraComponent.target.transform->position;
    }
    else {
        return 0.f;
    }
}
