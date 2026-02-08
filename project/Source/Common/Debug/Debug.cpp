#include "Debug.h"
#include "../../../ImGui/imgui.h"
#include "../InputManager/InputManager.h"

namespace {
	std::list<std::string>logger;

	float fps;
	float drawFps;
	int num;
	bool shaker;
	float rate;

	bool sphereColliderDraw;
	bool aabbColliderDraw;
	bool modelColliderDraw;
	bool rayColliderDraw;
	InputManager* inputManager;
	bool debug;
}

void Debug::CreateMessageBox(const std::string& _error, const std::string& _title)
{
#if DebugIf
	MessageBox(NULL, _error.c_str(), _title.c_str(), MB_ICONERROR | MB_OK);
#endif
}

void Debug::InitDebug(InputManager* _input)
{
	drawFps				= 60.0f;
	shaker				= true;

	sphereColliderDraw	= false;
	aabbColliderDraw	= false;
	modelColliderDraw	= false;
	rayColliderDraw		= false;
	inputManager		= _input;

	logger.clear();

#ifdef _DEBUG
	debug = false;
#else
	debug = false;

#endif
}

//void Debug::UpdateDebug()
//{
//#ifdef _DEBUG
//	ImGui::Begin("debug");
//	if (ImGui::TreeNode("ger")) {
//		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(500, 200));
//		for (std::string& str : logger) {
//			ImGui::Text("%s", str.c_str());
//			ImGui::Separator();
//		}
//		ImGui::EndChild();
//		ImGui::TreePop();
//	}
//	if (logger.size() >= 100) {
//		logger.pop_front();
//	}
//	ImGui::End();
//#endif
//}

void Debug::DebugUpdate()
{
#ifdef _DEBUG
	if (inputManager->KeyInputDown("debugChange")) {
		debug = !debug;
	}


	if (!debug) {
		return;
	}
	num++;
	if (num >= 60) {
		fps = GetFPS();
		num = 0;
	}
	ImGui::Text("FPS : %.1f", fps);
	DrawMemory();
	if (ImGui::TreeNode("Debug")) {
		if (ImGui::Button("breakPoint")) {
			int a = 0;
		}
		if (shaker) {
			if (ImGui::Button("shakeStop")) {
				shaker = false;
			}
		}
		else {
			if (ImGui::Button("shakeAgain")) {
				shaker = true;
			}
		}
		ImGui::DragFloat("timeRate", &rate, 0.1f, 0.0f, 2.0f);
		if (ImGui::Button("timeChange")) {
			Time::DebugSlowTimeSet(rate);
		}
		if (ImGui::TreeNode("DebugCollsion")) {
			
			if (ImGui::Button("sphereColliderDraw")) {
				sphereColliderDraw = !sphereColliderDraw;
			}
			if (ImGui::Button("aabbColliderDraw")) {
				aabbColliderDraw = !aabbColliderDraw;
			}
			if (ImGui::Button("modelColliderDraw")) {
				modelColliderDraw = !modelColliderDraw;
			}
			if (ImGui::Button("rayColliderDraw")) {
				rayColliderDraw = !rayColliderDraw;
			}
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

	UpdateLogger();
#endif // _DEBUG
}

void Debug::DebugLog(std::string _log)
{
	logger.emplace_back(_log);
}

void Debug::DebugLogPrintfArgs(const TCHAR* _log,...)
{
	std::string name;
	name = printfString(_log);
	logger.emplace_back(name);
}

void Debug::DebugLogPrintf(const TCHAR* _log)
{
	std::string name;
	name = _log;
	logger.emplace_back(name);
}

void Debug::UpdateLogger()
{
	if (ImGui::TreeNode("logger")) {
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(500, 400));
		for (std::string& str : logger) {
			ImGui::Text("%s", str.c_str());
			ImGui::Separator();
		}
		ImGui::EndChild();
		ImGui::TreePop();
	}
	if (logger.size() >= 1000) {
		logger.pop_front();
	}
	
}

bool Debug::GetDebugMode()
{
	return debug;
}

void Debug::ClearLogger()
{
	logger.clear();
}

bool Debug::ShakeStop()
{
	return shaker;
}

bool Debug::SphereColliderDraw()
{
	return sphereColliderDraw;
}

bool Debug::AABBColliderDraw()
{
	return aabbColliderDraw;
}

bool Debug::ModelColliderDraw()
{
	return modelColliderDraw;
}

bool Debug::RayColliderDraw()
{
	return rayColliderDraw;
}

void Debug::DrawMemory()
{
	if(ImGui::TreeNode("Memory")) {
		//現在の使用メモリ
		ImGui::Text("Memory : %zu", GetMemory());
		size_t memory = GetMemory();
		//メガバイト表記
		float memoryMB = (float)memory / (1024.0f * 1024.0f);
		ImGui::Text("Memory: %.2f MB\n", memoryMB);
		//最大メモリ
		ImGui::Text("Total : Memory : %zu", GetTotalMemory());
		size_t totalMemory = GetTotalMemory();
		//メガバイト表記
		float totalMemoryMB = (float)totalMemory / (1024.0f * 1024.0f);
		ImGui::Text("Total : Memory: %.2f MB\n", totalMemoryMB);
		ImGui::TreePop();
	}
	
}
