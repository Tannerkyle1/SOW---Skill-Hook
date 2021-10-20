#ifndef HOOKS
    #define HOOKS

    namespace GameHook
    {
        void CreateSkillEquipHook();
        void ToggleSkillEquipHook(bool = false);

        // Sets up all the code injections and hooks for the game and reads the config file.
        void CreateGameHooks();
    }
#endif