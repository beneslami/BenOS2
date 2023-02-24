#ifndef PCI_H
#define PCI_H

#include <hardwarecommunication/port.h>
#include <drivers/driver.h>
#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <memorymanagement.h>

namespace BenOS {
    namespace hardwarecommunication{

        enum BaseAddressRegisterType{
            MemoryMapping = 0,
            InputOutput = 1
        };

        class BaseAddressRegister{
        public:
            bool prefetchable;
            BenOS::common::uint8_t *address;
            BenOS::common::uint32_t size;
            BaseAddressRegisterType type;
        };

        class PeripheralComponentInterconnectDeviceDescriptor {
        public:
            BenOS::common::uint32_t portBase;
            BenOS::common::uint32_t interrupt;
            BenOS::common::uint16_t bus;
            BenOS::common::uint16_t device;
            BenOS::common::uint16_t function;
            BenOS::common::uint16_t vendor_id;
            BenOS::common::uint16_t device_id;
            BenOS::common::uint8_t class_id;
            BenOS::common::uint8_t subclass_id;
            BenOS::common::uint8_t interface_id;
            BenOS::common::uint8_t revision;
            PeripheralComponentInterconnectDeviceDescriptor();
            ~PeripheralComponentInterconnectDeviceDescriptor();
        };

        class PeripheralComponentInterconnectController {
        private:
            Port32Bit dataPort;
            Port32Bit commandPort;
        public:
            PeripheralComponentInterconnectController();
            ~PeripheralComponentInterconnectController();
            common::uint32_t Read(BenOS::common::uint16_t bus, BenOS::common::uint16_t device, BenOS::common::uint16_t function, BenOS::common::uint32_t registeroffset);
            void Write(BenOS::common::uint16_t bus, BenOS::common::uint16_t device, BenOS::common::uint16_t function, BenOS::common::uint32_t registeroffset, BenOS::common::uint32_t value);
            bool DeviceHasFunctions(BenOS::common::uint16_t bus, BenOS::common::uint16_t device);
            void SelectDrivers(BenOS::drivers::DriverManager *driverManager, BenOS::hardwarecommunication::InterruptManager *interrupts);
            BenOS::drivers::Driver *GetDriver(PeripheralComponentInterconnectDeviceDescriptor dev, BenOS::hardwarecommunication::InterruptManager *interrupts);
            PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(BenOS::common::uint16_t bus, BenOS::common::uint16_t device, BenOS::common::uint16_t function);
            BaseAddressRegister GetBaseAddressRegister(BenOS::common::uint16_t bus, BenOS::common::uint16_t device, BenOS::common::uint16_t function, BenOS::common::uint16_t bar);
        };
    }
}
#endif