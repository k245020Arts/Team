#pragma once
#include "../../Library/myDxLib.h"
#include "../Component/Shaker/Shaker.h"

class MeshRenderer;
class MeshRenderer2D;
class Physics;
class StateManager;
class Camera;
class InputManager;
class KeyboardInputManager;
class ControllerInputManager;
class Player;
class Animator;
class Color;
class BaseObject;
class Shaker;
class EffectManager;
class SoundManager;
class WeaponManager;
class MotionBlur;
class GameManager;
class EnemyManager;
class Anim2D;
class ButtonUI;

namespace PlayerInformation {

	enum PlayerState
	{
		WAIT,
		WALK,
	};

	struct CharaComponent
	{
		CharaComponent() {
			stateManager = nullptr;
			renderer = nullptr;
			physics = nullptr;
			camera = nullptr;
			InputManager = nullptr;
			controller = nullptr;
			keyboard = nullptr;
			player = nullptr;
			anim = nullptr;
			color = nullptr;
			targetObj = nullptr;
			shaker = nullptr;
			effect = nullptr;
			sound = nullptr;
			weapon = nullptr;
			blur = nullptr;
			gameManager = nullptr;
			enemyManager = nullptr;
			hitObj = nullptr;
			meshRenderer2D = nullptr;
			anim2D = nullptr;
			specialAttackButton = nullptr;
		}
		StateManager* stateManager;
		MeshRenderer* renderer;
		Physics* physics;
		Camera* camera;
		InputManager* InputManager;
		ControllerInputManager* controller;
		KeyboardInputManager* keyboard;
		Player* player;
		Animator* anim;
		Color* color;
		BaseObject* targetObj;
		BaseObject* hitObj;
		Shaker* shaker;
		EffectManager* effect;
		SoundManager* sound;
		WeaponManager* weapon;
		MotionBlur* blur;
		GameManager* gameManager;
		EnemyManager* enemyManager;
		MeshRenderer2D* meshRenderer2D;
		Anim2D* anim2D;
		ButtonUI* specialAttackButton;

	};
	/// <summary>
	/// 基準の空気抵抗値
	/// </summary>
	static const VECTOR3 BASE_INTERIA = VECTOR3(7500.0f, -7500.0f, 7500.0f);
	/// <summary>
	/// 基準の重力値
	/// </summary>
	static const VECTOR3 BASE_GRAVITY = VECTOR3(0.0f, -5000.0f, 0.0f);
	/// <summary>
	/// ジャスト回避をした時にどれくらい敵をスローさせるか
	/// </summary>
	static const float JUST_AVOID_ENEMY_TIME_SCALE = 0.3f;

	struct PlayerReaction
	{
		PlayerReaction() {
			shakePower = VZero;
			shakeTime = 0.0f;
			cameraShakePower = VZero;
			cameraShakeTime = 0.0f;
			soundName.clear();
			soundKind = 0;
			shakerLoop = false;
			shakePattern = Shaker::NONE;
		}
		PlayerReaction(const VECTOR3& _shakePower,float _shakeTime,const VECTOR3& _cameraShakePower,
			float _cameraShakeTime,const std::string& _soundName,int _soundKind,bool _shakerLoop,
			Shaker::ShakePattern _shakePattern)
		{
			shakePower = _shakePower;
			shakeTime = _shakeTime;
			cameraShakePower = _cameraShakePower;
			cameraShakeTime = _cameraShakeTime;
			soundName = _soundName;
			soundKind = _soundKind;
			shakerLoop = _shakerLoop;
			shakePattern = _shakePattern;
		}

		VECTOR3 shakePower;
		float shakeTime;
		VECTOR3 cameraShakePower;
		float cameraShakeTime;
		std::string soundName;
		int soundKind;
		bool shakerLoop;
		Shaker::ShakePattern shakePattern;
	};
}
