#pragma once
#include <string>
#include "BaseObject.h"
#include "function.h"

//コンポーネントの基底クラス

class Component
{
public:
	Component() {
		active = true;
		obj = nullptr;
		timeRate = 1.0f;
		debugId = -1;
	}
	virtual ~Component(){}
	virtual void Update(){}
	virtual void Draw(){}
	//コンポーネントを始めた時に設定する。
	virtual void Start(){}
	//親のオブジェクトをここで持たせている。内部的にやっているので呼び出す必要性なし)
	virtual void Init(BaseObject* _ob) { obj = _ob; }
	//アクティブ状態を記録しておく（アクティブじゃなかったら削除)
	bool IsActive() { return active; }
	//Imguiでデバック表示したいときに使う関数。
	virtual void ImguiDraw() {};
	virtual void DebugDraw() {};
	//タグを設定できるようにする「
	virtual void SetTag(std::string name) { tag = name; }
	virtual std::string GetTag() { return tag; }

	int GetId() { return debugId; }
	std::string GetCollTag() { return colltag; }
	void SetCollTag(std::string _tag) { colltag = _tag; }

	//void SetSlowMode(bool _slow) { slowMode = _slow;}

protected:
	BaseObject* obj;
	bool active;
	std::string tag;
	std::string colltag;

	float timeRate;
	int debugId;
};