#include "Animator.h"
#include <string>
#include <assert.h>
#include "../Library/time.h"
#include "LoadManager.h"
#include "../ImGui/imgui.h"

Animator::Animator()
{
    fileInfos.clear();
    playSpeed = 1.0f;
    blendTime = 0.0f;
    blendTimeMax = 0.0f;
    finished = true;
    debugId = 0;
    tag = Function::GetClassNameC<Animator>();
}

Animator::~Animator()
{
    if (before.attachID >= 0)
    {
        MV1DetachAnim(baseModel, before.attachID);
    }
    if (current.attachID >= 0)
    {
        MV1DetachAnim(baseModel, current.attachID);
    }
    for (auto& inf : fileInfos)
    {
        if (inf.second.hModel > 0)
        {
            MV1DeleteModel(inf.second.hModel);
            inf.second.hModel = -1;
        }
    }
    fileInfos.clear();
}

void Animator::BaseModelSet(int _model, int _root)
{
    baseModel = _model;
    rootNum = _root;
}

void Animator::BaseModelSet(int _model, std::string _rootName)
{
    baseModel = _model;
    rootNum = MV1SearchFrame(_model, _rootName.c_str());
}

void Animator::Update()
{

    //MV1ResetFrameUserLocalMatrix(baseModel, rootNum);
    //ローカルマトリックスの座標を打ち消す
    MV1ResetFrameUserLocalMatrix(baseModel, rootNum);
    MATRIX matrix = MGetIdent();
    MATRIX beforeMatrix = MGetIdent();

    VECTOR3 beforePos = currentPosition;
    if (current.attachID >= 0)
    { // current
        const FileInfo& f = fileInfos[current.fileID];
        current.beforeFrame = current.frame;
        current.frame += obj->GetObjectTimeRate() * playSpeed * f.playSpeed * 30.0f;
        if (current.frame >= f.maxFrame)
        {
            if (f.loop)
            {
                current.frame -= f.maxFrame;
            }
            else
            {
                current.frame = f.maxFrame;
                finished = true;
            }
        }
        if (fileInfos[current.fileID].eventFinishTime >= current.frame && fileInfos[current.fileID].eventStartTime <= current.frame) {
            animEventCan = true;
        }
        else {
            animEventCan = false;
        }
        if (current.boneIndex != -1) {
            currentPosition = MV1GetAttachAnimFrameLocalPosition(baseModel, current.attachID, rootNum);
            MV1SetAttachAnimTime(baseModel, current.attachID, current.frame);
            nowPosition = MV1GetAttachAnimFrameLocalPosition(baseModel, current.attachID, rootNum);
            subPosition = nowPosition - currentPosition;
        }
        else {
           MV1SetAttachAnimTime(baseModel, current.attachID, current.frame);
        }

       
        // 現在の行列を取得
        matrix = MV1GetFrameLocalMatrix(baseModel, rootNum);
        // 無補正時の座標を取得
        const VECTOR3 framePos = MV1GetAttachAnimFrameLocalPosition(baseModel, current.attachID, rootNum);

        // 座標移動を打ち消す
        matrix *= MGetTranslate(framePos * -1.0f);

            // Yだけ維持、XZを原点
        matrix *= MGetTranslate(VECTOR3(0.0f, framePos.y, 0.0f));

    }

    if (before.attachID>=0)
    { // before
        blendTime += obj->GetObjectTimeRate();
        if (blendTime >= blendTimeMax)
        {
            MV1SetAttachAnimBlendRate(baseModel, current.attachID, 1.0f);
            MV1SetAttachAnimBlendRate(baseModel, before.attachID, 0.0f);
            MV1DetachAnim(baseModel, before.attachID);
            before.attachID = -1;
            before.fileID = -1;
            return;
        }
        const FileInfo& f = fileInfos[before.fileID];
        before.frame += obj->GetObjectTimeRate() * playSpeed * f.playSpeed * 30.0f;
        if (before.frame >= f.maxFrame)
        {
            if (f.loop) {
                before.frame -= f.maxFrame;
            }
            else {
                before.frame = f.maxFrame;
            }
        }
        MV1SetAttachAnimTime(baseModel, before.attachID, before.frame);

        float rate = blendTime / blendTimeMax;
        MV1SetAttachAnimBlendRate(baseModel, current.attachID, rate);
        MV1SetAttachAnimBlendRate(baseModel, before.attachID, 1.0f - rate);

       // 前回の行列を設定
       beforeMatrix = MV1GetFrameLocalMatrix(baseModel, rootNum);

       // 無補正時の座標を取得
       const VECTOR3 framePos = MV1GetAttachAnimFrameLocalPosition(baseModel, before.attachID, rootNum);

       // 座標移動を打ち消す
       beforeMatrix *= MGetTranslate(framePos * -1.0f);

       // Yだけ維持、XZを原点
       beforeMatrix *= MGetTranslate(VECTOR3(0.0f, framePos.y, 0.0f));
    }

    // ◇前回のアニメーションが再生中なら、ブレンドする
   if (before.attachID >= 0)
   {
          // root姿勢を滑らかに遷移
       float rate = blendTime / blendTimeMax;

       // 現姿勢と前姿勢を合成
       // 最低値 + (最大値 - 最低値) * progress
       matrix = MAdd(beforeMatrix, MAdd(matrix, beforeMatrix * MGetScale(VOne * -1.0f)) * MGetScale(VOne* rate));
   }

    MV1SetFrameUserLocalMatrix(baseModel, rootNum, matrix);

    // ①前姿勢を補正
    // ②後姿勢を補正
    // ③前姿勢と後姿勢を割合でブレンド

    // 一旦リセット
    //const FileInfo& f = fileInfos[current.fileID];
    //MV1ResetFrameUserLocalMatrix(baseModel, rootNum);

    //MATRIX currentM = MGetIdent();	// 現在のアニメーションの行列
    //MATRIX beforeM = MGetIdent();		// 前回のアニメーションの行列

    //// ◇前回のアニメーションが再生中なら
    //if (current.attachID >= 0) {
    //    // ▽補間処理
    //    {
    //        blendTime += obj->GetObjectTimeRate() * playSpeed;

    //        if (blendTime >= blendTimeMax) {
    //            MV1DetachAnim(baseModel, before.attachID);
    //            MV1SetAttachAnimBlendRate(baseModel, current.attachID, 1.0f);
    //            before.attachID = -1;
    //        }
    //        else {
    //            float rate = blendTime / blendTimeMax;
    //            MV1SetAttachAnimBlendRate(baseModel, before.attachID, 1.0f - rate);
    //            MV1SetAttachAnimBlendRate(baseModel, current.attachID, rate);
    //        }
    //    }

    //    // 前回の行列を設定
    //    beforeM = MV1GetFrameLocalMatrix(baseModel, rootNum);

    //    // 無補正時の座標を取得
    //    const VECTOR3 framePos = MV1GetAttachAnimFrameLocalPosition(baseModel, before.attachID, rootNum);

    //    // 座標移動を打ち消す
    //    beforeM *= MGetTranslate(framePos * -1.0f);

    //    // Yだけ維持、XZを原点
    //    beforeM *= MGetTranslate(VECTOR3(0.0f, framePos.y, 0.0f));
    //}

    //// ◇アニメーションが再生中なら
    //if (current.attachID >= 0) {

    //    current.beforeFrame = current.frame;
    //    current.frame += obj->GetObjectTimeRate() * playSpeed * f.playSpeed * 30.0f;

    //    // アニメーションが総再生フレームまで再生したら
    //    if (current.frame >= f.maxFrame) {

    //        if (!f.loop)
    //            current.frame = f.maxFrame;
    //        else {
    //            current.frame -= f.maxFrame;

    //        }
    //    }

    //    // アニメーションを適応させる
    //    MV1SetAttachAnimTime(baseModel, current.attachID, current.frame);

    //    // 現在の行列を取得
    //    currentM = MV1GetFrameLocalMatrix(baseModel, rootNum);

    //    if (fileInfos[current.fileID].eventFinishTime >= current.frame && fileInfos[current.fileID].eventStartTime <= current.frame) {
    //        animEventCan = true;
    //    }
    //    else {
    //        animEventCan = false;
    //    }

    //    // ◇ローカル座標の固定化が有効なら
    //   /* if (anims[playingLabel].isFixedRoot)
    //    {*/
    //        // 無補正時の座標を取得
    //        const VECTOR3 framePos = MV1GetAttachAnimFrameLocalPosition(baseModel, current.attachID, rootNum);

    //        // 座標移動を打ち消す
    //        currentM *= MGetTranslate(framePos * -1.0f);

    //        // Yだけ維持、XZを原点
    //        currentM *= MGetTranslate(VECTOR3(0.0f, framePos.y, 0.0f));
    //   // }

    //    // ◇前回のアニメーションが再生中なら、ブレンドする
    //    if (before.attachID >= 0)
    //    {
    //        // root姿勢を滑らかに遷移
    //        float progress = blendTime / blendTimeMax;

    //        // 現姿勢と前姿勢を合成
    //        // 最低値 + (最大値 - 最低値) * progress
    //        currentM = MAdd(beforeM, MAdd(currentM, beforeM * MGetScale(VOne * -1.0f)) * MGetScale(VOne* progress));
    //    }

    //    // セット
    //    MV1SetFrameUserLocalMatrix(baseModel, rootNum, currentM);
    //}
}

void Animator::AddFile(ID::IDType id, std::string filename, bool loop, float speed, float _eventStart, float _eventFinish)
{
    FileInfo inf;
    std::string str = ID::GetID(id);
    inf.hModel = Load::LoadAnim(Load::ANIM_PATH + filename, id);
    assert(inf.hModel > 0);
    inf.loop = loop;
    inf.playSpeed = speed;
    inf.eventStartTime = _eventStart;
    inf.eventFinishTime = _eventFinish;
    if (inf.hModel > 0)
    {
        inf.maxFrame = MV1GetAnimTotalTime(inf.hModel, 0);
    }
    if (fileInfos.count(str) > 0)
    {
        MessageBox(nullptr,
            (std::string("ID=") + (str)+"は使われています").c_str(),
            "Animator", MB_OK);
        assert(false);
    }
    fileInfos.emplace(str, inf); // 登録
}

void Animator::SetMaxFrame(ID::IDType id, float _maxTime)
{
    fileInfos[ID::GetID(id)].maxFrame = _maxTime;
}

void Animator::Play(ID::IDType id, float margeTime)
{
    std::string str = ID::GetID(id);
    if (current.fileID == str)
    {
        return;
    }
    if (before.attachID >= 0)
    {
        MV1DetachAnim(baseModel, before.attachID);
        before.attachID = -1;
        before.fileID = -1;
    }
    if (current.attachID >= 0)
    {
        before = current;
        blendTimeMax = margeTime;
        blendTime = 0.0f;
    }
    else
    {
        blendTimeMax = 0.0f;
    }
    current.fileID = str;
    if (fileInfos.count(str) > 0)
    {
        current.attachID = MV1AttachAnim(baseModel, 0, fileInfos[str].hModel);
        current.frame = 0.0f;
        current.beforeFrame = 0.0f;
        finished = false;
        MV1SetAttachAnimTime(baseModel, current.attachID, 0.0f);
        if (before.attachID >= 0)
        {
            MV1SetAttachAnimBlendRate(baseModel, current.attachID, 0.0f);
            MV1SetAttachAnimBlendRate(baseModel, before.attachID, 1.0f);
        }
        else 
        {
            MV1SetAttachAnimBlendRate(baseModel, current.attachID, 1.0f);
        }
    }
}

void Animator::SetPlaySpeed(float speed)
{
    playSpeed = speed;
}

float Animator::GetPlaySpeed()
{
    return playSpeed;
}

bool Animator::IsFinish()
{
    return finished;
}

std::string Animator::GetCurrentID()
{
    return current.fileID;
}

float Animator::GetCurrentFrame()
{
    return current.frame;
}

float Animator::GetCurrentBeforeFrame()
{
    return current.beforeFrame;
}

float Animator::GetMaxFrame()
{
    if (current.attachID >= 0)
    {
        return fileInfos[current.fileID].maxFrame;
    }
    return 0.0f;
}

void Animator::SetBoneFrame(int _index)
{
    current.boneIndex = _index;
}

VECTOR3 Animator::BoneMovePositionAdd(VECTOR3 _position)
{
    return _position + subPosition;
}

VECTOR3 Animator::BoneNowPosition()
{
    MATRIX matrix = MV1GetFrameLocalWorldMatrix(baseModel, current.boneIndex);

   /* MV1SetMatrix(baseModel, matrix);

    VECTOR3 Vect = VGet(0.0f, 0.0f, 0.0f);
    Vect = VTransform(Vect, matrix);*/
    return nowPosition * matrix;
}

void Animator::SetFrame(float _frame)
{
    current.frame = _frame;
}

int Animator::GetBaseModel()
{
    return baseModel;
}

int Animator::GetCurrentAttackID()
{
    return current.attachID;
}

void Animator::ImguiDraw()
{
    ImGui::Text("frame : %.1f", current.frame);
    if (ImGui::TreeNode("AnimInfo")) {
        for (auto file : fileInfos) {
            ImGui::Text("%s : %f", file.first.c_str(), file.second.maxFrame);
        }
        ImGui::TreePop();
    }
}

VECTOR3 Animator::BoneMovePositionAdd()
{
    return subPosition;
}