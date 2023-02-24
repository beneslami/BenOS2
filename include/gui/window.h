#ifndef WINDOW_H
#define WINDOW_H

#include <gui/widget.h>
#include <drivers/mouse.h>

namespace BenOS{
    namespace gui{
        class Window : public CompositeWidget {
        protected:
            bool Dragging;
        public:
            Window(Widget *parent, BenOS::common::int32_t x, BenOS::common::int32_t y, BenOS::common::int32_t w, BenOS::common::int32_t h, BenOS::common::uint8_t r, BenOS::common::uint8_t g, BenOS::common::uint8_t b);
            ~Window();
            void OnMouseDown(BenOS::common::int32_t x, BenOS::common::int32_t y, BenOS::common::uint8_t button);
            void OnMouseUp(BenOS::common::int32_t x, BenOS::common::int32_t y, BenOS::common::uint8_t button);
            void OnMouseMove(BenOS::common::int32_t oldx, BenOS::common::int32_t oldy, BenOS::common::int32_t newx, BenOS::common::int32_t newy);
        };
    }
}

#endif