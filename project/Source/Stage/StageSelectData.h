#pragma once
#include "../Common/Singleton/SingletonBase.h"
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include "../Common/JsonReader.h"

struct StageData
{
	int id;
	std::string name;
	std::string mapFile;
	std::string bgm;
    int bossID;
};

class StageSelectData : public SingletonBase<StageSelectData>
{
public:
    void Update() override {}
    void Draw() override {}

    bool Load(const std::string& filename)
    {
        if (!jsonReader.Load(filename)) {
            return false;
        }

        auto& json = jsonReader.Data();

        // "stages" が存在するかチェック
        if (!json.contains("stages")) {
            return false;
        }

        stages.clear();

        for (const auto& elem : json["stages"])
        {
            StageData stage;

            stage.id = elem.value("id", -1);
            stage.name = elem.value("name", "");
            stage.mapFile = elem.value("map", "");
            stage.bgm = elem.value("bgm", "");
            stage.bossID = elem.value("bossID", -1);

            stages.push_back(stage);
        }

        return true;
    }

    int GetStageMax() const
    {
        return static_cast<int>(stages.size());
    }

    const StageData& GetStageData(int index) const{
        return stages.at(index);
    }

    const StageData&  GetNowStageData() const {
        return stages.at(stageID);
    }

    int GetStageID() const { return stageID; }
    void SetStageID(int id) { stageID = id; }

private:
    friend class SingletonBase<StageSelectData>;

    StageSelectData();
    ~StageSelectData() {}

    JsonReader jsonReader;
    std::vector<StageData> stages;
    int stageID;
};