#ifndef DESKTOP_H
#define DESKTOP_H

#include <gui/widget.h>
#include <drivers/mouse.h>

namespace BenOS{
    namespace gui{
        class Desktop : public CompositeWidget, public BenOS::drivers::MouseEventHandler{
        protected:
            BenOS::common::uint32_t MouseX;
            BenOS::common::uint32_t MouseY;
        public:
            Desktop(BenOS::common::uint32_t w, BenOS::common::uint32_t h, BenOS::common::uint8_t r, BenOS::common::uint8_t g, BenOS::common::uint8_t b);
            ~Desktop();
            void Draw(BenOS::common::GraphicsContext *gc);
            void OnMouseDown(BenOS::common::uint8_t button);
            void OnMouseUp(BenOS::common::uint8_t button);
            void OnMouseMove(int x, int y);
        };
    }
}

#endif