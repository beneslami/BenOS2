#ifndef GDT_H
#define GDT_H

#include <common/types.h>

namespace BenOS {
    class GlobalDescriptorTable {
    public:
        class SegmentDescriptor {
        private:
            BenOS::common::uint16_t limit_lo;
            BenOS::common::uint16_t base_lo;
            BenOS::common::uint8_t base_hi;
            BenOS::common::uint8_t type;
            BenOS::common::uint8_t flags_limit_hi;
            BenOS::common::uint8_t base_vhi;
        public:
            SegmentDescriptor(BenOS::common::uint32_t base, BenOS::common::uint32_t limit, BenOS::common::uint8_t type);
            BenOS::common::uint32_t Base();
            BenOS::common::uint32_t Limit();
        }__attribute__((packed));

        SegmentDescriptor nullSegmentSelector;
        SegmentDescriptor unusedSegmentSelector;
        SegmentDescriptor codeSegmentSelector;
        SegmentDescriptor dataSegmentSelector;
    public:
        GlobalDescriptorTable();
        ~GlobalDescriptorTable();
        BenOS::common::uint16_t CodeSegmentSelector();
        BenOS::common::uint16_t DataSegmentSelector();
    };
}

#endif
