#pragma once

#include "UIContainer.h"
#include "UIElement.h"
#include "UIToggleButton.h"

namespace vrui
{
    class UIToggleGroupContainer : public UIContainer
    {
    public:
        explicit UIToggleGroupContainer(const std::string& name, const UIContainerLayout layout = UIContainerLayout::Manual, const float padding = 0) :
            UIContainer(name, layout, padding) {}

        void addElement(const std::shared_ptr<UIToggleButton>& button);

        void clearToggleState() const;

        virtual std::string toString() const override;

    protected:
        virtual void onStateChanged(UIElement* element) override;
    };
}
