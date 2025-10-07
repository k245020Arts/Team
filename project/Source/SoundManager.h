#pragma once
#include "../Library/GameObject.h"
#include <unordered_map>
#include "ID.h"

class Sound;

class SoundManager : public GameObject
{
public:
	SoundManager();
	~SoundManager();
	void Update()override;
	void Draw() override;

	/// <summary>
	/// soundをロードする関数
	/// </summary>
	/// <param name="_type">種類</param>
	/// <param name="path">ファイルパス</param>
	/// <param name="exten">拡張子</param>
	/// <param name="volume">0～255を指定</param>
	void SoundLoad(ID::SOUND_ID _type,std::string path,std::string exten,int volume);


	void PlaySceneLoad();
	void TitleSceneLoad();
	void CommonSceneLoad();
	/// <summary>
	/// SEを再生させる
	/// </summary>
	/// <param name="_type"></param>
	void PlaySe(ID::SOUND_ID _type);
	//BGMを再生させる
	void PlayBGM(ID::SOUND_ID _type, bool _loop, bool firstPlay);
	//BGMを止める
	void StopBGM(ID::SOUND_ID _type);
	void StopSE(ID::SOUND_ID _type);
	/// <summary>
	/// データのハンドルを削除する
	/// </summary>
	void AllDeleteSound();
	/// <summary>
	/// ランダムで音を再生したいときに使う。
	/// ファイル名　＋　数字で
	/// 連番の時だけできるような仕組みになっている
	/// </summary>
	/// <param name="_name">ファイルネーム(数字はのぞく)</param>
	/// <param name="num">最大の数字</param>
	void RandamSe(std::string _name, int num);

	//音が再生させているかどうかを調べる
	bool CheckSe(ID::SOUND_ID _id);


private:
	
	std::unordered_map<std::string,Sound*> sound;
};