#include "UIUtils.h"

#include "common/Logger.h"

namespace
{
    /**
     * Extract the float value from the nif node name in this format: "VRUI (W/H:4.5)"
     */
    float extractWidthToHeightValueFronName(const std::string& nifName)
    {
        const auto pos = nifName.find(':');
        if (pos == std::string::npos)
            return -1.0f;

        const auto end = nifName.find(')', pos);
        if (end == std::string::npos)
            return -1.0f;

        const auto num = nifName.substr(pos + 1, end - pos - 1);
        return std::stof(num);
    }
}

namespace vrui
{
    /**
     * Get struct with useful RE::NiNodes references related to player.
     */
    f4vr::PlayerNodes* getPlayerNodes()
    {
        const auto player = RE::PlayerCharacter::GetSingleton();
        auto* nodes = reinterpret_cast<f4vr::PlayerNodes*>(reinterpret_cast<std::uintptr_t>(player) + 0x6E0);
        return nodes;
    }

    /**
     * Update the node flags to show/hide it.
     */
    void setNodeVisibility(RE::NiNode* node, const bool visible, const float originalScale)
    {
        node->local.scale = visible ? originalScale : 0;

        // TODO: try to understand why it's not working for our nifs.
        //if (visible)
        //	node->flags &= 0xfffffffffffffffe; // show
        //else
        //	node->flags |= 0x1; // hide
        //RE::NiAVObject::NiUpdateData* ud = nullptr;
        //node->UpdateWorldData(ud);
    }

    /**
     * Get a RE::NiNode that can be used in game UI for the given .nif file.
     * Why is just loading not enough?
     */
    std::tuple<RE::NiNode*, float> getUINodeFromNifFile(const std::string& path)
    {
        auto& normPath = path._Starts_with("Data") ? path : "Data/Meshes/" + path;
        const auto nifNode = f4vr::getClonedNiNodeForNifFile(normPath);
        const float widthHeightRatio = extractWidthToHeightValueFronName(nifNode->name.c_str());
        if (widthHeightRatio < 0) {
            common::logger::warn("UI node nif doesn't contain width-to-height ratio data! (Nif: {})", path.c_str());
        }
        nifNode->name = RE::BSFixedString(path.c_str());
        return { nifNode, widthHeightRatio };
    }

    /**
     * Find node by name in the given subtree of the given root node.
     */
    RE::NiNode* findNode(const char* nodeName, RE::NiNode* node)
    {
        if (!node) {
            return nullptr;
        }

        if (_stricmp(nodeName, node->name.c_str()) == 0) {
            return node;
        }

        for (const auto& child : node->children) {
            if (child) {
                if (const auto childNiNode = child->IsNode()) {
                    if (const auto result = findNode(nodeName, childNiNode)) {
                        return result;
                    }
                }
            }
        }
        return nullptr;
    }
}
