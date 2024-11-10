#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <thread>
//#include <iostream>

#include "../Storage/global.h"
#include "../a2x/pre.h"
#include "../a2x/post.h"
#include "../Memory/Memory.h"


extern Memory mem;

//Functions
//void CreateConsole();

void Entity_Finder(uintptr_t CLIENT);

void EnemyThread(HINSTANCE Instance);

