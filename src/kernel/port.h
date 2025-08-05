#ifndef PORT_H
#define PORT_H

#include "types.h"

class port
{
    protected:
        uint16_t portnumber;
        port(uint16_t portnumber);
        ~port();
};

class port8bit : public port
{
    public:
        port8bit(uint8_t portnumber);
        ~port8bit();
        virtual void write(uint8_t data);
        virtual uint8_t read();
};

class port8bit_slow : public port
{
    public:
        port8bit_slow(uint8_t portnumber);
        ~port8bit_slow();
        virtual void write(uint8_t data);
        virtual uint8_t read();
};

class port16bit : public port
{
    public:
        port16bit(uint16_t portnumber);
        ~port16bit();
        virtual void write(uint16_t data);
        virtual int16_t read();
};

class port32bit : public port
{
    public:
        port32bit(uint32_t portnumber);
        ~port32bit();
        virtual void write(uint32_t data);
        virtual uint32_t read();
};

#endif /*PORT_H*/
