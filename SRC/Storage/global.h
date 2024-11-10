#pragma once

#include <Windows.h>
#include <atomic>
#include <mutex>
#include <vector>

namespace COMMON_STORAGE
{
	//Game state varibales
	namespace GAME_STATE
	{
		extern std::atomic<bool> IN_LOBBY;
		extern std::atomic<bool> BREAK_CALL;
	};

	//Storing Cheats state
	namespace CHEAT_STATE
	{
		//Entity Listing Options
		
		//Aimbot Options
		extern std::atomic<bool> AIMBOT_STATE;
		extern std::atomic<int> SCREEN_WIDTH;
		extern std::atomic<int> SCREEN_HEIGHT;
		extern std::atomic<float> SMOOTHING_FACTOR;
		extern std::atomic<float> FOV_RADIUS;
		extern std::atomic<bool> FOV_CIRCLE;
		extern std::atomic<int> AIMBOT_TARGET;
		extern std::atomic<bool> FRENDLY_FIRE;

		//Misc Options
		extern std::atomic<bool> BHOP_STATE;
		extern std::atomic<bool> TRIGGER_BOT_STATE;
		extern std::atomic<bool> TRIGGER_BOT_ON_KEY;
		extern std::atomic<int> DELAY_TRIGGER_BOT;
		extern std::atomic<bool> ANTIAFK_STATE;
		extern std::atomic<bool> BOMB_TIMER;

	};

	extern std::vector<bool> CURRENT_CHEAT_CONFIG;
	extern std::vector<uintptr_t> ENEMY_PTRS;
};

void ADD_ENEMY_POINTER(uintptr_t ptr);

void CLEAR_ENEMY_LIST();

void GET_ENTITY_LIST(std::vector<uintptr_t>& storage);

int GET_LENGTH();

void UPDATE_CURRENT_CHEAT_STATE();