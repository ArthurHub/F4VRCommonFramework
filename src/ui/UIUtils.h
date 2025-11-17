#pragma once

#include "UIElement.h"
#include "f4vr/PlayerNodes.h"

namespace vrui
{
    inline std::string getDebugSphereNifName() { return "FRIK/1x1Sphere.nif"; }
    inline std::string getToggleButtonFrameNifName() { return "FRIK/ui_common_btn_border.nif"; }

    /**
     * Get the size of a UI element based on its width-to-height ratio.
     * The way VR UI nifs are defined is that the height is always 2.0f units and the width is
     * the only one that changes if the shape is not square.
     * The reason height is 2.0f and not 1.0f is to easily make the center of the UI element at (0,0,0)
     */
    inline UISize getElementSize(const float widthToHeightRatio)
    {
        return UISize(2.0f * widthToHeightRatio, 2.0f);
    }

    f4vr::PlayerNodes* getPlayerNodes();
    void setNodeVisibility(RE::NiNode* node, bool visible, float originalScale);
    std::tuple<RE::NiNode*, float> getUINodeFromNifFile(const std::string& path);
    RE::NiNode* findNode(const char* nodeName, RE::NiNode* node);
}
