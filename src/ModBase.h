#pragma once

#include "common/ConfigBase.h"

namespace f4cf
{
    class ModBase
    {
    public:
        ModBase(const std::string_view& name, const std::string_view& project, std::uint32_t version, common::ConfigBase* config);
        virtual ~ModBase() = default;

        common::ConfigBase* getConfig() const { return _config; }

        bool onF4SEPluginQuery(const F4SE::QueryInterface* skse, F4SE::PluginInfo* info) const;
        bool onF4SEPluginLoad(const F4SE::LoadInterface* f4se);

    private:
        void logPluginGameStart();
        static void onF4VRSEMessage(F4SE::MessagingInterface::Message* msg);
        void onGameLoadedInner();
        void onGameSessionLoadedInner();

    protected:
        virtual void onModLoaded(const F4SE::LoadInterface* f4SE) = 0;
        virtual void onGameLoaded() = 0;
        virtual void onGameSessionLoaded() = 0;

        std::string _name;
        std::string _project;
        std::uint32_t _version = 0;
        common::ConfigBase* _config = nullptr;
        const F4SE::MessagingInterface* _messaging = nullptr;
    };

    // The ONE global to rule them ALL
    inline ModBase* g_mod;
}
