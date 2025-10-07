#pragma once
#include "unordered_map"
#include "playerStateBase.h"
#include <typeinfo>
#include <string>
#include "ID.h"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_node_editor.h"
#include "Debug.h"
#include <vector>
#include "CsvReader.h"
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
	StateManager(){ 
		state = nullptr;
		nodeBaseNum = -1;
		nodeName = "";
		nodeWindow = ed::CreateEditor();
		currentId = ID::ID_MAX;
		nextId = ID::ID_MAX;
		stateNoChange = false;
	}
	~StateManager() { 
		/*for (auto& t : stateInfo) {
			delete t.second;
			t.second = nullptr;
		}
		state = nullptr;*/
		stateInfo.clear(); 
		ed::DestroyEditor(nodeWindow);
		Debug::DebugLogPrintf(Debug::printfString("%s : size = %d", nodeName.c_str(), state.use_count()));
	}

	virtual void Update() { 
		if (currentId != nextId) {
			currentId = nextId;
			Change(currentId);
		}
		state->Update();
	}
	virtual void Draw() { state->Draw(); }

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
	void StartState(ID::IDType _id) {
		std::string id = ID::GetID(_id);
		state = stateInfo[id];
		state->Start();
	}
	/// <summary>
	/// 次の１フレーム後にステートを変える
	/// </summary>
	/// <returns></returns>
	void ChangeState(ID::IDType _id) {
		if (currentId == _id) {
			return;
		}
		nextId = _id;
	}
	/// <summary>
	/// 通った瞬間にstateを変えることが出来るクラス
	/// </summary>
	/// <param name="_id"></param>
	void NowChangeState(ID::IDType _id) {
		nextId = _id;
		currentId = _id;
		Change(currentId);
	}
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


	void NodeDrawReady() {
		for (auto s : stateInfo) {
			idName.emplace_back(s.second->GetID());
		}
	}

	void LoadState() {
		CsvReader csvReader;

		csvReader.Load(fileName);

		for (int r = 0; r < csvReader.GetLines(); r++) {
			if (r == csvReader.GetLines() - 1) {
				newLinkIdNum = std::stoi(csvReader.GetString(r, 0));
			}
			else {
				Link links;
				for (int c = 0; c < csvReader.GetColums(r); c++) {
					std::string string;
					switch (c)
					{
					case 0:
						links.input = std::stoi(csvReader.GetString(r, c));
						break;
					case 1:
						links.output = std::stoi(csvReader.GetString(r, c));
						break;
					case 2:
						links.id = std::stoi(csvReader.GetString(r, c));
						break;
					default:
						break;
					}
				}
				link.emplace_back(links);
			}

		}
	}

	void SaveState() {
		CsvReader csvReader;

		std::vector<std::vector<std::string>> data;

		for (const Link& l : link) {
			std::vector<std::string> row;
			row.emplace_back(std::to_string(l.input.Get()));
			row.emplace_back(std::to_string(l.output.Get()));
			row.emplace_back(std::to_string(l.id.Get()));
			data.emplace_back(row);
		}
		std::vector<std::string> num;
		num.emplace_back(std::to_string(newLinkIdNum));
		data.emplace_back(num);

		csvReader.SaveAll(fileName, data);
	}

	void StateNodeDraw() {
		SetCurrentEditor(nodeWindow);
		std::string nName = nodeName + "StateNode";
		ed::Begin(nName.c_str());
		for (int i = 0; i < idName.size(); i++) {

			if (state->GetID() == idName[i]) {
				ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(255, 0, 0));
			}
			int baseId = i * nodeBaseNum;
			int nodeId = baseId;
			int pinInputId = baseId + 1;
			int pinOutputId = baseId + 2;
			ed::BeginNode(nodeId);

			std::string string = ID::GetID(idName[i]);
			std::string start = string + "Start";
			std::string end = string + "End";
			ImGui::Text(string.c_str());
			ed::BeginPin(pinInputId, ed::PinKind::Input);
			ImGui::Text("In");
			ed::EndPin();
			ed::BeginPin(pinOutputId, ed::PinKind::Output);
			ImGui::Text("Out");
			ed::EndPin();
			ed::EndNode();
			if (state->GetID() == idName[i]) {
				ed::PopStyleColor();
			}
		}

		//生成処理
		if (ed::BeginCreate()) {
			ed::PinId startPinId, finishPinId;
			Debug::DebugLog("delete");
			if (ed::QueryNewLink(&startPinId, &finishPinId)) {
				if (ed::AcceptNewItem()) {
					Debug::DebugLog("newLink");
					ed::LinkId newLinkId = ed::LinkId(newLinkIdNum++);
					link.emplace_back(Link(finishPinId, startPinId, newLinkId));
					ed::Link(newLinkId, startPinId, finishPinId);
				}
			}
		}
		ed::EndCreate();

		ed::LinkId popLink = ed::GetHoveredLink();
		std::string popName = nodeName + "LinkPopMenu";
		if (popLink && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
			keepLink = popLink; // 右クリックしたリンクを保存
			Debug::DebugLog(std::to_string(keepLink.Get()));
			ed::Suspend();
			ImGui::OpenPopup(popName.c_str());
			ed::Resume();

		}

		ed::Suspend();
		if (ImGui::BeginPopup(popName.c_str())){
			if (ImGui::MenuItem("Delete Link")){
				ed::LinkId deleteId = keepLink.Get();
				Debug::DebugLog("Deleting link ID: " + std::to_string(deleteId.Get()));
				//削除処理
				for (auto e = link.begin(); e != link.end();) {
					if ((e)->id == deleteId) {
						e = link.erase(e);
						break;
					}
					e++;
				}
				keepLink = ed::LinkId();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
		ed::Resume();



		// 削除処理の後にリンクを描画する
		for (Link& l : link) {
			ed::Link(l.id, l.output, l.input);
		}

		ed::NodeId doubleClick = ed::GetDoubleClickedNode();
		if (doubleClick) {
			int nodeId = static_cast<int>(doubleClick.Get());
			int index = nodeId / nodeBaseNum;
			ChangeState(idName[index]);
		}
		ed::End();
	}

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

	void Change(ID::IDType _id) {
		if (stateNoChange) {
			return;
		}
		state->Finish();
		beforeId = _id;
		StartState(_id);
	}
};