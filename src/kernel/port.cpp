
#include "port.h"
#include "types.h"

port::port(uint16_t portnumber)
{
    this->portnumber = portnumber;
}

port::~port()
{
}

port8bit::port8bit(uint8_t portnumber)
   : port(portnumber)
{
}

void port8bit::write(uint8_t data)
{
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (portnumber));
}

uint8_t port8bit::read()
{
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (portnumber));
    return result;
}

port8bit_slow::port8bit_slow(uint8_t portnumber)
   : port(portnumber)
{
}

port8bit_slow::~port8bit_slow()
{
}

void port8bit_slow::write(uint8_t data)
{
    __asm__ volatile("outb %0, %1\njmp lf\n1" : : "a" (data), "Nd" (portnumber));
}

uint8_t port8bit_slow::read()
{
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (portnumber));
    return result;
}

port16bit::port16bit(uint16_t portnumber)
   : port(portnumber)
{
}


port16bit::~port16bit()
{
}

void port16bit::write(uint16_t data)
{
    __asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (portnumber));
}

uint16_t port16bit::read()
{
    uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (portnumber));
    return result;
}


port32bit::port32bit(uint32_t portnumber)
   : port(portnumber)
{
}

port32bit::~port32bit()
{
}

void port32bit::write(uint32_t data)
{
    __asm__ volatile("outl %0, %1" : : "a" (data), "Nd" (portnumber));
}

uint32_t port32bit::read()
{
    uint32_t result;
    __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (portnumber));
    return result;
}

