#pragma once
#include <random>
#include <vector>

//std::randomを使用して乱数を生成するヘッダー
namespace Random {

    //任意でシードを指定、未指定ならランダムシード
	void Init(unsigned int seed = std::random_device{}());

    // ===== 基本分布 =====▼

	// 一様分布の乱数を生成...0.0から1.0の範囲で値をランダムに返す
    double GetReal();      

	// 一様整数...乱数を生成...minからmaxの範囲で値をランダムに返す
    int GetInt(int min, int max); 

    // 重み付き選択...各要素の重みを指定し、確率に応じてインデックスを返す。
    int GetWeightedIndex(const std::vector<double>& weights);

    // ===== 追加分布 =====▼
  
	// 正規分布...平均値と標準偏差を指定して乱数を生成
    double GetNormal(double mean, double stddev);

	// ベルヌーイ分布...確率pでtrueを返す...0.3なら30%の確率でtrueを返す
    bool GetBernoulli(double p);       

	// 指数分布...平均値lambdaを指定して乱数を生成
    double GetExponential(double lambda);           

    // ===== ユーティリティ =====
	unsigned int GetSeed();// 現在のシード値を取得
};