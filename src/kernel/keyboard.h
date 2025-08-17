#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "types.h"
#include "port.h"
#include "interrupts.h"

class keyboard_driver : public interrupt_handler
{
  port8bit dataport;
  port8bit commandport;
public:
  keyboard_driver(interrupt_manager* manager);
  ~keyboard_driver();
  virtual uint32_t handler_interrupt(uint32_t esp);
};

#endif /*keyboard.h*/
