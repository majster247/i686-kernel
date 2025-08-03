#include "kernel.h"

using namespace os;
using namespace os::gdt;

int32_t terminalX = 0;
int32_t terminalY = 0;

void os::printf(const char* str, ...) {
    uint16_t* VideoMemory = (uint16_t*) 0xb8000;

    // Only support up to 5 variables, and only %d (int) and %s (string)
    // Arguments must be passed as: printf("...", arg1, arg2, arg3)
    // This version does NOT use va_list or stdlib

    int args[5];
    const char* strArgs[5];

    // Manually extract up to 5 arguments from the stack
    // This is x86-specific and assumes cdecl calling convention
    // Arguments are after the format string on the stack
    // Use compiler extensions or inline assembly if needed for portability

    // Get pointer to first argument (after 'str')
    int* argPtr = (int*)(&str + 1);

    int argIndex = 0;
    for(int i = 0; str[i] != '\0' && argIndex < 3; i++) {
        if(str[i] == '%') {
            i++;
            if(str[i] == 'd') {
                args[argIndex] = *argPtr;
                strArgs[argIndex] = nullptr;
                argPtr++;
                argIndex++;
            } else if(str[i] == 's') {
                strArgs[argIndex] = (const char*)(*argPtr);
                args[argIndex] = 0;
                argPtr++;
                argIndex++;
            }
        }
    }

    int argUsed = 0;
    for(int i = 0; str[i] != '\0'; i++) {
        if(str[i] == '%') {
            i++;
            if(str[i] == 'd' && argUsed < 5) {
                int value = args[argUsed];
                char buf[12];
                int idx = 0;
                if (value < 0) {
                    VideoMemory[terminalY * 80 + terminalX] = (VideoMemory[terminalY * 80 + terminalX] & 0xFF00) | '-';
                    terminalX++;
                    value = -value;
                }
                do {
                    buf[idx++] = '0' + (value % 10);
                    value /= 10;
                } while (value);
                for(int j = idx - 1; j >= 0; j--) {
                    VideoMemory[terminalY * 80 + terminalX] = (VideoMemory[terminalY * 80 + terminalX] & 0xFF00) | buf[j];
                    terminalX++;
                }
                argUsed++;
            } else if(str[i] == 's' && argUsed < 5) {
                const char* s = strArgs[argUsed];
                for(int j = 0; s && s[j] != '\0'; j++) {
                    VideoMemory[terminalY * 80 + terminalX] = (VideoMemory[terminalY * 80 + terminalX] & 0xFF00) | s[j];
                    terminalX++;
                }
                argUsed++;
            }
        } else if(str[i] == '\n') {
            terminalX = 0;
            terminalY++;
        } else {
            VideoMemory[terminalY * 80 + terminalX] = (VideoMemory[terminalY * 80 + terminalX] & 0xFF00) | str[i];
            terminalX++;
        }
    }
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors() {
    for(constructor* i = &start_ctors; i != &end_ctors; ++i) {
        (*i)();
    }
}

extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber) {
    clearScreen();
    printf("Kernel initialized successfully!\n");

    //print cpu info
    printf("CPU Information:\n");
    CPUInfo();

    // Initialize Global Descriptor Table
    printf("Initializing Global Descriptor Table...\n");
    GlobalDescriptorTable gdt;
    printf("GDT initialized with Code Segment: %04x, Data Segment: %04x\n", gdt.CodeSegmentSelector(), gdt.DataSegmentSelector());

    while(1);
}

