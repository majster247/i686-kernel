#include "interrupts.h"

using namespace os;
using namespace os::interrupts;

void printf(const char* str, ...);

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

void InterruptManager::SetInterruptDescriptorTableEntry(
    uint8_t interruptNumber,
    uint16_t codeSegmentSelectorOffset,
    void (*handler)(),
    uint8_t DescriptorPriviligeLevel,
    uint8_t DescriptorType
) {
    const uint8_t IDT_DESC_PRESENT = 0x80;

    interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].handlerAddressHighBits = ((uint32_t)handler >> 16) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
    interruptDescriptorTable[interruptNumber].reserved = 0;
    interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPriviligeLevel&3) << 5);
}


InterruptManager::InterruptManager(GlobalDescriptorTable* gdt) {
    uint16_t CodeSegment = gdt->CodeSegmentSelector();
    const uint8_t IDT_DESC_INTERRUPT_GATE = 0xE;

    for (uint16_t i = 0; i < 256; i++) {
       SetInterruptDescriptorTableEntry(i, CodeSegment, IgnoreInterruptRequest, 0, IDT_DESC_INTERRUPT_GATE);
    }
}
InterruptManager::~InterruptManager() {}



uint32_t InterruptManager::handleInterrupt(uint32_t interruptNumber, uint32_t esp) {



    return esp;
}