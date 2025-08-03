#ifndef __OS__INTERRUPTS_H
#define __OS__INTERRUPTS_H

#include "types.h"
#include "port.h"
#include "gdt.h"

using namespace os;
using namespace os::gdt;


namespace os {
    namespace interrupts {
        class InterruptManager {
            protected:
                struct GateDescriptor {
                    uint16_t handlerAddressLowBits;
                    uint16_t gdt_codeSegmentSelector;
                    uint8_t reserved;
                    uint8_t access;
                    uint16_t handlerAddressHighBits;
                    
                } __attribute__((packed));

                static GateDescriptor interruptDescriptorTable[256];

                static void SetInterruptDescriptorTableEntry(
                    uint8_t interruptNumber,
                    uint16_t codeSegmentSelectorOffset,
                    void (*handler)(),
                    uint8_t DescriptorPriviligeLevel,
                    uint8_t DescriptorType
                );

            public:
                InterruptManager(GlobalDescriptorTable* gdt);
                ~InterruptManager();

                static uint32_t handleInterrupt(uint32_t interruptNumber, uint32_t esp);


                static void IgnoreInterruptRequest();
                static void HandleInterruptRequest0x00();
                static void HandleInterruptRequest0x01();
       };
    }
}

#endif