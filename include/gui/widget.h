#ifndef WIDGET_H
#define WIDGET_H

#include <common/types.h>
#include <common/graphicscontext.h>
#include <drivers/keyboard.h>

namespace BenOS{
    namespace gui{
    class Widget : public BenOS::drivers::KeyboardEventHandler{
        protected:
            Widget *parent;
            BenOS::common::int32_t x;
            BenOS::common::int32_t y;
            BenOS::common::int32_t w;
            BenOS::common::int32_t h;
            BenOS::common::uint8_t r;
            BenOS::common::uint8_t g;
            BenOS::common::uint8_t b;
            bool Focussable;
        public:
            Widget(Widget *parent, BenOS::common::int32_t x, BenOS::common::int32_t y, BenOS::common::int32_t w, BenOS::common::int32_t h, BenOS::common::uint8_t r, BenOS::common::uint8_t g, BenOS::common::uint8_t b);
            ~Widget();
            virtual void GetFocus(Widget *widget);
            virtual void ModelToScreen(BenOS::common::int32_t &x, BenOS::common::int32_t &y);
            virtual void Draw(BenOS::common::GraphicsContext *gc);
            virtual void OnMouseDown(BenOS::common::int32_t x, BenOS::common::int32_t y, BenOS::common::uint8_t button);
            virtual void OnMouseUp(BenOS::common::int32_t x, BenOS::common::int32_t y, BenOS::common::uint8_t button);
            virtual void OnMouseMove(BenOS::common::int32_t oldx, BenOS::common::int32_t oldy, BenOS::common::int32_t newx, BenOS::common::int32_t newy);
            virtual bool ContainsCoordinate(common::int32_t x, common::int32_t y);
        };

        class CompositeWidget : public Widget{
        private:
            Widget *children[100];
            int numChildren;
            Widget *focussedChild;
        public:
            CompositeWidget(Widget *parent, BenOS::common::int32_t x, BenOS::common::int32_t y, BenOS::common::int32_t w, BenOS::common::int32_t h, BenOS::common::uint8_t r, BenOS::common::uint8_t g, BenOS::common::uint8_t b);
            ~CompositeWidget();
            virtual void GetFocus(Widget *widget);
            virtual bool AddChild(Widget *child);
            virtual void Draw(BenOS::common::GraphicsContext *gc);
            virtual void OnMouseDown(BenOS::common::int32_t x, BenOS::common::int32_t y, BenOS::common::uint8_t button);
            virtual void OnMouseUp(BenOS::common::int32_t x, BenOS::common::int32_t y, BenOS::common::uint8_t button);
            virtual void OnMouseMove(BenOS::common::int32_t oldx, BenOS::common::int32_t oldy, BenOS::common::int32_t newx, BenOS::common::int32_t newy);
            virtual void OnKeyDown(char);
            virtual void OnKeyUp(char);
        };
    }
}

#endif