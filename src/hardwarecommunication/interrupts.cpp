#include <hardwarecommunication/interrupts.h>

using namespace BenOS;
using namespace BenOS::common;
using namespace BenOS::hardwarecommunication;

void printf(char* str);
void printfHex(uint8_t);
InterruptHandler::InterruptHandler(uint8_t interruptNumber, InterruptManager *interruptManager) {
    this->interruptNumber = interruptNumber;
    this->interruptManager = interruptManager;
    interruptManager->handlers[interruptNumber] = this;
}

InterruptHandler::~InterruptHandler() {
    if(interruptManager->handlers[interruptNumber] = this)
        interruptManager->handlers[interruptNumber] = 0;
}

BenOS::common::uint32_t InterruptHandler::HandleInterrupt(uint32_t esp) {
    return esp;
}


InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

InterruptManager *InterruptManager::ActiveInterruptManager = 0;

BenOS::common::uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp) {
    if(ActiveInterruptManager != 0){
        return ActiveInterruptManager->DohandleInterrupt(interruptNumber, esp);
    }
    return esp;
}

BenOS::common::uint32_t InterruptManager::DohandleInterrupt(uint8_t interruptNumber, uint32_t esp) {
    if(handlers[interruptNumber] != 0){
        esp = handlers[interruptNumber]->HandleInterrupt(esp);
    }
    else if(interruptNumber != hardwareInterruptOffset) {
        printf("Unhandled interrupt 0x00");
        printfHex(interruptNumber);
    }
    if(interruptNumber == hardwareInterruptOffset){
        esp = (uint32_t)taskManager->Schedule((CPUState*)esp);
    }
    if(hardwareInterruptOffset <= interruptNumber && interruptNumber < hardwareInterruptOffset + 16){
        picMasterCommand.Write(0x20);
        if(0x28 <= interruptNumber)
            picSlaveCommand.Write(0x20);
    }
    return esp;
}

void InterruptManager::SetInterruptDescriptorTableEntry(uint8_t interruptNumber,
                                                               uint16_t gdt_codeSegmentSelectorOffset,
                                                               void (*handler)(), uint8_t DescriptorPrivilegeLevel,
                                                               uint8_t DescriptorType) {
    const uint8_t IDT_DESC_PRESENT = 0x80;
    interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].handleAddressHighBits = (((uint32_t)handler) >> 16)& 0xFFFF;
    interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = gdt_codeSegmentSelectorOffset;
    interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPrivilegeLevel&3) << 5);
    interruptDescriptorTable[interruptNumber].reserved = 0;
}

InterruptManager::InterruptManager(uint16_t hardwareInterruptOffset, GlobalDescriptorTable *gdt, TaskManager *taskManager): picMasterCommand(0x20), picMasterData(0x21),
                                                                picSlaveCommand(0xA0), picSlaveData(0xA1) {
    this->taskManager = taskManager;
    this->hardwareInterruptOffset = hardwareInterruptOffset;
    uint16_t CodeSegment = gdt->CodeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    for(uint16_t i = 0; i < 256; i++){
        handlers[i] = 0;
        SetInterruptDescriptorTableEntry(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }
    SetInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x2C, CodeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);
    picMasterCommand.Write(0x11);
    picSlaveCommand.Write(0x11);
    picMasterData.Write(0x20);
    picSlaveData.Write(0x28);
    picMasterData.Write(0x04);
    picSlaveData.Write(0x02);
    picMasterData.Write(0x01);
    picSlaveData.Write(0x01);
    picMasterData.Write(0x00);
    picSlaveData.Write(0x00);
    InterruptDescriptorTablePointer idt;
    idt.size = 256*sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)interruptDescriptorTable;
    __asm__ volatile("lidt %0" : : "m" (idt));
}

InterruptManager::~InterruptManager() {
    Deactivate();
}

void InterruptManager::Activate() {
    if(ActiveInterruptManager != 0)
        ActiveInterruptManager->Deactivate();
    ActiveInterruptManager = this;
    asm("sti");
}

void InterruptManager::Deactivate() {
    if(ActiveInterruptManager == this) {
        ActiveInterruptManager = 0;
        asm("cli");
    }
}