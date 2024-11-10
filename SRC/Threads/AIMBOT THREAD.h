#pragma once

#include <Windows.h>
#include <thread>
#include <vector>

#include "../Memory/Memory.h"
#include "../Storage/global.h"
#include "../Math/Vector.h"
#include "../a2x/post.h"
#include "../a2x/pre.h"

extern std::vector<Vector> PLAYER_SCREEN_POSITION;
extern std::vector<uintptr_t> ENEMY_PTRStemp;
extern int SMOOTHING_FACTOR;
extern float FOV_RADIUS;
extern Memory mem_aim;
extern int UPDATE_TIMER;

//Resolution
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

//Functions
void UPDATE_CONFIG();
void UPDATE_AIMBOT_RESOLUTION();

Vector findClosest(const std::vector<Vector> playerPositions);

void MoveMouseToPlayer(Vector position);

void EXECUTE(uintptr_t CLIENT);

void AimbotThread(HINSTANCE Instance);