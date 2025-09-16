#pragma once

#include "Resources.h"
#include "common/ConfigBase.h"

namespace my_mod
{
    static const auto BASE_PATH = common::BASE_PATH + "\\" + std::string(Version::PROJECT);
    static const auto INI_PATH = BASE_PATH + "\\" + std::string(Version::PROJECT) + ".ini";

    class Config : public common::ConfigBase
    {
    public:
        explicit Config() :
            ConfigBase(Version::PROJECT, INI_PATH, IDR_CONFIG_INI) {}

        // Mod configs
        float myConfigValue = 0.0f;

    protected:
        virtual void loadIniConfigInternal(const CSimpleIniA& ini) override;
    };

    // Global singleton for easy access
    inline Config g_config;
}
