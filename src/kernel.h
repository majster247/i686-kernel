#ifndef __OS__KERNEL_H
#define __OS__KERNEL_H

#include "gdt.h"
#include "types.h"

using namespace os;
using namespace os::gdt;
namespace os {
    void clearScreen() {
        uint16_t* VideoMemory = (uint16_t*) 0xb8000;
        for(int i = 0; i < 80 * 25; i++) {
            VideoMemory[i] = 0x0F00; // Clear screen with white text on black background
        }
    }


    void printf(const char* str, ...);

    // Simple cpu info reading function
    void CPUInfo() {
    uint32_t eax, ebx, ecx, edx;

    // CPUID function 0: Get vendor string
    __asm__ volatile("cpuid"
                 : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                 : "a"(0));

    char vendor[13];
    *(uint32_t*)&vendor[0]  = ebx;
    *(uint32_t*)&vendor[4]  = edx;
    *(uint32_t*)&vendor[8]  = ecx;
    vendor[12] = 0;
    printf("Vendor: %s\n", vendor);

    // CPUID function 1: Features and basic info
    __asm__ volatile("cpuid"
                 : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                 : "a"(1));

    uint8_t cpu_stepping = eax & 0xF;
    uint8_t cpu_model    = (eax >> 4) & 0xF;
    uint8_t cpu_family   = (eax >> 8) & 0xF;
    printf("Family: %d, Model: %d, Stepping: %d\n", cpu_family, cpu_model, cpu_stepping);

    // Logical processors per package (max threads per CPU)
    uint8_t logical_cores = (ebx >> 16) & 0xFF;
if (logical_cores == 0) {
    printf("Logical CPUs: unknown or 1 (legacy)\n");
} else {
    printf("Logical CPUs: %d\n", logical_cores);
}

    // Hyper-threading check (bit 28 in EDX)
    if (edx & (1 << 28)) {
        printf("Hyper-Threading: Yes\n");
    } else {
        printf("Hyper-Threading: No\n");
    }

    // Instruction support
    if (edx & (1 << 25)) printf("SSE supported\n");
    if (edx & (1 << 26)) printf("SSE2 supported\n");
    if (ecx & (1 << 0))  printf("SSE3 supported\n");
    if (ecx & (1 << 28)) printf("AVX supported\n");

    // CPUID function 0x16: Frequency info (optional, not always supported)
    __asm__ volatile("cpuid"
                 : "=a"(eax), "=b"(ebx)
                 : "a"(0x16)
                 : "ecx", "edx");

    if (eax > 0) {
        printf("Base Clock: %d MHz\n", eax);
        printf("Max  Clock: %d MHz\n", ebx);
    }
}

}

#endif