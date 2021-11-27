#pragma region imports

#include <windows.h>
#include <Psapi.h>
#include <vector>

#include "MCM.h"
#include "MinHook.h"

#include "SDK/Player.h"

#pragma endregion

#pragma region Fastcall Defines

typedef void(__thiscall* player)(Player* plr, void* a2);
player _actorTick;

#pragma endregion

#pragma region hookFuncs

void actorTick(Player* plr, void* a2) {
    _actorTick(plr, a2); // call the original mc function

    plr->onGround = true; // Airjump
}

#pragma endregion

void Init(LPVOID c) {
    if (MH_Initialize() == MH_OK) { // main hooks here
        uintptr_t actorAddr = MCM::findSig("F3 0F 10 81 ? ? ? ? 41 0F 2F 00");
        if (MH_CreateHook((void*)actorAddr, &actorTick, reinterpret_cast<LPVOID*>(&_actorTick)) == MH_OK) {
            MH_EnableHook((void*)actorAddr);
        };
    }
}

BOOL APIENTRY DllMain(HMODULE a, DWORD b, LPVOID c)
{
    if (b == DLL_PROCESS_ATTACH)
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Init, a, 0, 0);
    return TRUE;
}