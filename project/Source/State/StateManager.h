#pragma once
#include "unordered_map"
#include "../Player/PlayerState/PlayerStateBase.h"
#include <typeinfo>
#include <string>
#include "../Common/ID/ID.h"
#include "../../ImGui/imgui.h"
#include "../../ImGui/imgui_node_editor.h"
#include "../Common/Debug/Debug.h"
#include <vector>
#include "../Common/CsvReader.h"
#include <memory>

namespace ed = ax::NodeEditor;

struct Link
{
	Link() {}
	Link(ed::PinId _input, ed::PinId _output, ed::LinkId  _id) {
		input = _input;
		output = _output;
		id = _id;
	}
	ed::PinId  input, output;
	ed::LinkId id;
};

struct Node
{
	Node() { ; }
	Node(ed::NodeId _id, ImVec2 _pos) {
		id = _id;
		position = _pos;
		//name = _name;
	}

	//ID::IDType name;
	ImVec2 position;
	ed::NodeId id;
};

//ステートパターンを管理させるクラス。

class StateManager : public Component
{
public:
	StateManager();
	~StateManager();

	virtual void Update();
	virtual void Draw();

	ID::IDType GetBeforeID() { return beforeId; }

	//ステートを追加
	template <typename T>
	std::shared_ptr<T> CreateState(std::string _state) {
		std::shared_ptr<T> sta = std::make_shared<T>();
		sta->Init(obj);
		stateInfo.emplace(_state,sta);
		return sta;
	}

	template <typename T>
	void SetComponent(T* _c) {
		for (auto& t : stateInfo) {
			t.second->SetComponent(_c);
		}
	}

	//最初の一回だけ呼び出す用関数
	void StartState(ID::IDType _id);
	/// <summary>
	/// 次の１フレーム後にステートを変える
	/// </summary>
	/// <returns></returns>
	void ChangeState(ID::IDType _id);
	/// <summary>
	/// 通った瞬間にstateを変えることが出来るクラス
	/// </summary>
	/// <param name="_id"></param>
	void NowChangeState(ID::IDType _id);
	/// <summary>
	/// この変数をtrueにするとstateがチェンジされない
	/// </summary>
	/// <param name="_noChange"></param>
	void SetNoStateChange(bool _noChange) { stateNoChange = _noChange; }

	std::shared_ptr<StateBase> GetCurrentState() { return state; }

	//現在のステートをとる。
	template <typename T>
	std::shared_ptr<T> GetState() {
		return std::dynamic_pointer_cast<T>(GetCurrentState());
	}


	void NodeDrawReady();

	void LoadState();

	void SaveState();

	void StateNodeDraw();

	void ImguiDraw();

protected:

	std::unordered_map<std::string, std::shared_ptr<StateBase>> stateInfo;
	std::shared_ptr<StateBase> state;

	ID::IDType currentId;
	ID::IDType beforeId;
	ID::IDType nextId;

	ax::NodeEditor::EditorContext* nodeWindow;

	std::vector<ID::IDType> idName;
	std::vector<Link> link;
	std::vector<Node> node;

	int nodeBaseNum;
	std::string nodeName;

	int newLinkIdNum;
	ed::LinkId keepLink;

	std::string fileName;
	bool stateNoChange;
private:

	void Change(ID::IDType _id);
};