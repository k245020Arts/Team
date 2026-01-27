#pragma once
#include "../../../Library/GameObject.h"
#include <unordered_map>
#include "../ID/SoundID.h"

class Sound;
class BaseObject;

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
	void SoundLoad(Sound_ID::SOUND_ID _type,std::string path,std::string exten,int volume);


	void PlaySceneLoad();
	void TitleSceneLoad();
	void CommonSceneLoad();
	/// <summary>
	/// SEを再生させる
	/// </summary>
	/// <param name="_type"></param>
	void PlaySe(Sound_ID::SOUND_ID _type);
	//BGMを再生させる
	void PlayBGM(Sound_ID::SOUND_ID _type, bool _loop, bool firstPlay);
	//BGMを止める
	void StopBGM(Sound_ID::SOUND_ID _type);
	void StopSE(Sound_ID::SOUND_ID _type);
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
	bool CheckSe(Sound_ID::SOUND_ID _id);

	void FeedInStart(Sound_ID::SOUND_ID _id,float _time);
	void FeedInOut(Sound_ID::SOUND_ID _id, float _time);

	void SetFeedInOutList(Sound* sound);
	void ChangeSound(Sound_ID::SOUND_ID _id,float _change);
	void BaseVolumeChange(Sound_ID::SOUND_ID _id);
	/// <summary>
	/// 3Dサウンドを使いたいときはこの関数を使う
	/// </summary>
	/// <param name="_id">サウンドID</param>
	/// <param name="_targetObj">ここから音が始まるもととなるObject</param>
	/// <param name="_maxVolSize">音が聞こえる最大の距離、これより離れると音はならない</param>
	/// <param name="_minVolSize">音の最大音量の距離、これより近づいても最大の音量で流れる。</param>
	void Play3DSound(Sound_ID::SOUND_ID _id, BaseObject* _targetObj,float _maxVolSize,float _minVolSize);
	void Base3DSoundObject(BaseObject* _base);

	/// <summary>
	/// 周波数をランダムで変更させて音を流したいときに使用する関数
	/// </summary>
	/// <param name="_id">sound_id</param>
	/// <param name="_ramdom">どれくらいの振幅か</param>
	/// <param name="_baseFrequ">元となる周波数</param>
	void PlayRamdomChangeFrequencySe(Sound_ID::SOUND_ID _id,int _ramdom,int _baseFrequ);

private:
	
	std::unordered_map<std::string,Sound*> sound;

	std::list<Sound*> feedInOutList;
	BaseObject* Base3DSoundObj;
};