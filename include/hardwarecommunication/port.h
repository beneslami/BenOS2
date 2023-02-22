#ifndef PORT_H
#define PORT_H

#include <common/types.h>

namespace BenOS {
    namespace hardwarecommunication {
        class Port {
        protected:
            Port(BenOS::common::uint16_t portNum);
            ~Port();
            BenOS::common::uint16_t portNumber;
        };

        class Port8Bit : public Port {
        public:
            Port8Bit(BenOS::common::uint16_t portNum);
            ~Port8Bit();
            virtual void Write(BenOS::common::uint8_t data);
            virtual BenOS::common::uint8_t Read();
        };

        class Port8BitSlow : public Port8Bit {
        public:
            Port8BitSlow(BenOS::common::uint16_t portNum);
            ~Port8BitSlow();
            void Write(BenOS::common::uint8_t data);
        };

        class Port16Bit : public Port {
        public:
            Port16Bit(BenOS::common::uint16_t portNum);
            ~Port16Bit();
            void Write(BenOS::common::uint16_t data);
            BenOS::common::uint16_t Read();
        };

        class Port32Bit : public Port {
        public:
            Port32Bit(BenOS::common::uint16_t portNum);
            ~Port32Bit();
            void Write(BenOS::common::uint32_t data);
            BenOS::common::uint32_t Read();
        };
    }
}

#endif