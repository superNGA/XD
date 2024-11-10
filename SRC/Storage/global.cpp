#include "global.h"

namespace COMMON_STORAGE
{
	std::mutex DATA_PROTECTOR;
	//Game state varibales
	namespace GAME_STATE
	{
		std::atomic<bool> IN_LOBBY = true;
		std::atomic<bool> BREAK_CALL = false;
	};

	//Storing Cheats state
	namespace CHEAT_STATE
	{
		//Entity Listing Options

		//aimbot options
		std::atomic<bool> AIMBOT_STATE = false;
		std::atomic<int> SCREEN_WIDTH = 1400;
		std::atomic<int> SCREEN_HEIGHT = 1080;
		std::atomic<float> SMOOTHING_FACTOR = 2.0f;
		std::atomic<float> FOV_RADIUS = 25.0f;
		std::atomic<bool> FOV_CIRCLE = false;
		std::atomic<int> AIMBOT_TARGET = 0;
		std::atomic<bool> FRENDLY_FIRE = false;

		//Misc Options
		std::atomic<bool> BHOP_STATE = false;
		std::atomic<bool> TRIGGER_BOT_STATE = false;
		std::atomic<bool> TRIGGER_BOT_ON_KEY = true;
		std::atomic<int> DELAY_TRIGGER_BOT = 100;
		std::atomic<bool> ANTIAFK_STATE = false;
		std::atomic<bool> BOMB_TIMER = false;
	};

	std::vector<bool> CURRENT_CHEAT_CONFIG;
	std::vector<uintptr_t> ENEMY_PTRS;
};

void ADD_ENEMY_POINTER(uintptr_t ptr)
{
	std::lock_guard<std::mutex> lock(COMMON_STORAGE::DATA_PROTECTOR);
	COMMON_STORAGE::ENEMY_PTRS.push_back(ptr);
}

void CLEAR_ENEMY_LIST()
{
	std::lock_guard<std::mutex> lock(COMMON_STORAGE::DATA_PROTECTOR);
	COMMON_STORAGE::ENEMY_PTRS.clear();
}

void GET_ENTITY_LIST(std::vector<uintptr_t>& storage)
{
	storage.clear();
	std::lock_guard<std::mutex> lock(COMMON_STORAGE::DATA_PROTECTOR);
	storage = COMMON_STORAGE::ENEMY_PTRS;
}

int GET_LENGTH()
{
	std::lock_guard<std::mutex> lock(COMMON_STORAGE::DATA_PROTECTOR);
	return COMMON_STORAGE::ENEMY_PTRS.size();
}

void UPDATE_CURRENT_CHEAT_STATE()
{
	std::lock_guard<std::mutex> lock(COMMON_STORAGE::DATA_PROTECTOR);
	COMMON_STORAGE::CURRENT_CHEAT_CONFIG.clear();
	COMMON_STORAGE::CURRENT_CHEAT_CONFIG.push_back(COMMON_STORAGE::CHEAT_STATE::AIMBOT_STATE);
}