#pragma once

#include "F4VRUtils.h"

namespace f4cf::f4vr
{
    struct F4VRThumbstickControls
    {
        /**
         * If to enable/disable the use of primary controller analog thumbstick.
         * Optional: add "fDirectionalDeadzone:Controls" to restrict secondary hand thumbstick as well.
         */
        static void setControlsThumbstickEnableState(const bool toEnable)
        {
            if (_controlsThumbstickEnableState == toEnable) {
                return; // no change
            }
            _controlsThumbstickEnableState = toEnable;
            logger::debug("Set player controls thumbstick '{}'", toEnable ? "enable" : "disable");
            if (toEnable) {
                f4vr::getIniSetting("fLThumbDeadzone:Controls")->SetFloat(_controlsThumbstickOriginalDeadzone);
                f4vr::getIniSetting("fLThumbDeadzoneMax:Controls")->SetFloat(_controlsThumbstickOriginalDeadzoneMax);
            } else {
                const auto controlsThumbstickOriginalDeadzone = f4vr::getIniSetting("fLThumbDeadzone:Controls")->GetFloat();
                if (controlsThumbstickOriginalDeadzone < 1) {
                    _controlsThumbstickOriginalDeadzone = controlsThumbstickOriginalDeadzone;
                    _controlsThumbstickOriginalDeadzoneMax = f4vr::getIniSetting("fLThumbDeadzoneMax:Controls")->GetFloat();
                } else {
                    logger::warn("Controls thumbstick deadzone is already set to 1.0, not changing it.");
                }
                f4vr::getIniSetting("fLThumbDeadzone:Controls")->SetFloat(1.0);
                f4vr::getIniSetting("fLThumbDeadzoneMax:Controls")->SetFloat(1.0);
            }
        }

    private:
        inline static bool _controlsThumbstickEnableState = true;
        inline static float _controlsThumbstickOriginalDeadzone = 0.25f;
        inline static float _controlsThumbstickOriginalDeadzoneMax = 0.94f;
    };
}
