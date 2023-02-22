#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>

namespace BenOS {
    namespace drivers {
        class KeyboardEventHandler {
        public:
            KeyboardEventHandler();
            virtual void OnKeyDown(char);
            virtual void OnKeyUp(char);
        };

        class KeyboardDriver : public BenOS::hardwarecommunication::InterruptHandler, public Driver {
        private:
            BenOS::hardwarecommunication::Port8Bit dataport;
            BenOS::hardwarecommunication::Port8Bit commandport;
            KeyboardEventHandler *handler;
        public:
            KeyboardDriver(BenOS::hardwarecommunication::InterruptManager *manager, KeyboardEventHandler *handler);
            ~KeyboardDriver();
            virtual BenOS::common::uint32_t HandleInterrupt(BenOS::common::uint32_t esp);
            virtual void Activate();
        };
    }
}
#endif
