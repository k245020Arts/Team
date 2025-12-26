#pragma once
#include  <vector>
#include <string>
#include <nlohmann/json.hpp>

using JSON = nlohmann::json;


class JsonReader
{
public:
    bool Load(const std::string& filename);// data_ に読み込む

    bool Save(const std::string& filename, JSON& newData) const;// 上書き保存

    // 読み書き用アクセサ
   JSON& Data(){ return data;}

private:
    JSON data;

};