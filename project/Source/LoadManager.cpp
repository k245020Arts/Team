#include "LoadManager.h"
#include <unordered_map>
#include <array>
#include <EffekseerForDXLib.h>
#include "Debug.h"
#include "../Library/SceneManager.h"

namespace {

	enum class Type
	{
		MODEL = 0,
		ANIM,
		IMAGE,
		SOUND,
		EFFECT,
	};

	struct LoadData
	{
		Type type;
		int handle = -1;
	};

	std::unordered_map <std::string, LoadData> fileLoad;
	//std::array <std::string, (int)Type::EFFECT + 1> path;

	void ShowLoadError(const std::string& type, const std::string& name) {
		Debug::CreateMessageBox("読み込み失敗: " + name, type);
	}

	std::string ID(ID::IDType type){
		return ID::GetID(type);
	}

	std::string ID(ID::SOUND_ID type) {
		return ID::GetSoundID(type);
	}
}

void Load::Init()
{
	fileLoad.clear();
}

int Load::LoadModel(std::string path,ID::IDType id)
{
	std::string name = ID::GetID(id);
	LoadData data = fileLoad[name];
	if (data.handle == -1) {
		path += ".mv1";
		data.handle = MV1LoadModel(path.c_str());
		data.type = Type::MODEL;
	}
	if (data.handle == -1) {
		ShowLoadError("Model", name);
		return -1;
		
	}
	else {
		fileLoad[name] = data;
	}
	return data.handle;
}

int Load::LoadSound(std::string path,std::string exten, ID::SOUND_ID id)
{
	std::string name = path;
	LoadData data = fileLoad[name];
	std::string loadName = Load::SOUND_PATH + path;
	if (data.handle == -1) {
		loadName += exten;
		data.handle = LoadSoundMem(loadName.c_str());
		data.type = Type::SOUND;
		ID::SetSoundId(path, id);
	}
	if (data.handle == -1) {
		ShowLoadError("Sound", name);
		return -1;
	}
	else {
		fileLoad[name] = data;
	}
	return data.handle;
}

int Load::LoadEffect(std::string path, ID::EFFECT_ID id,float size)
{
	//std::string name = ID::GetEffectID(id);
	std::string name = path;
	LoadData data = fileLoad[name];
	std::string loadName = Load::EFFECT_PATH + path;
	if (data.handle == -1) {
		loadName += ".efkefc";
		data.handle = LoadEffekseerEffect(loadName.c_str(),size);
		data.type = Type::EFFECT;
		ID::SetEffectID(path, id);
	}
	if (data.handle == -1) {
		ShowLoadError("Effect", name);
		return -1;
	}
	else {
		fileLoad[name] = data;
	}
	return data.handle;
}

int Load::LoadImageGraph(std::string path, ID::IDType id)
{
	std::string name = ID::GetID(id);
	LoadData data = fileLoad[name];
	if (data.handle == -1) {
		path += ".png";
		data.handle = LoadGraph(path.c_str());
		data.type = Type::IMAGE;
	}
	if (data.handle == -1) {
		ShowLoadError("Image", name);
		return -1;
	}
	else {
		fileLoad[name] = data;
	}
	return data.handle;
}

int Load::LoadAnim(std::string path, ID::IDType id)
{
	std::string name = ID::GetID(id);
	LoadData data = fileLoad[name];
	if (data.handle == -1) {
		path += ".mv1";
		data.handle = MV1LoadModel(path.c_str());
		data.type = Type::ANIM;
	}
	if (data.handle == -1) {
		ShowLoadError("Anim", name);
		return -1;
	}
	else {
		fileLoad[name] = data;
	}
	return data.handle;
}

void Load::DeleteData(ID::IDType id)
{
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
		}
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

int Load::GetSoundHandle(ID::SOUND_ID id)
{
	auto it = fileLoad.find(ID::GetSoundID(id));
	if (fileLoad.end() == it) {
		std::string str = std::to_string((int)id);
		Debug::DebugLog("そのサウンドデータはありません" + str);
		return -1;
	}
	else {
		return it->second.handle;
	}
}

int Load::GetEffectHandle(ID::EFFECT_ID id)
{
	auto it = fileLoad.find(ID::GetEffectID(id));
	if (fileLoad.end() == it) {
		std::string str = std::to_string((int)id);
		Debug::DebugLog("そのエフェクトデータはありません" + str);
		return -1;
	}
	else {
		return it->second.handle;
	}
}
