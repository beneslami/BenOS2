#ifndef SYSCALL_H
#define SYSCALL_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <multitasking.h>

namespace BenOS{
    class SyscallHandler : public hardwarecommunication::InterruptHandler{
    public:
        SyscallHandler(hardwarecommunication::InterruptManager* interruptManager, BenOS::common::uint8_t InterruptNumber);
        ~SyscallHandler();
        virtual BenOS::common::uint32_t HandleInterrupt(BenOS::common::uint32_t esp);
    };
}

#endif