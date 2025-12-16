#include "LoadManager.h"
#include <unordered_map>
#include <array>
#include <EffekseerForDXLib.h>
#include "Debug/Debug.h"
#include "../../Library/SceneManager.h"


namespace {

	enum class Type
	{
		NONE = 1,
		MODEL = 0,
		ANIM,
		IMAGE,
		SOUND,
		EFFECT,
	};

	struct LoadData
	{
		LoadData() {
			type = Type::NONE;
			handle = -1;
		}
		~LoadData() {
			/*switch (type)
			{
			case Type::MODEL:
			case Type::ANIM:
				MV1DeleteModel(handle);
				break;
			case Type::SOUND:
				DeleteSoundMem(handle);
				break;
			case Type::EFFECT:
				DeleteEffekseerEffect(handle);
				break;
			case Type::IMAGE:
				DeleteGraph(handle);
				break;
			default:
				my_error_assert("リソースデータ削除ミス");
			}*/
			
		}
		Type type;
		int handle;
	};

	std::unordered_map <std::string, LoadData> fileLoad;
	//std::array <std::string, (int)Type::EFFECT + 1> path;

	void ShowLoadError(const std::string& type, const std::string& name) {
		//エラーしたらメッセージボックスでエラー分を出す
		Debug::CreateMessageBox("読み込み失敗: " + name, type);
	}

	std::string ID(ID::IDType type){
		return ID::GetID(type);
	}

	std::string ID(Sound_ID::SOUND_ID type) {
		return Sound_ID::GetSoundID(type);
	}

	//int counter = 0;
	
}

void Load::Init()
{
	fileLoad.clear();
}

int Load::LoadModel(std::string path,ID::IDType id)
{
	std::string name = path;
	//LoadData data = fileLoad[name];
	if (fileLoad[name].handle == -1) {
		//拡張子はこっちで指定する分楽にできるようにする
		ID::SetID(path, id);
		path += ".mv1";
		fileLoad[name].handle = MV1LoadModel(path.c_str());
		fileLoad[name].type = Type::MODEL;
	}
	if (fileLoad[name].handle == -1) {
		ShowLoadError("Model", name);
		return -1;
		
	}
	Debug::DebugOutPutPrintf("%s : Loadしました。", name.c_str());
	return fileLoad[name].handle;
}

int Load::LoadSound(std::string path,std::string exten, Sound_ID::SOUND_ID id)
{
	std::string name = path;
	//LoadData data = fileLoad[name];
	std::string loadName = Load::SOUND_PATH + path;
	if (fileLoad[name].handle == -1) {
		//soundだけは拡張子が様々なので引数で指定
		loadName += exten;
		fileLoad[name].handle = LoadSoundMem(loadName.c_str());
		fileLoad[name].type = Type::SOUND;
		Sound_ID::SetSoundId(path, id);
	}
	if (fileLoad[name].handle == -1) {
		ShowLoadError("Sound", name);
		return -1;
	}
	/*else {
		fileLoad[name] = data;
	}*/
	Debug::DebugOutPutPrintf("%s : Loadしました。", name.c_str());
	return fileLoad[name].handle;
}

int Load::LoadEffect(std::string path, Effect_ID::EFFECT_ID id,float size)
{
	//std::string name = ID::GetEffectID(id);
	std::string name = path;
	//LoadData data = fileLoad[name];
	std::string loadName = Load::EFFECT_PATH + path;
	if (fileLoad[name].handle == -1) {
		//拡張子はこっちで指定する分楽にできるようにする
		loadName += ".efkefc";
		fileLoad[name].handle = LoadEffekseerEffect(loadName.c_str(),size);
		fileLoad[name].type = Type::EFFECT;
		Effect_ID::SetEffectID(path, id);
	}
	if (fileLoad[name].handle == -1) {
		ShowLoadError("Effect", name);
		return -1;
	}
	/*else {
		fileLoad[name] = data;
	}*/
	Debug::DebugOutPutPrintf("%s : Loadしました。", name.c_str());
	return fileLoad[name].handle;
}

int Load::LoadImageGraph(std::string path, ID::IDType id)
{
	std::string name = path;
	//LoadData data = fileLoad[name];
	if (fileLoad[name].handle == -1) {
		//拡張子はこっちで指定する分楽にできるようにする
		ID::SetID(path, id);
		path += ".png";
		fileLoad[name].handle = LoadGraph(path.c_str());
		fileLoad[name].type = Type::IMAGE;
	}
	if (fileLoad[name].handle == -1) {
		ShowLoadError("Image", name);
		return -1;
	}
	/*else {
		fileLoad[name] = data;
	}*/
	Debug::DebugOutPutPrintf("%s : Loadしました。", name.c_str());
	return fileLoad[name].handle;
}

int Load::LoadAnim(std::string path, ID::IDType id)
{
	std::string name = path;
	//LoadData data = fileLoad[name];
	if (fileLoad[name].handle == -1) {
		//拡張子はこっちで指定する分楽にできるようにする
		ID::SetID(path, id);
		path += ".mv1";
		fileLoad[name].handle = MV1LoadModel(path.c_str());
		fileLoad[name].type = Type::ANIM;
	}
	if (fileLoad[name].handle == -1) {
		ShowLoadError("Anim", name);
		return -1;
	}
	/*else {
		fileLoad[name] = data;
	}*/
	Debug::DebugOutPutPrintf("%s : Loadしました。", name.c_str());
	return fileLoad[name].handle;
}

void Load::DeleteData(ID::IDType id)
{
	//指定したIDのリソースを削除する
	std::string name = ID::GetID(id);
	auto itr = fileLoad.find(name);
	if (itr == fileLoad.end()) {
		Debug::CreateMessageBox("データが存在しない", "削除ミス");
	}
	else {
		switch (itr->second.type)
		{
		case Type::MODEL:
		case Type::ANIM:
			MV1DeleteModel(itr->second.handle);
			break;
		case Type::SOUND:
			DeleteSoundMem(itr->second.handle);
			break;
		case Type::EFFECT:
			DeleteEffekseerEffect(itr->second.handle);
			break;
		case Type::IMAGE:
			DeleteGraph(itr->second.handle);
			break;
		}
		fileLoad.erase(name);
	}
}

void Load::AllDelete()
{
	//すべてのリソースデータを削除する
	for (auto f:fileLoad) {
		switch (f.second.type)
		{
		case Type::MODEL:
		case Type::ANIM:
			MV1DeleteModel(f.second.handle);
			break;
		case Type::SOUND:
			DeleteSoundMem(f.second.handle);
			break;
		case Type::EFFECT:
			DeleteEffekseerEffect(f.second.handle);
			break;
		case Type::IMAGE:
			DeleteGraph(f.second.handle);
			break;
		default:
			my_error_assert("リソースデータ削除ミス");
		}
		Debug::DebugOutPutPrintf("%s : Deleteしました。", f.first.c_str());

	}
	fileLoad.clear();
}

int Load::GetHandle(ID::IDType id)
{

	auto it = fileLoad.find(ID::GetID(id));
	if (fileLoad.end() == it) {
		std::string str = std::to_string((int)id);
		Debug::DebugLog("そのIDはありません" + str);
		return -1;
	}
	else {
		return it->second.handle;
	}
}

int Load::GetSoundHandle(Sound_ID::SOUND_ID id)
{
	auto it = fileLoad.find(Sound_ID::GetSoundID(id));
	if (fileLoad.end() == it) {
		std::string str = std::to_string((int)id);
		Debug::DebugLog("そのサウンドデータはありません" + str);
		return -1;
	}
	else {
		return it->second.handle;
	}
}

int Load::GetEffectHandle(Effect_ID::EFFECT_ID id)
{
	auto it = fileLoad.find(Effect_ID::GetEffectID(id));
	if (fileLoad.end() == it) {
		std::string str = std::to_string((int)id);
		Debug::DebugLog("そのエフェクトデータはありません" + str);
		return -1;
	}
	else {
		return it->second.handle;
	}
}