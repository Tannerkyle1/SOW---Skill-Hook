#include <framework.h>

namespace GameHook
{
    // Multi Skill Equip
    const SHORT bytePattern[] = { 0x74, 0x100, 0x48, 0x8B, 0x100, 0x40, 0x04, 0x00, 0x00, 0x48, 0x85, 0x100, 0x74, 0x100, 0x4C, 0x39, 0x100, 0x40, 0x74, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x08, 0x00, 0x00, 0x00 };

    const SHORT patternLength = 32;

    const SHORT patternOffset = -0x12;

    // Name of the module in which to scan for byte patterns.
    const WCHAR moduleName[] = L"ShadowOfWar.exe";

    AOBScanner pScanner;

    void Set(const BYTE* writeAddress, const std::vector<BYTE>& newBytes)
    {
        MEMORY_BASIC_INFORMATION mbi{};
        if (!VirtualQuery(writeAddress, &mbi, sizeof(mbi)) || mbi.State != MEM_COMMIT || mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD))
            return;

        DWORD oldProtect;
        if(!VirtualProtect((LPVOID)writeAddress, newBytes.size(), PAGE_READWRITE, &oldProtect))
            return;

        WriteProcessMemory(GetCurrentProcess(), (LPVOID)writeAddress, newBytes.data(), newBytes.size(), nullptr);
        VirtualProtect((LPVOID)writeAddress, newBytes.size(), oldProtect, &oldProtect);
    }

    // Injections and scanned addresses.
    extern "C"
    {
        EXPORT BYTE* skillEquipAddress = nullptr;
    }

    void CreateSkillEquipHook()
    {
        skillEquipAddress = pScanner.PerformModuleScan(bytePattern, patternLength, patternOffset, moduleName);
    }

    bool IsSkillHookEnabled = false;
    void ToggleSkillEquipHook(bool noSound)
    {
        if(!noSound)
            PlaySound(L".\\plugins\\sounds\\onEquip.wav", NULL, SND_ASYNC);
        if(IsSkillHookEnabled)
            Set(skillEquipAddress, {0x74}), IsSkillHookEnabled = false;
        else
            Set(skillEquipAddress, {0xEB}), IsSkillHookEnabled = true;
    }


    // Hook startup.
    void CreateGameHooks()
    {
        CreateSkillEquipHook();
    }
}