#pragma once
#include "../Library/GameObject.h"
#include "ID.h"
#include "SoundID.h"
#include "EffectID.h"

namespace Load {

	//ロードを管理しているクラス。
	//初期化処理
	void Init();

	int LoadModel(std::string path, ID::IDType id);
	int LoadSound(std::string path, std::string exten, Sound_ID::SOUND_ID id);
	int LoadEffect(std::string path, Effect_ID::EFFECT_ID id, float size);
	int LoadImageGraph(std::string path, ID::IDType id);
	int LoadAnim(std::string path, ID::IDType id);

	void DeleteData(ID::IDType id);

	void AllDelete();

	int GetHandle(ID::IDType id);
	int GetSoundHandle(Sound_ID::SOUND_ID id);

	int GetEffectHandle(Effect_ID::EFFECT_ID id);
	//パスの定数
	//パスを毎回書くのは手間なので定数にして行う
	const std::string MODEL_PATH = "data/model/";
	const std::string ANIM_PATH = "data/model/animation/";
	const std::string IMAGE_PATH = "data/image/";
	const std::string SOUND_PATH = "data/sound/";
	const std::string EFFECT_PATH = "data/effect/";
}