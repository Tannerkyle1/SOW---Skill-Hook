#include <framework.h>

int keyParam;
void ReadConfig()
{
	WCHAR inputBuffer[50];
	WCHAR configName[] = L".\\plugins\\multiskill.ini";

	GetPrivateProfileString(L"General", L"ToggleSkillEquip", L"0x0061", inputBuffer, 50, configName);
	keyParam = std::stoi(inputBuffer, nullptr, 16);
}

bool isAttached = true;
bool wasKeyPressed = false;
void ThreadMain(void *arg)
{
	ReadConfig();
	while(isAttached)
	{
		Sleep(50);
		if(GetKeyState(keyParam) & 0x8000)
		{
			if(!wasKeyPressed)
			{
				wasKeyPressed = true;
				GameHook::ToggleSkillEquipHook();
			}
		}
		else wasKeyPressed = false;
	}
}

DLLHOOK OnLoad()
{
	GameHook::CreateGameHooks();
	_beginthread(ThreadMain, 0, nullptr);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD callReason, LPVOID lpReserved)
{
	if(callReason == DLL_PROCESS_ATTACH)
		DisableThreadLibraryCalls(hModule);
	else if(callReason == DLL_PROCESS_DETACH)
		isAttached = false;

    return TRUE;
}