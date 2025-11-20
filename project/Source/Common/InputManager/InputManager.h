#pragma once
#include <list>
#include "../../../Library/gameObject.h"
#include "controllerInputManager.h"
#include "keyboardInputManager.h"
#include "mouseInputManager.h"
#include <unordered_map>

class ControllerInputManager;
class KeyboardInputManager;

/// <summary>
/// キーコンフィグ用のデータ
/// これを使うと直接ボタンの種類をやるのではなく、文字列でキーボード、コントローラー、マウスが対応可能になる
/// </summary>
struct KeyConfigData
{
	
	std::string action;
	int keyNumber;
	int padNumber;
	MouseInputManager::MouseButton mouceNumber;

	KeyConfigData() { 
		MouseInputManager::MouseButton m = MouseInputManager::MouseButton::MOUSE_NONE;
		KeyConfigData k =  KeyConfigData("", -1, -1, m);
	}

	KeyConfigData(std::string _act, int _key, int _pad, MouseInputManager::MouseButton _mouce) {
		action = _act;
		keyNumber = _key;
		padNumber = _pad;
		mouceNumber = _mouce;
	}
};

namespace
{
	//何フレーム前の入力まで覚えておくか
	const int SAVE_FRAME_MAX = 600;
	//スティック入力の最大値
	const int STICK_INPUT_MAX = 32767;
	//入力を受け付けない値
	const int NOT_INPUT = -1;
}

/// <summary>
/// プレイヤーの操作の入力を管理するクラス
///	シーンを跨いで存在できるようにしているためBootSceneで生成推奨
/// </summary>
class InputManager :public GameObject
{
public:
	/// <summary>
	/// 使用したいデバイスのみ生成推奨
	/// 生成したデバイスはGet関数で取得可能
	/// </summary>
	/// <param name="_useController">コントローラー入力を取るか</param>
	/// <param name="_useKeyboard">キーボード入力を取るか</param>
	/// <param name="_useMouse">マウス入力を取るか</param>
	InputManager(bool _useController,bool _useKeyboard,bool _useMouse);
	~InputManager();
	void Update()override;

	///<summary>指定したキーボードのキーかコントローラーのボタンが押されている間Trueを返す</summary>
	//bool GetIsKeyOrButtonPushingNow(int _key, int _button);
	///<summary>指定したキーボードのキーかコントローラーのボタンの押し始め1F目のみTrueを返す</summary>
	//bool GetIsKeyOrButtonPutNow(int _key, int _button);
	///<summary>指定したキーボードのキーかコントローラーのボタンを押して離し始めた1F目のみTrueを返す</summary>
	//bool GetIsKeyOrButtonReleaseNow(int _key, int _button);

	ControllerInputManager* GetControllerInput();
	KeyboardInputManager* GetKeyboardInput();
	MouseInputManager* GetMouseInput();
	/// <summary>
	/// キーコンフィグ用データを生成する。
	/// </summary>
	/// <param name="_data"></param>
	void CreateInputData(KeyConfigData _data);
	//キーコンフィグ用のボタンの種類
	bool KeyInputDown(std::string _name);//最初の一回のみ
	bool KeyInput(std::string _name);//入力し続けている間
	bool KeyInputUp(std::string _name);//話した瞬間

	bool IsData(std::string _name);

private:
	ControllerInputManager* controller;
	KeyboardInputManager* keyboard;
	MouseInputManager* mouse;

	std::unordered_map<std::string, KeyConfigData> inputData;
};