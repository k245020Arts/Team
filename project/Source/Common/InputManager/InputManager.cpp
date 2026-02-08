#include "inputManager.h"
#include "../Debug/Debug.h"

InputManager::InputManager(bool _useController, bool _useKeyboard, bool _useMouse)
{
	controller = nullptr;
	keyboard = nullptr;
	mouse = nullptr;
	if (_useController)
	{
		controller = new ControllerInputManager();
	}
	if (_useKeyboard)
	{
		keyboard = new KeyboardInputManager();
	}
	if (_useMouse)
	{
		mouse = new MouseInputManager();
	}

	DontDestroyOnSceneChange(true);

	CreateInputData(KeyConfigData("attack", KEY_INPUT_E, XINPUT_BUTTON_B, MouseInputManager::LEFT_CLICK));
	CreateInputData(KeyConfigData("avoid", KEY_INPUT_X, XINPUT_BUTTON_A, MouseInputManager::MOUSE_NONE));
	CreateInputData(KeyConfigData("SceneChange", KEY_INPUT_P, XINPUT_BUTTON_A, MouseInputManager::MOUSE_NONE));
	CreateInputData(KeyConfigData("debugChange", KEY_INPUT_Z, NOT_INPUT, MouseInputManager::RIGHT_CLICK));
	CreateInputData(KeyConfigData("camera", KEY_INPUT_SPACE, XINPUT_BUTTON_RIGHT_THUMB, MouseInputManager::RIGHT_CLICK));
	CreateInputData(KeyConfigData("changeMode", KEY_INPUT_TAB, -1, MouseInputManager::MOUSE_NONE));
	CreateInputData(KeyConfigData("SpecialAttack", KEY_INPUT_Q, XINPUT_BUTTON_X, MouseInputManager::MOUSE_NONE));
	CreateInputData(KeyConfigData("heavyAttack", KEY_INPUT_C, XINPUT_BUTTON_Y, MouseInputManager::MOUSE_NONE));

}

InputManager::~InputManager()
{
	inputData.clear();
}

void InputManager::Update()
{

}

ControllerInputManager* InputManager::GetControllerInput()
{
	if (controller != nullptr)
	{
		return controller;
	}
	return nullptr;
}

KeyboardInputManager* InputManager::GetKeyboardInput()
{
	if (keyboard != nullptr)
	{
		return keyboard;
	}
	return nullptr;
}

MouseInputManager* InputManager::GetMouseInput()
{
	if(mouse!=nullptr)
	{
		return mouse;
	}
	return nullptr;
}

void InputManager::CreateInputData(const KeyConfigData& _data)
{
	auto it = inputData.find(_data.action);
	if (inputData.end() == it) {
		inputData[_data.action] = _data;
	
	}
	else {
		Debug::DebugLog("そのInputDataは存在しています" + _data.action);
	}
}

bool InputManager::KeyInputDown(std::string _name)
{
	bool data = IsData(_name);
	if (!data) {//データが存在していないならキーを入力をしていないものとして返す
		Debug::DebugLog("noAction");
		return false;
	}
	ControllerInputManager* c = GetControllerInput();
	KeyboardInputManager* k = GetKeyboardInput();
	MouseInputManager* m = GetMouseInput();
	bool control = false;
	bool key = false;
	bool mouse = false;

	if (c != nullptr) {
		control = c->GetIsButtonPutNow(inputData[_name].padNumber);
	}
	if (k != nullptr) {
		key = k->GetIsKeyboardPut(inputData[_name].keyNumber);
	}
	if (m != nullptr) {
		mouse = m->GetMousePut(inputData[_name].mouceNumber);
	}
	if (control || key || mouse) {
		return true;
	}
	return false;
}

bool InputManager::KeyInput(std::string _name)
{
	bool data = IsData(_name);
	if (!data) { //データが存在していないならキーを入力をしていないものとして返す
		Debug::DebugLog("noAction");
		return false;
	}
	ControllerInputManager* c = GetControllerInput();
	KeyboardInputManager* k = GetKeyboardInput();
	MouseInputManager* m = GetMouseInput();
	bool control = false;
	bool key = false;
	bool mouse = false;

	if (c != nullptr) {
		control = c->GetIsButtonPushingNow(inputData[_name].padNumber);
	}
	if (k != nullptr) {
		key = k->GetIsKeyboardPushing(inputData[_name].keyNumber);
	}
	if (m != nullptr) {
		mouse = m->GetMousePushing(inputData[_name].mouceNumber);
	}
	if (control || key || mouse) {
		return true;
	}
	return false;
}

bool InputManager::KeyInputUp(std::string _name)
{
	bool data = IsData(_name);
	if (!data) {//データが存在していないならキーを入力をしていないものとして返す
		Debug::DebugLog("noAction");
		return false;
	}
	ControllerInputManager* c = GetControllerInput();
	KeyboardInputManager* k = GetKeyboardInput();
	MouseInputManager* m = GetMouseInput();
	bool control = false;
	bool key = false;
	bool mouse = false;

	if (c != nullptr) {
		control = c->GetIsButtonReleaseNow(inputData[_name].padNumber);
	}
	if (k != nullptr) {
		key = k->GetIsKeyboardRelease(inputData[_name].keyNumber);
	}
	if (m != nullptr) {
		mouse = m->GetMousePut(inputData[_name].mouceNumber);
	}
	if (control || key || mouse) {
		return true;
	}
	return false;
}

bool InputManager::IsData(std::string _name)
{
	auto it = inputData.find(_name);
	if (inputData.end() == it) {//データが存在していないならfalse
		return false;
	}
	else {
		return true;
	}
}

/*bool InputManager::GetIsKeyOrButtonPushingNow(int _key, int _button)
{
	if(GetIsKeyPushingNow(_button)||GetIsKeyboardPushing(_key))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputManager::GetIsKeyOrButtonPutNow(int _key, int _button)
{
	if (GetIsKeyPutNow(_button) || GetIsKeyboardPut(_key))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputManager::GetIsKeyOrButtonReleaseNow(int _key, int _button)
{
	if (GetIsKeyReleaseNow(_button) || GetIsKeyboardRelease(_key))
	{
		return true;
	}
	else
	{
		return false;
	}
}*/