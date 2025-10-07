#pragma once
#include "../Library/GameObject.h"
#define DebugIf (1);
#include <stdio.h>
#include <stdarg.h>
#include <tchar.h>

class InputManager;

namespace Debug {

	//デバック用機能を備えたクラス

	/// <summary>///
	/// MassageBoxを簡易的に使いやすくしたものエラー文とタイトルを指定してメッセージを出すことが可能
	/// 本来のMassageBoxはさらにmassageに対するアクションを変えることが出来るが基本変えないので抜きました
	/// </summary>/// 
	/// <param name="_error">エラー文</param>
	/// /// <param name="_title">左上に出る文章</param>

	void CreateMessageBox(const std::string& _error, const std::string& _title);

	void InitDebug(InputManager* _input);

	void UpdateDebug();

	void DebugUpdate();

	void DebugLog(std::string _log);
	void DebugLogPrintfArgs(const TCHAR* _log,...);
	void DebugLogPrintf(const TCHAR* _log);

	void UpdateLogger();

	void ClearLogger();
	bool ShakeStop();

	bool SphereColliderDraw();
	bool AABBColliderDraw();
	bool ModelColliderDraw();
	bool RayColliderDraw();
	void DrawMemory();

	inline const TCHAR* printfString(const TCHAR* _format, ...) {
		static TCHAR strPrintf[1024];
		va_list args;
		va_start(args, _format);
		_vstprintf_s(strPrintf, _format, args);
		return strPrintf;
	}
}