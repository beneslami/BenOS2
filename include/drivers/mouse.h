#ifndef MOUSE_H
#define MOUSE_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>

namespace BenOS {
    namespace drivers {
        class MouseEventHandler {
        public:
            MouseEventHandler();
            virtual void OnActivate();
            virtual void OnMouseDown(BenOS::common::uint8_t button);
            virtual void OnMouseUp(BenOS::common::uint8_t button);
            virtual void OnMouseMove(int x, int y);
        };

        class MouseDriver : public BenOS::hardwarecommunication::InterruptHandler, public Driver {
            BenOS::hardwarecommunication::Port8Bit dataport;
            BenOS::hardwarecommunication::Port8Bit commandport;
            BenOS::common::uint8_t buffer[3];
            BenOS::common::uint8_t offset;
            BenOS::common::uint8_t button;
            MouseEventHandler *handler;
        public:
            MouseDriver(BenOS::hardwarecommunication::InterruptManager *manager, MouseEventHandler *handler);
            ~MouseDriver();
            virtual BenOS::common::uint32_t HandleInterrupt(BenOS::common::uint32_t esp);
            virtual void Activate();
        };
    }
}
#endif
