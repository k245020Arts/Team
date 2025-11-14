#pragma once
#include <string>
#include <yaml-cpp/yaml.h>
#include <fstream>

//-----------------------------------------------------YAMLの使い方------------------------------------------------------
/*
まずはyaml-cpp-masterファイルをCドライブの直下に落とす
次にプロジェクトのプロパティーを改造する必要があって
c/c++の全般の追加のインクルードディレクトリーで\yaml-cpp-master\yaml-cpp-master\include
で最後のパスをincludeにする
c/c++のプリプロセッサのプリプロセッサ定義をYAML_CPP_STATIC_DEFINEを追加する
次にc/c++のコード生成のランタイムライブラリで 
Debugは DLL (/MDd)
 Releaseは
 /MD （マルチスレッドDLL）
のモードにする
今度はリンカーにして追加のライブラリーディレクトリーにして
debugは
\yaml-cpp-master\build\Debug
releaseは
\yaml-cpp-master\build\Release
にしてパスの設定にする
これで実行が出来ると思うのでこれを確認して行う
未解決の外部参照がでたらYAMLに対するプロパティーミスがほとんどだと思うのでそこをチェックする
*/

class YamlReader
{
public:

    /// <summary>
    /// YAMLの保存をするクラス
    /// 任意の構造体や型をstd::vectorに格納してsaveしてください
    /// なおマクロでしっかりと定義しないとエラーが出てしまうので注意
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="filename">ファイルネーム</param>
    /// <param name="vec">saveしたい型の名前</param>
    template <typename T>
    static void SaveVector(const std::string& filename, const std::vector<T>& vec) {
        YAML::Node node;

        for (const auto& item : vec) {
            node.push_back(YAML::convert<T>::encode(item)); //ここで型の認識とノードの詰め込み
        }

        std::ofstream fout(filename + ".yaml");
        if (!fout) {
            throw std::runtime_error("Failed to open file for writing: " + filename);
        }
        fout << node;
    }

    /// <summary>
    /// YAMLのLoadクラス
    /// 任意の構造体や型をvectorに格納してください
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="filename">ファイルネーム</param>
    /// <returns></returns>
    template <typename T>
    static std::vector<T> LoadVector(const std::string& filename) {
        std::vector<T> result;
        YAML::Node root = YAML::LoadFile(filename + ".yaml");
        for (const auto& node : root) {

            result.push_back(node.as<T>());//ここで型の認識とノードの書き込み
        }
        return result;
    }

    /// <summary>
    /// 上記のsaveクラスとやることは基本同じだけど
    /// 上のは
    /// 例
    /// - Alice
    /// ってなるのに対して
    /// 下はmapの第一引数のstringに値をしっかりと入れれば
    /// 例
    /// name Alice
    /// となるので視野性が上がるので基本的にはこちらを推奨するがどちらでも出来ることは出来る
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="filename">ファイルネーム</param>
    /// <param name="map">saveしたいmapデータ</param>
    template <typename T>
    static void SaveMap(const std::string& filename, const std::map<std::string, T>& map) {
        YAML::Node node;
        for (const auto& [key, val] : map) {
            YAML::Node child = YAML::convert<T>::encode(val);//ここで型の認識とノードの詰め込み
            node[key] = child;
        }
        std::ofstream fout(filename  + ".yaml");
        if (!fout) throw std::runtime_error("Failed to open file");
        fout << node;
    }

    /// <summary>
    /// 先ほどのSaveMapをloadするときにはこちらが必要
    /// 細かい説明はsaveの方に書いてあるので見てください
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="filename"></param>
    /// <returns></returns>
    template <typename T>
    static std::map<std::string, T> LoadMap(const std::string& filename) {
        std::map<std::string, T> result;
        YAML::Node root = YAML::LoadFile(filename + ".yaml");
        if (!root.IsMap()) throw std::runtime_error("YAML root is not a map");

        for (auto it = root.begin(); it != root.end(); ++it) {
            std::string key = it->first.as<std::string>();
            T val = it->second.as<T>();//ここで型の認識とノードの書き込み
            result.emplace(std::move(key), std::move(val));
        }
        return result;
    }

private:

};

//C++ のプリプロセッサは複雑なマクロの中で順番通り展開しないことがあるので、これを挟んで展開を保証する
#define EXPAND(x) x
#define GET_MACRO(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10, NAME, ...) NAME
//可変引数の個数に応じて、適切なFOR_EACHを呼び出して、各引数にactionを適用させる
#define FOR_EACH(action, ...) \
    EXPAND(GET_MACRO(__VA_ARGS__, FOR_EACH10, FOR_EACH9, FOR_EACH8, FOR_EACH7, FOR_EACH6, FOR_EACH5, FOR_EACH4, FOR_EACH3, FOR_EACH2, FOR_EACH1)(action, __VA_ARGS__))

#define FOR_EACH1(action, x1) action(x1)
#define FOR_EACH2(action, x1,x2) action(x1) action(x2)
#define FOR_EACH3(action, x1,x2,x3) action(x1) action(x2) action(x3)
// 必要に応じて最大数まで増やす

// フィールドごとの処理
//YAML_ENCODE_FIELD(field)は、YAML にフィールドを書き出す処理。
#define YAML_ENCODE_FIELD(field) node[#field] = rhs.field;//#field はプリプロセッサの文字列化マクロで、field を "field" に変換出来る。
///YAML_DECODE_FIELD(field) は、YAML からフィールドを読み込む処理。
#define YAML_DECODE_FIELD(field) rhs.field = node[#field].as<decltype(rhs.field)>();
//このマクロが実際に YAML::convert<Type> を定義する
//複数行マクロを定義したい場合は/（\）を定義してあげる
#define YAML_CONVERT_STRUCT(Type, ...) \
namespace YAML {                                           \
template <>                                                \
struct convert<Type> {                                     \
    static Node encode(const Type& rhs) {                  \
        Node node;                                         \
        FOR_EACH(YAML_ENCODE_FIELD, __VA_ARGS__)           \
        return node;                                       \
    }                                                      \
    static bool decode(const Node& node, Type& rhs) {      \
        FOR_EACH(YAML_DECODE_FIELD, __VA_ARGS__)           \
        return true;                                       \
    }                                                      \
};                                                         \
}
/// <summary>
/// テンプレートの特殊化をしてその型にだけ適応させることが出来る。
/// </summary>