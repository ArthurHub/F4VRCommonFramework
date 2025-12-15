#pragma once

#include "vrcf/VRControllersManager.h"

namespace f4cf::f4vr
{
    struct Skelly
    {
        static void initBoneTreeMap();
        static const std::string& getBoneName(int index);
        static RE::NiTransform getBoneWorldTransform(const std::string& boneName);
        static RE::NiPoint3 getIndexFingerTipWorldPosition(const vrcf::Hand& hand);

    private:
        inline static std::map<std::string, int> _boneTreeMap;
        inline static std::vector<std::string> _boneTreeVec;
    };
}
