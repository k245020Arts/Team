#pragma once
#include "../../Library/GameObject.h"
#include "ID/ID.h"
#include "ID/SoundID.h"
#include "ID/EffectID.h"
/// <summary>
/// ロードを一括で管理する名前空間
/// map上で管理されてそこからGetHandleをすることによって画像データを得ることが出来る
/// </summary>

namespace Load {

	//ロードを管理しているクラス。
	//初期化処理
	void Init();
	/// <summary>
	/// モデルのロードをする
	/// </summary>
	/// <param name="path"></param>
	/// <param name="id"></param>
	/// <returns></returns>
	int LoadModel(std::string path, ID::IDType id);
	/// <summary>
	/// サウンドのロードをする
	/// </summary>
	/// <param name="path"></param>
	/// <param name="exten"></param>
	/// <param name="id"></param>
	/// <returns></returns>
	int LoadSound(std::string path, std::string exten, Sound_ID::SOUND_ID id);
	/// <summary>
	/// エフェクトのロードをする
	/// </summary>
	/// <param name="path"></param>
	/// <param name="id"></param>
	/// <param name="size"></param>
	/// <returns></returns>
	int LoadEffect(std::string path, Effect_ID::EFFECT_ID id, float size);
	/// <summary>
	/// 画像データのロードをする
	/// </summary>
	/// <param name="path"></param>
	/// <param name="id"></param>
	/// <returns></returns>
	int LoadImageGraph(std::string path, ID::IDType id);
	/// <summary>
	/// アニメーションデータのロードをする
	/// </summary>
	/// <param name="path"></param>
	/// <param name="id"></param>
	/// <returns></returns>
	int LoadAnim(std::string path, ID::IDType id);

	/// <summary>
	/// 指定したデータの削除
	/// </summary>
	/// <param name="id"></param>
	void DeleteData(ID::IDType id);

	/// <summary>
	/// すべてのデータを削除
	/// </summary>
	void AllDelete();
	/// <summary>
	/// 指定したIDのデータを取得
	/// </summary>
	/// <param name="id"></param>
	/// <returns></returns>
	int GetHandle(ID::IDType id);
	int GetSoundHandle(Sound_ID::SOUND_ID id);

	int GetEffectHandle(Effect_ID::EFFECT_ID id);
	//int GetCounter();
	//パスの定数
	//パスを毎回書くのは手間なので定数にして行う
	const std::string MODEL_PATH = "data/model/";
	const std::string ANIM_PATH = "data/model/animation/";
	const std::string IMAGE_PATH = "data/image/";
	const std::string SOUND_PATH = "data/sound/";
	const std::string EFFECT_PATH = "data/effect/";
}