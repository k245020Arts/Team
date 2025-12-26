#include "JsonReader.h"
#include <fstream>

bool JsonReader::Load(const std::string& filename)
{
    std::ifstream ifs(filename);//読み込み用のファイルストリーム

    if (!ifs.is_open()) {
        return false; // ファイルが開けない
    }
    try {
        ifs >> data;// ファイルが開けたらdataいれる
    }
    catch (const JSON::parse_error&) {
        data.clear();
        return false; // JSONパース失敗
    }
    return true;
}

bool JsonReader::Save(const std::string& filename, JSON& newData) const
{
    std::ofstream ofs(filename);// 書き込み用のファイルストリーム
    if (!ofs.is_open()) {
        return false;
    }
    ofs << newData.dump(2); // インデント2で整形
    return static_cast<bool>(ofs);
}

