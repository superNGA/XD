
#include "AIMBOT THREAD.h"

std::vector<DOUBLE> PARTS = { 1, 1.1, 1.2, 1.4, 1.6, 2 };

std::vector<Vector> PLAYER_SCREEN_POSITION;
std::vector<uintptr_t> ENEMY_PTRStemp;
int SMOOTHING_FACTOR = 2;
float FOV_RADIUS = 30.0f;
//float FOV_squared = COMMON_STORAGE::CHEAT_STATE::FOV_RADIUS * COMMON_STORAGE::CHEAT_STATE::FOV_RADIUS;
float FOV_squared = FOV_RADIUS * FOV_RADIUS;
Memory mem_aim("cs2.exe");
int UPDATE_TIMER = 0;

//Resolution
int SCREEN_WIDTH = 1400;
int SCREEN_HEIGHT = 1080;

void UPDATE_CONFIG()
{
	SMOOTHING_FACTOR = COMMON_STORAGE::CHEAT_STATE::SMOOTHING_FACTOR;
	FOV_RADIUS = COMMON_STORAGE::CHEAT_STATE::FOV_RADIUS;
	FOV_squared = FOV_RADIUS * FOV_RADIUS;

}
void UPDATE_AIMBOT_RESOLUTION()
{
	SCREEN_WIDTH = COMMON_STORAGE::CHEAT_STATE::SCREEN_WIDTH;
	SCREEN_HEIGHT = COMMON_STORAGE::CHEAT_STATE::SCREEN_HEIGHT;
}


Vector findClosest(const std::vector<Vector> playerPositions)
{
	// check if the player positions vector is empty, if it is then just break out of the function.
	if (playerPositions.empty())
	{
		//printf("playerPositions vector was empty.\n");
		return { 0.0f, 0.0f, 0.0f };
	}

	// get the center of the screen to be able to subtract the playerPosition by the center of the screen so we know where they are on the screen.
	//Vector center_of_screen{ (float)GetSystemMetrics(0) / 2, (float)GetSystemMetrics(1) / 2, 0.0f };
	Vector center_of_screen{ (float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2, 0.0f };


	// keep track of the lowest distance found
	float lowestDistance = 10000;

	// find the index of the new lowest distance in the vector and store it (-1 means there wasn't one found)
	int index = -1;
	//float aspect_ratio_scale = 1;
	float aspect_ratio_scale = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
	//float aspect_ratio_scale = (16.0f / 9.0f) / ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);
	//float aspect_ratio_scale = (16.0f / 9.0f) / (4.0f / 3.0f);

	// loop through every single vector.
	for (int i = 0; i < playerPositions.size(); ++i)
	{
		float adjusted_x = playerPositions[i].x * aspect_ratio_scale; //4:3 adjsutment

		// at the current index we're at, check the playerPosition and then calculate its distance from the center.
		float distance(std::pow(playerPositions[i].x - center_of_screen.x, 2) + std::pow(playerPositions[i].y - center_of_screen.y, 2));

		// if the distance is lower than the last vector we checked, then add it and save the index.
		if (distance < lowestDistance && distance < FOV_squared /*distance < (COMMON_STORAGE::CHEAT_STATE::FOV_RADIUS * COMMON_STORAGE::CHEAT_STATE::FOV_RADIUS)*/) {
			lowestDistance = distance;
			index = i;
		}
	}

	// check if we even found a player.
	if (index == -1) {
		return { 0.0f, 0.0f, 0.0f };
	}

	// return the player at that index.
	return { playerPositions[index].x, playerPositions[index].y, 0.0f };
}


void MoveMouseToPlayer(Vector position)
{
	const float MIN_STEP_SIZE = 1.0f;

	// check if the position is valid, make a function for this for better practice. this is also just ugly.
	if (position.IsZero())
		return;

	// get the center of our screen.
	Vector center_of_screen{ (float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2, 0.0f };
	//Vector center_of_screen{ 1400.0f / 2, 1080.0f / 2, 0.0f };

	// get our new x and y, by subtracting the position by the center of the screen, giving us a position to move the mouse to.
	auto new_x = position.x - center_of_screen.x;
	auto new_y = position.y - center_of_screen.y;

	float Smooth_x = new_x / SMOOTHING_FACTOR;
	float Smooth_y = new_y / SMOOTHING_FACTOR;

	// move the mouse to said position.
	//mouse_event(MOUSEEVENTF_MOVE, new_x, new_y, 0, 0);
	mouse_event(MOUSEEVENTF_MOVE, static_cast<int>(Smooth_x), static_cast<int>(Smooth_y), 0, 0);
}


void EXECUTE(uintptr_t CLIENT)
{
	auto VIEW_MATRIX = mem_aim.Read<view_matrix_t>(CLIENT + cs2_dumper::offsets::client_dll::dwViewMatrix);
	PLAYER_SCREEN_POSITION.clear();

	//Clearing and getting enemy pointers list
	ENEMY_PTRStemp.clear();
	GET_ENTITY_LIST(ENEMY_PTRStemp);

	for (auto ENEMY : ENEMY_PTRStemp)
	{
		Vector PLAYER_POSITION = *(Vector*)(ENEMY + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin); //Foot pos
		Vector HEAD_OFFSET = *(Vector*)(ENEMY + cs2_dumper::schemas::client_dll::C_BaseModelEntity::m_vecViewOffset);
		Vector HEAD_POS(PLAYER_POSITION.x + HEAD_OFFSET.x, PLAYER_POSITION.y + HEAD_OFFSET.y, PLAYER_POSITION.z + HEAD_OFFSET.z);

		HEAD_POS.z *= PARTS[COMMON_STORAGE::CHEAT_STATE::AIMBOT_TARGET];

		Vector f, h;

		if (Vector::world_to_screen(VIEW_MATRIX, PLAYER_POSITION, f) &&
			Vector::world_to_screen(VIEW_MATRIX, HEAD_POS, h))
		{
			// Getting Enemy screen Position
			PLAYER_SCREEN_POSITION.push_back(h);
		}
	}

	if (GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON) || GetAsyncKeyState(XBUTTON2) & 0x8000)
	{
		// find the closest player and store it in a variable
		auto closest_player = findClosest(PLAYER_SCREEN_POSITION);

		// move the mouse to the player
		MoveMouseToPlayer(closest_player);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	return;
}


void AimbotThread(HINSTANCE Instance)
{

	uintptr_t CLIENT = (uintptr_t)GetModuleHandle("client.dll");
	while (!(COMMON_STORAGE::GAME_STATE::BREAK_CALL))
	{

		if (COMMON_STORAGE::GAME_STATE::IN_LOBBY || !(COMMON_STORAGE::CHEAT_STATE::AIMBOT_STATE))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			continue;
		}
		EXECUTE(CLIENT);

		UPDATE_TIMER++;
		if (UPDATE_TIMER == 50)
		{
			UPDATE_CONFIG();
			UPDATE_TIMER = 0;
		}
	}

	FreeLibraryAndExitThread(Instance, 0);
	return;
}