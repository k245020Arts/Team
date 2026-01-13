#include "CutSceneCamera.h"
#include "../../Common/Easing.h"
#include "../../Component/Transform/Transform.h"
#include "../cameraInformation.h"
#include "../../Component/Shaker/Shaker.h"
#include "cameraStateManager.h"
#include "../camera.h"
#include "../CameraEditorGui.h"

//#define CUTNUMROM

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
#ifdef CUTNUMROM
    VECTOR3 movePos = VZero;

    VECTOR3 dir = (endPos - firstPos);
    dir = dir.Normalize();
    float size = dir.Size();
    VECTOR3 up = VECTOR3(0, 1, 0);
    VECTOR3 side = dir.Cross(up);
    side = side.Normalize();

    float curveSize = size * 0.3f;

    VECTOR3 pb = firstPos - dir * size * 0.2f + side * curveSize;
    VECTOR3 p0 = firstPos;
    VECTOR3 p1 = endPos;
    VECTOR3 p2 = endPos + dir * size * 0.2f + side * curveSize;

    movePos = CatmullRom(t, pb, p0, p1, p2);

    /*switch (cut.ease)
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
    }*/
#else 
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
#endif


   
    
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
            camera->target = keepTarget + (cut.camera.target * targetTransfrom->GetRotationMatrix());
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
    Transform* startTransform = PlayerEnemyWorldToPos(camera->cutSceneData[cutSceneIndex].firstPosBaseName);
    if (startTransform != nullptr) {
        firstPos = camera->cutSceneData[cutSceneIndex].camera.startPos * startTransform->GetRotationMatrix();
    }
    else {
        firstPos = camera->cutSceneData[cutSceneIndex].camera.startPos;
    }
  
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


VECTOR3 CutSceneCamera::CatmullRom(float _rate, VECTOR3 _pb, VECTOR3 _p0, VECTOR3 _p1, VECTOR3 _p2)
{
    float t3 = _rate * _rate * _rate; //3乗
    float t2 = _rate * _rate;//2乗
    float t1 = _rate;//1乗
    VECTOR3 rV0 = (_p1 - _pb) / 2.0f; //p0の傾き
    VECTOR3 rV1 = (_p2 - _p0) / 2.0f; //p1の傾き
    VECTOR3 a = _p0 * 2 - _p1 * 2 + rV0 + rV1;
    VECTOR3 b = _p0 * -3 + _p1 * 3 - rV0 * 2 - rV1;

    return a * t3 + b * t2 + rV0 * _rate + _p0;//三次曲線の式
}