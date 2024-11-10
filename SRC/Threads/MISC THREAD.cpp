#include "MISC THREAD.h"

//uintptr_t CLIENT = (uintptr_t)GetModuleHandle("client.dll");
//INPUT WHEEL_DOWN = { 0 };
//INPUT WHEEL_UP = { 0 };

MISCBOT::MISCBOT(uintptr_t BaseAdrs, INPUT& mousedown, INPUT& mouseup, INPUT& LeftMouse)
	:CLIENT(BaseAdrs), MOUSE_DOWN(mousedown), MOUSE_UP(mouseup), LEFT_MOUSE(LeftMouse)
{

}

void MISCBOT::BunnyHop()
{

	uintptr_t LocalPlayer = *(reinterpret_cast<uintptr_t*>(CLIENT + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn));
	if (!LocalPlayer) return;
	if (*(reinterpret_cast<int32_t*>(CLIENT + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth)) <= 0) return;
	int32_t Flag = *(reinterpret_cast<int32_t*>(LocalPlayer + cs2_dumper::schemas::client_dll::C_BaseEntity::m_fFlags));

	if (Flag == STANDING || Flag == CROUCHING)
	{
		SendInput(1, &MOUSE_DOWN, sizeof(INPUT));
	}
	else
	{
		SendInput(1, &MOUSE_UP, sizeof(INPUT));
	}
}

void MISCBOT::AntiAFK()
{
	uintptr_t LocalPlayer = *(reinterpret_cast<uintptr_t*>(CLIENT + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn));
	int HEALTH = *(int*)(LocalPlayer + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
	if (HEALTH > 0 && HEALTH <= 100) return; //Player still alive

	//Left Clicking
	LEFT_MOUSE.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &LEFT_MOUSE, sizeof(INPUT));
	LEFT_MOUSE.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &LEFT_MOUSE, sizeof(INPUT));
	LEFT_MOUSE.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

	//Scool wheel movement and Resting
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	SendInput(1, &MOUSE_DOWN, sizeof(INPUT));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	SendInput(1, &MOUSE_UP, sizeof(INPUT));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void MISCBOT::TriggerBot()
{
	uintptr_t LocalPlayer = *(reinterpret_cast<uintptr_t*>(CLIENT + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn));

	BYTE team = *(reinterpret_cast<BYTE*>(LocalPlayer + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum));

	uintptr_t EntityList = *(reinterpret_cast<uintptr_t*>(CLIENT + cs2_dumper::offsets::client_dll::dwEntityList));

	int CrosshairEntityIndex = *(reinterpret_cast<int*>(LocalPlayer + cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_iIDEntIndex));

	if (CrosshairEntityIndex < 0) return;

	uintptr_t ListEntry = *(reinterpret_cast<uintptr_t*>(EntityList + 0x8 * (CrosshairEntityIndex >> 9) + 0x10));
	uintptr_t Entity = *(reinterpret_cast<uintptr_t*>(ListEntry + 120 * (CrosshairEntityIndex & 0x1ff)));

	if (!Entity) return;

	//return if same team
	if (team == *(reinterpret_cast<BYTE*>(Entity + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum))) return;

	//if dead the return
	if (*(reinterpret_cast<int*>(Entity + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth)) <= 0) return;

	std::this_thread::sleep_for(std::chrono::milliseconds(COMMON_STORAGE::CHEAT_STATE::DELAY_TRIGGER_BOT));

	//Simulating click
	LEFT_MOUSE.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &LEFT_MOUSE, sizeof(INPUT));
	LEFT_MOUSE.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &LEFT_MOUSE, sizeof(INPUT));
	LEFT_MOUSE.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

	return;
}

void MiscThread(HINSTANCE Instance)
{

	// Mouse Inputs
	INPUT WHEEL_DOWN;   //Scrool Wheel down
	WHEEL_DOWN.type = INPUT_MOUSE;
	WHEEL_DOWN.mi.dwFlags = MOUSEEVENTF_WHEEL;
	WHEEL_DOWN.mi.mouseData = -WHEEL_DELTA;

	INPUT WHEEL_UP;		//Scrool wheel up
	WHEEL_UP.type = INPUT_MOUSE;
	WHEEL_UP.mi.dwFlags = MOUSEEVENTF_WHEEL;
	WHEEL_UP.mi.mouseData = WHEEL_DELTA;

	INPUT LEFT_MOUSE;	//left mouse click
	LEFT_MOUSE.type = INPUT_MOUSE;

	uintptr_t CLIENT = (uintptr_t)GetModuleHandle("client.dll");
	MISCBOT BOT(CLIENT, WHEEL_DOWN, WHEEL_UP, LEFT_MOUSE);

	while (!(COMMON_STORAGE::GAME_STATE::BREAK_CALL))
	{

		if (COMMON_STORAGE::GAME_STATE::IN_LOBBY)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			continue;
		}

		if (COMMON_STORAGE::CHEAT_STATE::BHOP_STATE)
		{
			while (GetAsyncKeyState(VK_SPACE))
			{
				BOT.BunnyHop();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}

		if (COMMON_STORAGE::CHEAT_STATE::TRIGGER_BOT_STATE)
		{
			if (COMMON_STORAGE::CHEAT_STATE::TRIGGER_BOT_ON_KEY)
			{
				while (GetAsyncKeyState(VK_XBUTTON2) & 0x8000)
				{
					BOT.TriggerBot();
					std::this_thread::sleep_for(std::chrono::milliseconds(2));
				}
			}
			else
			{
				BOT.TriggerBot();
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}
		}

		if (COMMON_STORAGE::CHEAT_STATE::ANTIAFK_STATE)
		{
			BOT.AntiAFK();
			std::this_thread::sleep_for(std::chrono::seconds(1)); //Resing for 1 second in anti afk for better performance
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	FreeLibraryAndExitThread(Instance, 0);
	return;
}