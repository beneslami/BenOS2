#ifndef VGA_H
#define VGA_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>
#include <hardwarecommunication/interrupts.h>

namespace BenOS{
    namespace drivers{
        class VideoGraphicsArray{
        protected:
            hardwarecommunication::Port8Bit miscPort;
            hardwarecommunication::Port8Bit crtcIndexPort;
            hardwarecommunication::Port8Bit crtcDataPort;
            hardwarecommunication::Port8Bit sequenceIndexPort;
            hardwarecommunication::Port8Bit sequenceDataPort;
            hardwarecommunication::Port8Bit graphicsControllerIndexPort;
            hardwarecommunication::Port8Bit graphicsControllerDataPort;
            hardwarecommunication::Port8Bit attributeControllerIndexPort;
            hardwarecommunication::Port8Bit attributeControllerReadPort;
            hardwarecommunication::Port8Bit attributeControllerWritePort;
            hardwarecommunication::Port8Bit attributeControllerResetPort;
            void WriteRegisters(BenOS::common::uint8_t *registers);
            BenOS::common::uint8_t *GetFrameBufferSegment();
            virtual BenOS::common::uint8_t GetColorIndex(BenOS::common::uint8_t r, BenOS::common::uint8_t g, BenOS::common::uint8_t b);
        public:
            VideoGraphicsArray();
            ~VideoGraphicsArray();
            virtual bool SupportsMode(BenOS::common::uint32_t width, BenOS::common::uint32_t height, BenOS::common::uint32_t colordepth);
            virtual bool SetMode(BenOS::common::uint32_t width, BenOS::common::uint32_t height, BenOS::common::uint32_t colordepth);
            virtual void PutPixel(BenOS::common::int32_t x, BenOS::common::int32_t y, BenOS::common::uint8_t r, BenOS::common::uint8_t g, BenOS::common::uint8_t b);
            virtual void PutPixel(BenOS::common::int32_t x, BenOS::common::int32_t y, BenOS::common::uint8_t colorIndex);
            virtual void FillRectangle(BenOS::common::uint32_t x, BenOS::common::uint32_t y, BenOS::common::uint32_t w, BenOS::common::uint32_t h, BenOS::common::uint8_t r, BenOS::common::uint8_t g, BenOS::common::uint8_t b);
        };
    }
}

#endif