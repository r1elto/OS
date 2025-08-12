#include "types.h"
#include "gdt.h"

typedef void (*constructor)();

void printf(char* str)
{
    uint16_t* VideoMemory = (uint16_t*)0xb8000;
    
    for (int16_t i = 0; str[i] != '\0'; ++i)
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
}

extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void call_constructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; ++i)
        (*i)();
}


extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber)
{
    printf("Hello World!");
    global_descriptor_table gdt;

    for (;;);
}
