#pragma once
#include <map>
#include "../../../Library/myDxLib.h"
#include <string>
#include "../Component.h"
#include "../../Common/ID/ID.h"

/// <summary>
/// アニメーションを再生する
/// </summary>
class Animator : public Component
{
public:
	/// <summary>
	/// アニメーションクラスを作る
	/// </summary>
	/// <param name="model">アニメーションを付けるモデル</param>
	Animator();
	~Animator();

	void BaseModelSet(int _model, int _root);
	void BaseModelSet(int _model, std::string _rootName);

	/// <summary>
	/// アニメーションの再生をする
	/// </summary>
	void Update();

	/// <summary>
	/// アニメーションファイルを追加する
	/// </summary>
	/// <param name="id">アニメーション番号</param>
	/// <param name="filename">アニメーションファイル</param>
	/// <param name="loop">ループする場合はtrue</param>
	/// <param name="speed">再生倍率（基本は1.0）</param>
	void AddFile(ID::IDType id, std::string filename, bool loop,float speed = 1.0f,float _eventStart = -1.0f,float _eventFinish = -1.0f);

	/// <summary>
	/// 最大タイムをデフォルトタイムから変えたいときに設定する。
	/// </summary>
	/// <param name="id"></param>
	/// <param name="_maxTime"></param>
	void SetMaxFrame(ID::IDType id,float _maxTime);

	/// <summary>
	/// アニメーションを再生する
	/// </summary>
	/// <param name="id">再生するアニメーションの番号</param>
	/// <param name="margeTime">前のアニメーションとのブレンド時間</param>
	void Play(ID::IDType id, float margeTime = 0.2f);

	/// <summary>
	/// アニメーションの再生速度を変更する
	/// </summary>
	/// <param name="speed">再生速度（倍率）</param>
	void SetPlaySpeed(float speed);

	/// <summary>
	/// 今のアニメーションの再生速度を返す
	/// </summary>
	/// <returns>再生速度（倍率）</returns>
	float GetPlaySpeed();

	/// <summary>
	/// アニメーションが終ったか調べる
	/// ループしないアニメーションが、最後のフレームまで再生したものを、終りとする
	/// </summary>
	/// <returns>終っていればtrue</returns>
	bool IsFinish();

	/// <summary>
	/// 現在再生しているアニメーション番号
	/// </summary>
	/// <returns>アニメーション番号</returns>
	std::string GetCurrentID();

	/// <summary>
	/// 現在の再生フレーム
	/// </summary>
	/// <returns>再生フレーム</returns>
	float GetCurrentFrame();
	float GetCurrentBeforeFrame();

	/// <summary>
	/// 現在再生中アニメーションの最大フレームを返す
	/// </summary>
	/// <returns>最大フレーム</returns>
	float GetMaxFrame();

	/// ボーン番号を引数に入れるとそのローカルポジションが次のアップデートから得られるようになる。
	void SetBoneFrame(int _index);

	/// アニメーションでもし移動する場合があった時にそのボーンの位置にポジションを合わせなければいけないので
	/// その時にこの関数を使う。
	/// これの返り値の座標引く動かしたい座標をすればその位置が帰ってくる。
	VECTOR3 BoneMovePositionAdd();
	VECTOR3 BoneMovePositionAdd(VECTOR3 _position);
	VECTOR3 BoneNowPosition();

	bool AnimEventCan() { return animEventCan; }
	void AnimEventReset() { animEventCan = false; }
	float EventStartTime(ID::IDType id) { return fileInfos[ID::GetID(id)].eventStartTime; }
	float EventFinishTime(ID::IDType id) { return fileInfos[ID::GetID(id)].eventFinishTime; }

	void SetFrame(float _frame);

	int GetBaseModel();
	int GetCurrentAttackID();

	bool BlendFinish();

	void ImguiDraw()override;

private:
	int baseModel;
	int rootNum;
	struct FileInfo {
		int hModel;
		bool loop;
		float maxFrame;
		float playSpeed;
		float eventStartTime;
		float eventFinishTime;
		
		FileInfo() : hModel(-1), loop(false), maxFrame(1.0f), playSpeed(1.0f), eventFinishTime(-1.0f), eventStartTime(-1.0f){}
	};
	std::map<std::string, FileInfo> fileInfos;

	struct PlayInfo {
		std::string fileID;
		int attachID;
		float frame;
		float beforeFrame;
		int boneIndex;
		

		PlayInfo() : fileID("NONE"), attachID(-1), frame(0.0f),beforeFrame(0.0f), boneIndex(-1){}
	};
	PlayInfo current; // 今のアニメ
	PlayInfo before; // 前のアニメ

	float blendTime; // ブレンドのタイマー
	float blendTimeMax; // ブレンドのタイマー最大値

	bool finished;
	float playSpeed;

	VECTOR3 currentPosition;
	VECTOR3 nowPosition;
	VECTOR3 subPosition;

	bool animEventCan;
};