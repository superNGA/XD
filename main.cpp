#include "includes.h"

#include <iostream>
//Threads
#include "SRC/Threads/AIMBOT THREAD.h"
#include "SRC/Threads/ENEMY THREAD.h"
#include "SRC/Threads/MISC THREAD.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

//Bomb Timer variables
float countdownTime = 40.0f;
std::chrono::time_point<std::chrono::high_resolution_clock> lastTimePoint;

//To Only Load custom font once
bool CustomFontLoaded = false;

namespace GAME_STATE
{
	uintptr_t CLIENT = (uintptr_t)GetModuleHandle("client.dll");

	bool IN_LOBBY = true;

	//Change height and width if causing bullshit
	int SCREEN_WIDTH = 1920;
	//int SCREEN_WIDTH = GetSystemMetrics(0);
	//int SCREEN_HEIGHT = GetSystemMetrics(1);
	int SCREEN_HEIGHT = 1080;

	/*ImVec2 VIEW_PORT_DIMENSIONS = ImGui::GetIO().DisplaySize;
	int SCREEN_WIDTH = VIEW_PORT_DIMENSIONS.x;
	int SCREEN_HEIGHT = VIEW_PORT_DIMENSIONS.y;*/

	ImVec2 center(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
};
//Global Cheat variables
namespace CHEAT_STATE
{
	//Aimbot options
	bool AIMBOT_STATE = false;
	int SCREEN_WIDTH = 1400;
	int SCREEN_HEIGHT = 1080;
	float SMOOTHING_FACTOR = 2.0f;
	float FOV_RADIUS = 25.0f;
	bool FOV_CIRCLE = false;
	bool FRENDLY_FIRE = false;

	//Misc options
	bool BHOP_STATE = false;
	bool TRIGGER_BOT_STATE = false;
	bool TRIGGER_BOT_ON_KEY = true;
	int DELAY_TRIGGER_BOT = 50;
	bool ANTIAFK_STATE = false;
	bool BOMB_TIMER = false;
	bool FLASH_GAURD_STATE = false;
};

void UPDATE()
{
	COMMON_STORAGE::CHEAT_STATE::FOV_RADIUS.store(CHEAT_STATE::FOV_RADIUS);
	COMMON_STORAGE::CHEAT_STATE::SMOOTHING_FACTOR.store(CHEAT_STATE::SMOOTHING_FACTOR);
	
	//call thread updates here
}

void LoadCustomFont()
{
	ImGuiIO& io = ImGui::GetIO();  // Get IO instance

	// Load a custom font from the specified path
	ImFont* customFont = io.Fonts->AddFontFromFileTTF("D:\\PROJECTS\\Cheat Menu\\ImGui DirectX 11 Kiero Hook\\Assets\\Font2.ttf", 18.0f);

	// Set the custom font as the default font
	io.FontDefault = customFont;

	// Build fonts atlas
	io.Fonts->Build();
}

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);

	ImGuiStyle& mStyle = ImGui::GetStyle();

	mStyle.WindowMinSize = ImVec2(160, 20);
	mStyle.FramePadding = ImVec2(4, 2);
	mStyle.ItemSpacing = ImVec2(6, 2);
	mStyle.ItemInnerSpacing = ImVec2(6, 4);
	mStyle.Alpha = 0.95f;
	mStyle.WindowRounding = 4.0f;
	mStyle.FrameRounding = 2.0f;
	mStyle.IndentSpacing = 6.0f;
	mStyle.ItemInnerSpacing = ImVec2(2, 4);
	mStyle.ColumnsMinSpacing = 50.0f;
	mStyle.GrabMinSize = 14.0f;
	mStyle.GrabRounding = 16.0f;
	mStyle.ScrollbarSize = 12.0f;
	mStyle.ScrollbarRounding = 16.0f;

	ImGuiStyle& style = mStyle;
	style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.92f, 0.18f, 0.29f, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.92f, 0.18f, 0.29f, 0.43f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.9f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.22f, 0.27f, 0.73f);

	if (!CustomFontLoaded)
	{
		LoadCustomFont();
		CustomFontLoaded = true;
	}
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		Globals::IS_VISIBLE = !Globals::IS_VISIBLE;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (Globals::IS_VISIBLE)
	{
		/*ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();*/

		ImGui::SetNextWindowSize(ImVec2(900, 400));
		ImGui::Begin("XD", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		{
			ImGui::SameLine();
			if (ImGui::Button("MISC", ImVec2(200, 30)))
			{
				Globals::Tab = 0;
			}
			ImGui::SameLine();
			if (ImGui::Button("AIM", ImVec2(200, 30)))
			{
				Globals::Tab = 1;
			}
			ImGui::SameLine();
			if (ImGui::Button("WALLS", ImVec2(200, 30)))
			{
				Globals::Tab = 2;
			}
			ImGui::SameLine();
			if (ImGui::Button("EXIT", ImVec2(200, 30)))
			{
				Globals::Tab = 3;
			}
			ImGui::Separator();
			//------------------------------LAYER 2----------------------------------------
			//Miscellaneous Features
			if (Globals::Tab == 0)
			{
				ImGui::Checkbox("Bhop", &(CHEAT_STATE::BHOP_STATE));
				COMMON_STORAGE::CHEAT_STATE::BHOP_STATE.store(CHEAT_STATE::BHOP_STATE);

				ImGui::Checkbox("Trigger Bot", &(CHEAT_STATE::TRIGGER_BOT_STATE));
				COMMON_STORAGE::CHEAT_STATE::TRIGGER_BOT_STATE.store(CHEAT_STATE::TRIGGER_BOT_STATE);

				ImGui::Checkbox("Trigger Bot on key", &(CHEAT_STATE::TRIGGER_BOT_ON_KEY));
				COMMON_STORAGE::CHEAT_STATE::TRIGGER_BOT_ON_KEY.store(CHEAT_STATE::TRIGGER_BOT_ON_KEY);

				ImGui::SliderInt("Trigger Bot Delay", &(CHEAT_STATE::DELAY_TRIGGER_BOT), 0, 1000);
				COMMON_STORAGE::CHEAT_STATE::DELAY_TRIGGER_BOT.store(CHEAT_STATE::DELAY_TRIGGER_BOT);

				ImGui::Checkbox("Anti AFK", &(CHEAT_STATE::ANTIAFK_STATE));
				COMMON_STORAGE::CHEAT_STATE::ANTIAFK_STATE.store(CHEAT_STATE::ANTIAFK_STATE);

				ImGui::Checkbox("Bomb Timer", &(CHEAT_STATE::BOMB_TIMER));
				COMMON_STORAGE::CHEAT_STATE::BOMB_TIMER.store(CHEAT_STATE::BOMB_TIMER);

				ImGui::Checkbox("Flash Gaurd", &(CHEAT_STATE::FLASH_GAURD_STATE));
			}
			
			//Aimbot Features
			else if (Globals::Tab == 1)
			{
				ImGui::Checkbox("AIMBOT", &(CHEAT_STATE::AIMBOT_STATE));
				COMMON_STORAGE::CHEAT_STATE::AIMBOT_STATE.store(CHEAT_STATE::AIMBOT_STATE); //Updating Cheat status

				if (ImGui::Button(("HEAD"), ImVec2(100,30)))
				{
					COMMON_STORAGE::CHEAT_STATE::AIMBOT_TARGET.store(0);
				}
				ImGui::SameLine();
				if (ImGui::Button(("NECK"), ImVec2(100, 30)))
				{
					COMMON_STORAGE::CHEAT_STATE::AIMBOT_TARGET.store(1);
				}
				ImGui::SameLine();
				if (ImGui::Button(("CHEST"), ImVec2(100, 30)))
				{
					COMMON_STORAGE::CHEAT_STATE::AIMBOT_TARGET.store(2);
				}
				ImGui::SameLine();
				if (ImGui::Button(("STOMACK"), ImVec2(100, 30)))
				{
					COMMON_STORAGE::CHEAT_STATE::AIMBOT_TARGET.store(3);
				}
				ImGui::SameLine();
				if (ImGui::Button(("PP"), ImVec2(100, 30)))
				{
					COMMON_STORAGE::CHEAT_STATE::AIMBOT_TARGET.store(4);
				}
				ImGui::SameLine();
				if (ImGui::Button(("FEET"), ImVec2(100, 30)))
				{
					COMMON_STORAGE::CHEAT_STATE::AIMBOT_TARGET.store(5);
				}

				ImGui::Separator();
				ImGui::Checkbox("Frendly Fire", &(CHEAT_STATE::FRENDLY_FIRE));
				COMMON_STORAGE::CHEAT_STATE::FRENDLY_FIRE.store(CHEAT_STATE::FRENDLY_FIRE);

				ImGui::SliderFloat("FOV radius", &(CHEAT_STATE::FOV_RADIUS), 10.0f, 500.0f);
				COMMON_STORAGE::CHEAT_STATE::FOV_RADIUS.store(CHEAT_STATE::FOV_RADIUS);

				CHEAT_STATE::FOV_CIRCLE = COMMON_STORAGE::CHEAT_STATE::FOV_CIRCLE;
				ImGui::Checkbox("FOV circle", &(CHEAT_STATE::FOV_CIRCLE));
				COMMON_STORAGE::CHEAT_STATE::FOV_CIRCLE.store(CHEAT_STATE::FOV_CIRCLE);

				ImGui::SliderFloat("Smoothing Factor", &(CHEAT_STATE::SMOOTHING_FACTOR), 1.0f, 10.0f);
				COMMON_STORAGE::CHEAT_STATE::SMOOTHING_FACTOR.store(CHEAT_STATE::SMOOTHING_FACTOR);
			}
			
			//Exit control
			else if (Globals::Tab == 3)
			{
				if (ImGui::Button("Exit :(", ImVec2(150, 50)))
				{
					COMMON_STORAGE::GAME_STATE::BREAK_CALL.store(true);
					kiero::shutdown();
				}
				ImGui::SameLine();
				if (ImGui::Button("Update Resolutoin", ImVec2(150, 50)))
				{
					ImVec2 NEW_WINDOW_RESOLUTION = ImGui::GetIO().DisplaySize;
					GAME_STATE::SCREEN_WIDTH = NEW_WINDOW_RESOLUTION.x;
					GAME_STATE::SCREEN_HEIGHT = NEW_WINDOW_RESOLUTION.y;
					COMMON_STORAGE::CHEAT_STATE::SCREEN_WIDTH.store(NEW_WINDOW_RESOLUTION.x);
					COMMON_STORAGE::CHEAT_STATE::SCREEN_HEIGHT.store(NEW_WINDOW_RESOLUTION.y);

					//Updating thread local resolutions
					UPDATE_AIMBOT_RESOLUTION();
					UPDATE_WORLD_TO_SCREEN_RESOLUTION();
				}

				ImGui::Separator();
				ImGui::Text("%d x %d", GAME_STATE::SCREEN_WIDTH, GAME_STATE::SCREEN_HEIGHT);
			}
			
			//------------------------------LAYER 3----------------------------------------
			ImGui::Separator();

			ImGui::SetCursorPos(ImVec2(10, 350));
			GAME_STATE::IN_LOBBY = COMMON_STORAGE::GAME_STATE::IN_LOBBY;
			ImGui::Checkbox("SAFETY PIN", &(GAME_STATE::IN_LOBBY));
			COMMON_STORAGE::GAME_STATE::IN_LOBBY.store(GAME_STATE::IN_LOBBY);
		};


		ImGui::End();

		//ImGui::Render();

		/*pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());*/
	}


	//Creating Bomb Timer window
	
	if (!(COMMON_STORAGE::GAME_STATE::IN_LOBBY) && CHEAT_STATE::BOMB_TIMER)
	{
		uintptr_t GAME_RULE = *(uintptr_t*)(GAME_STATE::CLIENT + cs2_dumper::offsets::client_dll::dwGameRules);

		bool BOMB_PLANTED = *(bool*)(GAME_RULE + cs2_dumper::schemas::client_dll::C_CSGameRules::m_bBombPlanted);

		auto currentTime = std::chrono::high_resolution_clock::now(); //Getting Current time
		float elapsedTime = std::chrono::duration<float>(currentTime - lastTimePoint).count(); //Getting duration from lastPoint
		lastTimePoint = currentTime; 
		if (BOMB_PLANTED)
		{

			ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
			ImGui::SetNextWindowBgAlpha(0.8f);  // Optional: Set background transparency

			ImGui::Begin("BOMB TIMER", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

			countdownTime -= elapsedTime;
			if (countdownTime <= 0.0f)
			{
				//countdownTime = 0.0f;
				ImGui::Text("KABOOM!");
			}
			else
			{
				// Display the countdown timer
				ImGui::Text("BOMB : %.1f", countdownTime);
			}
			

			ImGui::End();
			
		}
		else
		{

			ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
			ImGui::SetNextWindowBgAlpha(0.8f);  // Optional: Set background transparency
			if(countdownTime != 40.0f) countdownTime = 40.0f;
			ImGui::Begin("BOMB TIMER", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

			ImGui::Text("Bomb Inactive : %.1f", countdownTime);

			ImGui::End();
			//ImGui::Render();
		}
	}

	//Drawing FOV circle

	if (!(COMMON_STORAGE::GAME_STATE::IN_LOBBY) && CHEAT_STATE::FOV_CIRCLE)
	{
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowBgAlpha(0.0f);  // Optional: Set background transparency
		ImGui::SetNextWindowSize(ImVec2(GAME_STATE::SCREEN_WIDTH, GAME_STATE::SCREEN_HEIGHT));

		ImGui::Begin("BOMB TIMER", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

		//ImGui::Text("This is the transparent window!");
		ImGui::GetWindowDrawList()->AddCircle(ImVec2(GAME_STATE::SCREEN_WIDTH / 2, GAME_STATE::SCREEN_HEIGHT / 2), CHEAT_STATE::FOV_RADIUS/2, IM_COL32(255, 0, 0, 255), 32, 2.0f);

		ImGui::End();
	}

	//Drawing Flash Gaurd

	/*if (!(COMMON_STORAGE::GAME_STATE::IN_LOBBY) && CHEAT_STATE::FLASH_GAURD_STATE)
	{
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(GAME_STATE::SCREEN_WIDTH, GAME_STATE::SCREEN_HEIGHT), ImGuiCond_Always);
		ImGui::SetNextWindowBgAlpha(1.0f);

		ImGui::Begin("FLASH GAURD", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(GAME_STATE::SCREEN_WIDTH, GAME_STATE::SCREEN_HEIGHT), IM_COL32(0, 0, 0, 255));
	
		ImVec2 textPos((GAME_STATE::SCREEN_WIDTH) / 2, (GAME_STATE::SCREEN_HEIGHT) / 2);

		ImGui::Text("xd");

		ImGui::End();
	}*/

	ImGui::Render();
	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)& oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);

	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr); //ImGui Thread
		//My stuff 
		CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(EnemyThread), hMod, 0, nullptr);
		CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(AimbotThread), hMod, 0, nullptr);
		CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(MiscThread), hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}