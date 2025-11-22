#include "UIElement.h"

#include <format>
#include <stdexcept>

namespace vrui
{
    std::string UIElement::toString() const
    {
        return std::format("UIElement({}): {}, Pos({:.2f}, {:.2f}, {:.2f}), Size({:.2f}, {:.2f})",
            _name,
            _visible ? "V" : "H",
            _transform.translate.x,
            _transform.translate.y,
            _transform.translate.z,
            _size.width,
            _size.height
            );
    }

    void UIElement::attachToNode(RE::NiNode* attachNode)
    {
        if (_attachNode) {
            throw std::runtime_error(
                "Attempt to attach already attached widget: " + std::string(attachNode->name.c_str()));
        }
        _attachNode.reset(attachNode);
    }

    void UIElement::detachFromAttachedNode(bool)
    {
        if (!_attachNode) {
            throw std::runtime_error("Attempt to detach NOT attached widget");
        }
        _attachNode = nullptr;
    }

    /**
     * calculate the transform of the element with respect to all parents.
     */
    RE::NiTransform UIElement::calculateTransform() const
    {
        if (!_parent) {
            return _transform;
        }

        auto calTransform = _transform;
        const auto parentTransform = _parent->calculateTransform();
        calTransform.translate += parentTransform.translate;
        calTransform.scale *= parentTransform.scale;
        // TODO: add rotation handling
        return calTransform;
    }

    /**
     * Call "onPressEventFired" on this element and all elements up the UI tree.
     */
    void UIElement::onPressEventFiredPropagate(UIElement* element, UIFrameUpdateContext* context)
    {
        onPressEventFired(element, context);
        if (_parent) {
            _parent->onPressEventFiredPropagate(element, context);
        }
    }

    /**
     * On state change of the element propagate that it happen up the UI tree to let containers handle child state changes.
     * If overridden, make sure to call the parent.
     */
    void UIElement::onStateChanged(UIElement* element)
    {
        if (_parent) {
            _parent->onStateChanged(element);
        }
    }

    /**
     * Write the layout properties of the element to the given map.
     * Used for development layout setting to be able to adjust the properties via config files at runtime.
     */
    void UIElement::writeDevLayoutProperties(const std::string& namePrefix, std::map<std::string, std::string>& propertiesMap) const
    {
        propertiesMap[namePrefix + _name] = std::format("Pos:({:.2f},{:.2f},{:.2f}), Scale:({:.2f}), Size:({:.2f},{:.2f})",
            getPosition().x, getPosition().y, getPosition().z, getScale(), getSize().width, getSize().height);
    }

    /**
     * Read the layout properties of the element to the given map.
     * Used for development layout setting to be able to adjust the properties via config files at runtime.
     */
    void UIElement::readDevLayoutProperties(const std::string& namePrefix, const std::map<std::string, std::string>& propertiesMap)
    {
        const auto key = namePrefix + _name;
        if (!propertiesMap.contains(key)) {
            return;
        }
        try {
            float x, y, z, scale, width, height;
            if (std::sscanf(propertiesMap.at(key).c_str(), "Pos:(%f,%f,%f), Scale:(%f), Size:(%f,%f)", &x, &y, &z, &scale, &width, &height) == 6) { // NOLINT(cert-err34-c)
                setPosition(x, y, z);
                setScale(scale);
                setSize(width, height);
            }
        } catch (std::exception& e) {
            logger::warn("Failed to read VRUI properties in element '{}': {}", _name, e.what());
        }
    }
}
