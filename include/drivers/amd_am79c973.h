#ifndef AMD_AM79C973_H
#define AMD_AM79C973_H

#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecommunication/pci.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/port.h>

namespace BenOS{
    namespace drivers{
        class amd_am79c973 : public Driver, public hardwarecommunication::InterruptHandler{
            struct InitializationBlock{
                BenOS::common::uint16_t mode;
                unsigned reserved1 : 4;
                unsigned numSendBuffers : 4;
                unsigned reserved2 : 4;
                unsigned numRecvBuffers : 4;
                BenOS::common::uint64_t physicalAddress : 48;
                BenOS::common::uint16_t reserved3;
                BenOS::common::uint64_t logicalAddress;
                BenOS::common::uint64_t recvBufferDescrAddress;
                BenOS::common::uint64_t sendBufferDescrAddress;
            }__attribute__((packed));

            struct BufferDescriptor{
                BenOS::common::uint32_t address;
                BenOS::common::uint32_t flags;
                BenOS::common::uint32_t flags2;
                BenOS::common::uint32_t avail;
            }__attribute__((packed));

            hardwarecommunication::Port16Bit MACAddress0Port;
            hardwarecommunication::Port16Bit MACAddress2Port;
            hardwarecommunication::Port16Bit MACAddress4Port;
            hardwarecommunication::Port16Bit registerDataPort;
            hardwarecommunication::Port16Bit registerAddressPort;
            hardwarecommunication::Port16Bit resetPort;
            hardwarecommunication::Port16Bit busControlRegisterDataPort;
            InitializationBlock initBlock;
            BufferDescriptor *sendBufferDescr;
            BufferDescriptor *recvBufferDescr;
            BenOS::common::uint8_t sendBufferDescrMemory[2048+15];
            BenOS::common::uint8_t recvBufferDescrMemory[2048+15];
            BenOS::common::uint8_t sendBuffers[2*1024+15][8];
            BenOS::common::uint8_t recvBuffers[2*1024+15][8];
            BenOS::common::uint8_t currentSendBuffer;
            BenOS::common::uint8_t currentRecvBuffer;
        public:
            amd_am79c973(BenOS::hardwarecommunication::PeripheralComponentInterconnectDeviceDescriptor *dev,
                         BenOS::hardwarecommunication::InterruptManager *interrupts);
            ~amd_am79c973();
            void Activate();
            int Reset();
            BenOS::common::uint32_t HandleInterrupt(BenOS::common::uint32_t esp);
            void Send(BenOS::common::uint8_t *buffer, int count);
            void Receive();
        };
    }
}

#endif
