#pragma once
#define STANDING 65665
#define CROUCHING 65667

#define HEAD = 1
#define NECK = 1.2
#define CHEST = 1.6
#define STOMACK = 2
#define PP = 3
#define FEET = 50



#include <Windows.h>
#include <vector>

#include "../Storage/global.h"
#include "../a2x/post.h"
#include "../a2x/pre.h"

extern std::vector<DOUBLE> PARTS;

//Input Variables
extern uintptr_t CLIENT;
//extern INPUT WHEEL_DOWN;
//extern INPUT WHEEL_UP;

class MISCBOT
{
public:
	// Contructor
	MISCBOT(uintptr_t BaseAdrs, INPUT& mousedown, INPUT& mouseup, INPUT& LeftMouse);

	//Misc Cheats
	void BunnyHop();
	void TriggerBot();
	void AntiAFK();
private:
	uintptr_t CLIENT;
	INPUT MOUSE_DOWN;
	INPUT MOUSE_UP;
	INPUT LEFT_MOUSE;
};

//Functions
void Execute(INPUT& WHEEL_DOWN, INPUT& WHEEL_UP, uintptr_t CLIENT);
void MiscThread(HINSTANCE Instance);