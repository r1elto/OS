#include "keyboard.h"

keyboard_driver::keyboard_driver(interrupt_manager* manager)
  : interrupt_handler(0x21, manager),
  dataport(0x60), commandport(0x64)
{
  while (commandport.read() & 0x1) 
     dataport.read();
  commandport.write(0xAE); 
  commandport.write(0x20);
  uint8_t status = (dataport.read() | 1) & ~0x10;
  commandport.write(0x60);
  dataport.write(status);

  dataport.write(0xF4);

}

keyboard_driver::~keyboard_driver()
{
}

void printf(char*);

uint32_t keyboard_driver::handler_interrupt(uint32_t esp)
{
  uint8_t key = dataport.read();

  if(key < 0x80)
  {
    switch (key)
    {
      case 0xFA: break;
      case 0x45: case 0xC5: break;
      default:
          char foo[] = "keyboard 0x00\n";
          char* hex = "0123456789ABCDEF";
          foo[11] = hex[(key >> 4) & 0x0F];
          foo[12] = hex[key & 0x0F];
          printf(foo);
    }
  }
  return esp;
}
