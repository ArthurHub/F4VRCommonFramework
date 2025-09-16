#pragma once

#include "common/ConfigBase.h"

namespace f4cf
{
    class ModBase
    {
    public:
        ModBase(const std::string_view& name, const std::string_view& version, common::ConfigBase* config, int trampolineAllocationSize = 256);
        virtual ~ModBase() = default;

        common::ConfigBase* getConfig() const { return _config; }

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

        // Get the string to be used for the log file name.
        virtual std::string_view getLogFileName() const { return _name; }

        std::string _name;
        std::string _version;
        common::ConfigBase* _config = nullptr;
        const F4SE::MessagingInterface* _messaging = nullptr;
    };

    // The ONE global to rule them ALL
    inline ModBase* g_mod;
}
