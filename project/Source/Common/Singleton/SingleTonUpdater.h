#pragma once

/// <summary>
/// シングルトンの更新処理を回す場所 App.cppから直接回す
/// </summary>
class SingleTonUpdater
{
public:
	SingleTonUpdater();
	~SingleTonUpdater();

	static void Update();
	static void Draw();

	static void Release();

private:

};
