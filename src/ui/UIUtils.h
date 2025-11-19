#pragma once

#include "UIElement.h"

namespace vrui
{
    struct UIUtils
    {
        static std::string getDebugSphereNifName() { return "FRIK/1x1Sphere.nif"; }
        static std::string getEmptyButtonFrameNifName() { return "FRIK/ui_common_btn_empty.nif"; }
        static std::string getToggleButtonFrameNifName() { return "FRIK/ui_common_btn_border.nif"; }

        static UISize getElementSize(float widthToHeightRatio);

        static RE::NiNode* getPrimaryWandAttachNode();
        static RE::NiNode* getHMDAttachNode();
        static bool isLeftHandedMode();
        static void triggerInteractionHeptic();
        static void setNodeVisibility(RE::NiNode* node, bool visible, float originalScale);
        static std::tuple<RE::NiNode*, float> getUINodeFromNifFile(const std::string& path);
        static RE::NiNode* findNode(RE::NiNode* node, const char* nodeName);
    };
}
