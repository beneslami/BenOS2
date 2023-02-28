#ifndef ATA_H
#define ATA_H

#include <hardwarecommunication/port.h>
#include <common/types.h>

namespace BenOS{
    namespace drivers{
        class AdvancedTechnologyAttachment{
        protected:
            BenOS::hardwarecommunication::Port8Bit dataPort;
            BenOS::hardwarecommunication::Port8Bit errorPort;
            BenOS::hardwarecommunication::Port8Bit sectorCountPort;
            BenOS::hardwarecommunication::Port8Bit lbaLowPort;
            BenOS::hardwarecommunication::Port8Bit lbaMidPort;
            BenOS::hardwarecommunication::Port8Bit lbaHiPort;
            BenOS::hardwarecommunication::Port8Bit devicePort;
            BenOS::hardwarecommunication::Port8Bit commandPort;
            BenOS::hardwarecommunication::Port8Bit controlPort;
            bool master;
            BenOS::common::uint16_t bytesPerSector;
        public:
            AdvancedTechnologyAttachment(BenOS::common::uint16_t portBase, bool master);
            ~AdvancedTechnologyAttachment();
            void Identify();
            void Read28(BenOS::common::uint32_t sector, BenOS::common::uint8_t *data, int count);
            void Write28(BenOS::common::uint32_t sector, BenOS::common::uint8_t *data, int count);
            void Flush();
        };
    }
}

#endif