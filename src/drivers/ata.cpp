#include <drivers/ata.h>

using namespace BenOS;
using namespace BenOS::drivers;
using namespace BenOS::common;
using namespace BenOS::hardwarecommunication;

void printf(char *str);

AdvancedTechnologyAttachment::AdvancedTechnologyAttachment(uint16_t portBase, bool master)
    :dataPort(portBase), errorPort(portBase + 1), sectorCountPort(portBase + 2), lbaLowPort(portBase + 3),
    lbaMidPort(portBase + 4), lbaHiPort(portBase + 5), devicePort(portBase + 6), commandPort(portBase + 7),
    controlPort(portBase + 0x206){
    bytesPerSector = 512;
    this->master = master;
}

AdvancedTechnologyAttachment::~AdvancedTechnologyAttachment(){

}

void AdvancedTechnologyAttachment::Identify(){
    devicePort.Write(master ? 0xA0 : 0xB0);
    controlPort.Write(0);
    devicePort.Write(0xA0);
    uint8_t status = commandPort.Read();
    if(status == 0xFF)
        return;
    devicePort.Write(master ? 0xA0 : 0xB0);
    sectorCountPort.Write(0);
    lbaLowPort.Write(0);
    lbaMidPort.Write(0);
    lbaHiPort.Write(0);
    commandPort.Write(0xEC);
    status = commandPort.Read();
    if(status == 0x00)
        return;
    while(((status & 0x80) == 0x80) && ((status & 0x01) != 0x01))
        status = commandPort.Read();
    if(status & 0x01){
        printf("Error");
        return;
    }
    for(uint16_t i = 0; i < 256; i++){
        uint16_t data = dataPort.Read();
        char *temp = " \0";
        temp[1] = (data >> 8) & 0x00FF;
        temp[0] = data & 0x00FF;
        printf(temp);
    }
}

void AdvancedTechnologyAttachment::Read28(uint32_t sector, uint8_t *data, int count){
    if(sector & 0xF00000000)
        return;
    if(count > bytesPerSector)
        return;
    devicePort.Write((master ? 0xE0 : 0xF0) | ((sector & 0x0F0000000) >> 24));
    errorPort.Write(0);
    sectorCountPort.Write(0xA0);

    lbaLowPort.Write((sector & 0x000000FF));
    lbaMidPort.Write((sector & 0x0000FF00) >> 8 );
    lbaHiPort.Write((sector & 0x00FF0000) >> 16);
    commandPort.Write(0x20);

    uint8_t status = commandPort.Read();
    while(((status & 0x80) == 0x80) && ((status & 0x01) != 0x01))
        status = commandPort.Read();
    if(status & 0x01){
        printf("Error");
        return;
    }
    printf("Reading from ATA\n");
    for(uint16_t i = 0; i < count; i+=2){
        uint16_t wdata = dataPort.Read();

        char *temp = " \0";
        temp[1] = (wdata >> 8) & 0x00FF;
        temp[0] = wdata & 0x00FF;
        printf(temp);
        data[i] = wdata & 0x00FF;
        if(i+1 < count)
            data[i+1] = (wdata >> 8) & 0x00FF;
    }
    for(uint16_t i = count + (count % 2); i < bytesPerSector; i+=2)
        dataPort.Read();
}

void AdvancedTechnologyAttachment::Write28(uint32_t sector, uint8_t *data, int count){
    if(sector & 0xF00000000)
        return;
    if(count > bytesPerSector)
        return;
    devicePort.Write((master ? 0xE0 : 0xF0) | ((sector & 0x0F0000000) >> 24));
    errorPort.Write(0);
    sectorCountPort.Write(0xA0);

    lbaLowPort.Write((sector & 0x000000FF));
    lbaMidPort.Write((sector & 0x0000FF00) >> 8 );
    lbaHiPort.Write((sector & 0x00FF0000) >> 16);
    commandPort.Write(0x30);
    printf("Writing to ATA: ");

    for(uint16_t i = 0; i < count; i+=2){
        uint16_t wdata = data[i];
        if(i + 1 < count)
            wdata |= ((uint16_t)data[i+1]) << 8;
        char *temp = " \0";
        temp[1] = (wdata >> 8) & 0x00FF;
        temp[0] = wdata & 0x00FF;
        printf(temp);
        dataPort.Write(wdata);
    }
    for(uint16_t i = count + (count % 2); i < bytesPerSector; i+=2)
        dataPort.Write(0x0000);
}

void AdvancedTechnologyAttachment::Flush(){
    devicePort.Write(master ? 0xE0 : 0xF0);
    commandPort.Write(0xE7);
    uint8_t status = commandPort.Read();
    while(((status & 0x80) == 0x80) && ((status & 0x01) != 0x01))
        status = commandPort.Read();
    if(status & 0x01){
        printf("Error");
        return;
    }

}