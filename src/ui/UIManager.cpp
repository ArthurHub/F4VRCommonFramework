#include "UIManager.h"

#include "ModBase.h"
#include "../common/Logger.h"

using namespace common;

namespace vrui
{
    // globals, globals everywhere...
    UIManager* g_uiManager;

    /**
     * Run frame update on all the containers.
     */
    void UIManager::onFrameUpdate(UIModAdapter* adapter)
    {
        const auto config = f4cf::g_mod->getConfig();

        if (!_releaseSafeList.empty()) {
            _releaseSafeList.clear();
            adapter->setInteractionHandPointing(false, false);

            // remove dev layout properties if used
            if (!config->debugVRUIProperties.empty()) {
                config->debugVRUIProperties.clear();
                config->save();
            }
        }

        if (_rootElements.empty()) {
            return;
        }

        if (!config->debugVRUIProperties.empty()) {
            readDevLayoutFromConfig();
        }

        UIFrameUpdateContext context(adapter);

        for (const auto& element : _rootElements) {
            element->onLayoutUpdate(&context);
        }

        for (const auto& element : _rootElements) {
            element->onFrameUpdate(&context);
        }

        // will need to handle exposing which hand we want to handle here
        const auto isInteractionClose = context.isAnyPressableCloseToInteraction();
        if (isInteractionClose.has_value()) {
            adapter->setInteractionHandPointing(false, isInteractionClose.value());
        }

        if (config->checkDebugDumpDataOnceFor("ui_tree")) {
            dumpUITree();
        }
    }

    /**
     * Attach the given element and subtree to the given attach game node to be rendered and layout with relation to the node.
     */
    void UIManager::attachElement(const std::shared_ptr<UIElement>& element, RE::NiNode* attachNode)
    {
        element->attachToNode(attachNode);
        // only the root can exists in the manager collection
        if (!element->getParent()) {
            logger::info("UI Manager root element added and attached to '{}'", attachNode->name.c_str());
            _rootElements.emplace_back(element);
        }
    }

    /**
     * Attach the UI on top of the primary hand and bound to the hand movement.
     */
    void UIManager::attachPresetToPrimaryWandTop(const std::shared_ptr<UIElement>& element, const float zOffset)
    {
        element->setPosition(0, 0, 5 + zOffset);
        attachElement(element, getPlayerNodes()->primaryUIAttachNode);
    }

    /**
     * Attach the UI on left of the primary hand and bound to the hand movement.
     */
    void UIManager::attachPresetToPrimaryWandLeft(const std::shared_ptr<UIElement>& element, const bool leftHanded, const RE::NiPoint3 offset)
    {
        element->setPosition((leftHanded ? -1.f : 1.f) * (offset.x - 15), offset.y, offset.z - 5);
        attachElement(element, getPlayerNodes()->primaryUIAttachNode);
    }

    /**
     * Attach the UI just below the HMD (head mounted display) direct view. Bound to horizontal but not vertical head movement.
     */
    void UIManager::attachPresetToHMDBottom(const std::shared_ptr<UIElement>& element)
    {
        element->setPosition(0, 35, -40);
        attachElement(element, findNode("world_HMD_info.nif", getPlayerNodes()->UprightHmdNode));
    }

    /**
     * Used during development to adjust VR UI layout and see the result live at runtime.
     * Allows changing values like position, scale, padding, and layout in mod main ini file.
     * The change in values is reloaded immediately and reflected in the rendered VR UI.
     * The can them update the code with the new values played with at runtime.
     */
    void UIManager::enableDevLayoutViaConfig() const
    {
        f4cf::g_mod->getConfig()->debugVRUIProperties.clear();
        for (const auto& rootElm : _rootElements) {
            rootElm->writeDevLayoutProperties("", f4cf::g_mod->getConfig()->debugVRUIProperties);
        }
        f4cf::g_mod->getConfig()->save();
    }

    /**
     * Used for development layout setting to be able to adjust the properties via config files at runtime.
     */
    void UIManager::readDevLayoutFromConfig() const
    {
        for (const auto& rootElm : _rootElements) {
            rootElm->readDevLayoutProperties("", f4cf::g_mod->getConfig()->debugVRUIProperties);
        }
    }

    /**
     * Remove the element and subtree from attached game node.
     * Safe Release: If <true>, the element will be added to release queue to be released on the next frame update
     * so finishing access to it on this frame update is still safe (release UI while handling UI event).
     */
    void UIManager::detachElement(const std::shared_ptr<UIElement>& element, const bool releaseSafe)
    {
        element->detachFromAttachedNode(releaseSafe);

        // only the root can exists in the manager collection
        if (element->getParent()) {
            return;
        }
        for (auto it = _rootElements.begin(); it != _rootElements.end(); ++it) {
            if (it->get() == element.get()) {
                if (releaseSafe) {
                    _releaseSafeList.push_back(*it);
                }
                logger::info("UI Manager root element removed (ReleaseSafe: {})", releaseSafe);
                _rootElements.erase(it);
                break;
            }
        }
    }

    /**
     * Dump all the managed UI elements trees in a nice tree format.
     */
    void UIManager::dumpUITree() const
    {
        if (_rootElements.empty()) {
            logger::info("--- UI Manager EMPTY ---");
            return;
        }

        for (const auto& element : _rootElements) {
            logger::info("--- UI Manager Root ---");
            dumpUITreeRecursive(element.get(), "");
        }
    }

    void UIManager::dumpUITreeRecursive(UIElement* element, std::string padding)
    {
        logger::infoRaw("{}{}", padding.c_str(), element->toString().c_str());
        const auto container = dynamic_cast<UIContainer*>(element);
        if (!container) {
            return;
        }
        padding += "..";
        for (const auto& child : container->childElements()) {
            dumpUITreeRecursive(child.get(), padding);
        }
    }
}
