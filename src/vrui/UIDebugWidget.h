#pragma once

#include "UIWidget.h"

namespace f4cf::vrui
{
    class UIDebugWidget : public UIWidget
    {
    public:
        explicit UIDebugWidget(const bool followInteractPos = false);
        virtual std::string toString() const override;

        virtual void onFrameUpdate(UIFrameUpdateContext* adapter) override;

        bool isFollowInteractionPosition() const;
        void setFollowInteractionPosition(const bool followInteractionPosition);

    protected:
        bool _followInteractionPosition = false;
    };
}
