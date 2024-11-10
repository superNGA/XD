#include "ENEMY THREAD.h"

Memory mem("cs2.exe");

/*void CreateConsole()
{
	AllocConsole();
	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	std::cout << "Console created successfully!" << std::endl;
}*/


void Entity_Finder(uintptr_t CLIENT)
{

	CLEAR_ENEMY_LIST();//Clearing Entity List
	uintptr_t LOCALPLAYER = mem.Read<uintptr_t>(CLIENT + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
	int TEAM = mem.Read<int>(LOCALPLAYER + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);

	uintptr_t EntityList = mem.Read<uintptr_t>(CLIENT + cs2_dumper::offsets::client_dll::dwEntityList);
	if (!EntityList) return;

	uintptr_t ListEntry = mem.Read<uintptr_t>(EntityList + 0x10); //check
	for (int i = 1; i < 64; i++)
	{

		if (!ListEntry) continue;
		uintptr_t CurrentController = mem.Read<uintptr_t>(ListEntry + (i * 0x78));
		if (!CurrentController) continue;

		int PawnHandle = mem.Read<int>(CurrentController + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
		if (!PawnHandle) continue;

		uintptr_t ListEntry2 = mem.Read<uintptr_t>(EntityList + (0x8 * ((PawnHandle & 0x7FFF) >> 9) + 0x10));
		uintptr_t PawnCurrent = mem.Read<uintptr_t>(ListEntry2 + (0x78 * (PawnHandle & 0x1FF)));

		if (!(COMMON_STORAGE::CHEAT_STATE::FRENDLY_FIRE))
		{
			if (TEAM == mem.Read<int>(PawnCurrent + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum)) continue; //Checking Team
		}
		if (mem.Read<int>(PawnCurrent + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth) < 1) continue; //Cheaking if dead

		ADD_ENEMY_POINTER(PawnCurrent);//Finally adding to the list
	}

	return;
}


void EnemyThread(HINSTANCE Instance)
{
	uintptr_t CLIENT = (uintptr_t)GetModuleHandle("client.dll");
	uintptr_t ENGINE = (uintptr_t)GetModuleHandle("engine2.dll");

	while (!(COMMON_STORAGE::GAME_STATE::BREAK_CALL))
	{
		
		uintptr_t ENGINE_PTR = *(uintptr_t*)(ENGINE + cs2_dumper::offsets::engine2_dll::dwNetworkGameClient);
		int GameState = *(int*)(ENGINE_PTR + cs2_dumper::offsets::engine2_dll::dwNetworkGameClient_signOnState);

		if (GameState != 6) COMMON_STORAGE::GAME_STATE::IN_LOBBY.store(true);

		if (COMMON_STORAGE::GAME_STATE::IN_LOBBY)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			continue;
		}

		Entity_Finder(CLIENT);
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	FreeLibraryAndExitThread(Instance, 0);
	return;
}