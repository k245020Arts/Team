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
	std::unordered_map <std::string, LoadData> commonFileLoad;
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

// 共通データと通常データを管理するLoadクラスの例

int Load::LoadModel(std::string path, ID::IDType id) {
    return LoadModel(path, id, false);
}

int Load::LoadModel(std::string path, ID::IDType id, bool _common) {
    std::string name = path;
    auto& targetLoad = _common ? commonFileLoad : fileLoad;

    if (targetLoad[name].handle == -1) {
        ID::SetID(path, id);
        path += ".mv1";
        targetLoad[name].handle = MV1LoadModel(path.c_str());
        targetLoad[name].type = Type::MODEL;
    }

    if (targetLoad[name].handle == -1) {
        ShowLoadError("Model", name);
        return -1;
    }

    Debug::DebugOutPutPrintf("%s : Loadしました。", name.c_str());
    return targetLoad[name].handle;
}

int Load::LoadSound(std::string path, std::string exten, Sound_ID::SOUND_ID id) {
    // 通常版は _common = false
    return LoadSound(path, exten, id, false);
}

int Load::LoadSound(std::string path, std::string exten, Sound_ID::SOUND_ID id, bool _common) {
    std::string name = path;
    auto& targetLoad = _common ? commonFileLoad : fileLoad;

    if (targetLoad[name].handle == -1) {
        std::string loadName = Load::SOUND_PATH + path + exten;
        targetLoad[name].handle = LoadSoundMem(loadName.c_str());
        targetLoad[name].type = Type::SOUND;
        Sound_ID::SetSoundId(path, id);
    }

    if (targetLoad[name].handle == -1) {
        ShowLoadError("Sound", name);
        return -1;
    }

    Debug::DebugOutPutPrintf("%s : Loadしました。", name.c_str());
    return targetLoad[name].handle;
}

int Load::LoadEffect(std::string path, Effect_ID::EFFECT_ID id, float size) {
    return LoadEffect(path, id, size, false);
}


int Load::LoadEffect(std::string path, Effect_ID::EFFECT_ID id, float size, bool _common) {
    std::string name = path;
    auto& targetLoad = _common ? commonFileLoad : fileLoad;

    if (targetLoad[name].handle == -1) {
        std::string loadName = Load::EFFECT_PATH + path + ".efkefc";
        targetLoad[name].handle = LoadEffekseerEffect(loadName.c_str(), size);
        targetLoad[name].type = Type::EFFECT;
        Effect_ID::SetEffectID(path, id);
    }

    if (targetLoad[name].handle == -1) {
        ShowLoadError("Effect", name);
        return -1;
    }

    Debug::DebugOutPutPrintf("%s : Loadしました。", name.c_str());
    return targetLoad[name].handle;
}

int Load::LoadImageGraph(std::string path, ID::IDType id) {
    return LoadImageGraph(path, id, false);
}

int Load::LoadImageGraph(std::string path, ID::IDType id, bool _common) {
    std::string name = path;
    auto& targetLoad = _common ? commonFileLoad : fileLoad;

    if (targetLoad[name].handle == -1) {
        ID::SetID(path, id);
        std::string fullPath = path + ".png";
        targetLoad[name].handle = LoadGraph(fullPath.c_str());
        targetLoad[name].type = Type::IMAGE;
    }

    if (targetLoad[name].handle == -1) {
        ShowLoadError("Image", name);
        return -1;
    }

    Debug::DebugOutPutPrintf("%s : Loadしました。", name.c_str());
    return targetLoad[name].handle;
}

int Load::LoadAnim(std::string path, ID::IDType id) {
    return LoadAnim(path, id, false);
}

int Load::LoadAnim(std::string path, ID::IDType id, bool _common) {
    std::string name = path;
    auto& targetLoad = _common ? commonFileLoad : fileLoad;

    if (targetLoad[name].handle == -1) {
        ID::SetID(path, id);
        path += ".mv1";
        targetLoad[name].handle = MV1LoadModel(path.c_str());
        targetLoad[name].type = Type::ANIM;
    }

    if (targetLoad[name].handle == -1) {
        ShowLoadError("Anim", name);
        return -1;
    }

    Debug::DebugOutPutPrintf("%s : Loadしました。", name.c_str());
    return targetLoad[name].handle;
}

// 共通と通常の両方から検索するGetHandle
int Load::GetHandle(ID::IDType id) {
    std::string name = ID::GetID(id);

    auto it = fileLoad.find(name);
    if (it != fileLoad.end()) {
        return it->second.handle;
    }

    it = commonFileLoad.find(name);
    if (it != commonFileLoad.end()) {
        return it->second.handle;
    };

    Debug::DebugLog("そのIDはありません: " + std::to_string((int)id));
    return -1;
}

int Load::GetSoundHandle(Sound_ID::SOUND_ID id) {
    std::string name = Sound_ID::GetSoundID(id);

    auto it = fileLoad.find(name);
    if (it != fileLoad.end()) {
        return it->second.handle;
    }

    it = commonFileLoad.find(name);
    if (it != commonFileLoad.end()) {
        return it->second.handle;
    }

    Debug::DebugLog("そのサウンドデータはありません: " + std::to_string((int)id));
    return -1;
}

int Load::GetEffectHandle(Effect_ID::EFFECT_ID id) {
    std::string name = Effect_ID::GetEffectID(id);

    auto it = fileLoad.find(name);
    if (it != fileLoad.end()) {
        return it->second.handle;
    }

    it = commonFileLoad.find(name);
    if (it != commonFileLoad.end()) {
        return it->second.handle;
    }

    Debug::DebugLog("そのエフェクトデータはありません: " + std::to_string((int)id));
    return -1;
}

// DeleteDataで_commonフラグ対応
void Load::DeleteData(ID::IDType id, bool _common) {
    std::string name = ID::GetID(id);
    auto& targetLoad = _common ? commonFileLoad : fileLoad;

    auto itr = targetLoad.find(name);
    if (itr == targetLoad.end()) {
        Debug::CreateMessageBox("データが存在しない", "削除ミス");
        return;
    }

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
    targetLoad.erase(itr);
}

// fileLoadのみ全削除
void Load::FileLoadClear() {
    for (auto& f : fileLoad) {
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
    }
    fileLoad.clear();
}

// 共通も含めて全削除
void Load::AllDelete() {
    FileLoadClear();
    for (auto& f : commonFileLoad) {
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
    }
    commonFileLoad.clear();    
}

void Load::SetAsync(bool _async)
{
	SetUseASyncLoadFlag(_async);
}

bool Load::IsLoading()
{
	return (GetASyncLoadNum() > 0);
}