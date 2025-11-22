#pragma once

#include "ConfigBase.h"

namespace f4cf
{
    class ModBase
    {
    public:
        struct Settings
        {
            std::string name;
            std::string version;
            ConfigBase* config;
            std::string logFileName = name;
            int trampolineAllocationSize = 256;
            bool setupMainGameLoop = false;

            Settings(const std::string_view& name, const std::string_view& version, ConfigBase* config);

            Settings(const std::string_view& name, const std::string_view& version, ConfigBase* config, int trampolineAllocationSize, bool setupMainGameLoop);

            Settings(const std::string_view& name, const std::string_view& version, ConfigBase* config, const std::string_view& logFileName, int trampolineAllocationSize,
                bool setupMainGameLoop);
        };

        explicit ModBase(Settings settings);
        virtual ~ModBase() = default;

        ConfigBase* getConfig() const { return _settings.config; }

        bool onF4SEPluginQuery(const F4SE::QueryInterface* skse, F4SE::PluginInfo* info) const;
        bool onF4SEPluginLoad(const F4SE::LoadInterface* f4se);

        void onFrameUpdateSafe();

    private:
        void logPluginGameStart() const;
        static void onF4VRSEMessage(F4SE::MessagingInterface::Message* msg);
        void onGameLoadedInner();
        void onGameSessionLoadedInner();

    protected:
        // Run F4SE plugin load and initialize the plugin given the init handle.
        virtual void onModLoaded(const F4SE::LoadInterface* f4SE) = 0;

        // On game fully loaded initialize things that should be initialized only once.
        virtual void onGameLoaded() = 0;

        // Game session can be initialized multiple times as it is fired on new game and save loaded events.
        virtual void onGameSessionLoaded() = 0;

        // Runs on every game frame, main logic goes here.
        virtual void onFrameUpdate() = 0;

        // Dump game data if requested in "sDebugDumpDataOnceNames" flag in INI config.
        virtual void checkDebugDump() const;

        Settings _settings;
        const F4SE::MessagingInterface* _messaging = nullptr;
    };

    // The ONE global to rule them ALL
    inline ModBase* g_mod;
}
