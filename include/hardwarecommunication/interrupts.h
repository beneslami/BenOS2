#ifndef INTERRUPT_H
#define INTERRUPT_H
#include <common/types.h>
#include <multitasking.h>
#include <hardwarecommunication/port.h>
#include <gdt.h>

namespace BenOS {
    namespace hardwarecommunication {
        class InterruptManager;

        class InterruptHandler {
        protected:
            BenOS::common::uint8_t interruptNumber;
            InterruptManager *interruptManager;
            InterruptHandler(BenOS::common::uint8_t interruptNumber, InterruptManager *interruptManager);
            ~InterruptHandler();
        public:
            virtual BenOS::common::uint32_t HandleInterrupt(BenOS::common::uint32_t esp);
        };

        class InterruptManager {
            friend class InterruptHandler;
        protected:
            static InterruptManager *ActiveInterruptManager;
            InterruptHandler *handlers[256];
            TaskManager *taskManager;
            struct GateDescriptor {
                BenOS::common::uint16_t handlerAddressLowBits;
                BenOS::common::uint16_t gdt_codeSegmentSelector;
                BenOS::common::uint8_t reserved;
                BenOS::common::uint8_t access;
                BenOS::common::uint16_t handleAddressHighBits;
            }__attribute__((packed));
            static GateDescriptor interruptDescriptorTable[256];
            struct InterruptDescriptorTablePointer {
                BenOS::common::uint16_t size;
                BenOS::common::uint32_t base;
            }__attribute__((packed));
            BenOS::common::uint16_t hardwareInterruptOffset;
            static void SetInterruptDescriptorTableEntry(BenOS::common::uint8_t interruptNumber, BenOS::common::uint16_t gdt_codeSegmentSelectorOffset,
                                             void (*handler)(), BenOS::common::uint8_t DescriptorPrivilegeLevel,
                                             BenOS::common::uint8_t DescriptorType);
            Port8BitSlow picMasterCommand;
            Port8BitSlow picMasterData;
            Port8BitSlow picSlaveCommand;
            Port8BitSlow picSlaveData;

        public:
            InterruptManager(BenOS::common::uint16_t hardwareInterruptOffset, BenOS::GlobalDescriptorTable *gdt, BenOS::TaskManager *taskManager);
            ~InterruptManager();
            void Activate();
            void Deactivate();
            static BenOS::common::uint32_t handleInterrupt(BenOS::common::uint8_t interruptNumber, BenOS::common::uint32_t esp);
            BenOS::common::uint32_t DohandleInterrupt(BenOS::common::uint8_t interruptNumber, BenOS::common::uint32_t esp);
            static void IgnoreInterruptRequest();
            static void HandleInterruptRequest0x00();
            static void HandleInterruptRequest0x01();
            static void HandleInterruptRequest0x0C();
        };
    }
}

#endif