#include "Config.h"

using namespace common;

namespace my_mod
{
    static constexpr auto DEFAULT_SECTION = std::string(Version::PROJECT).c_str();

    void Config::loadIniConfigInternal(const CSimpleIniA& ini)
    {
        // TODO: load config from ini
        myConfigValue = static_cast<float>(ini.GetDoubleValue(DEFAULT_SECTION, "fMyConfigValue", 3.0));
    }
}
