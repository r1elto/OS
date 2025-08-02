typedef unsigned int    u32;
typedef unsigned short  u16;
typedef void (*constructor)();

void printf(char* str)
{
    u16* VideoMemory = (u16*)0xb8000;
    
    for (int i = 0; str[i] != '\0'; ++i)
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
}

extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void call_constructors()
{
    for(constructor* i = &start_ctors; i != end_ctors; ++i)
        (*i)();
}


extern "C" void kernelMain(void* multiboot_structure, u32 magicnumber)
{
    for (;;);
}
