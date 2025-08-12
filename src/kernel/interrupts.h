#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "types.h"
#include "gdt.h"
#include "port.h"


class interrupt_manager
{
  protected:
      struct gate_descriptor
      {
        uint16_t handler_address_low_bits;
        uint16_t gdt_code_segment_selector;
        uint8_t reserved;
        uint8_t access;
        uint16_t handler_address_high_bits;
      } __attribute__ ((packed))

      static gate_descriptor interrupt_descriptor_table[256];
  
      static void set_interrupt_descriptor_table_entry 
      {
        uint8_t interrupt_number,
        uint16_t code_segment_selector_offset;
        void (*handler)(),
        uint8_t descriptor_privilege_level;
        uint8_t descriptor_type;
      };
  public:
      static uint32_t handler_interrupt(uint8_t interrupt_number, uint32_t esp);
      
      static void ignore_interrupt_request();
      static void handler_interrupt_request_0x00();
      static void handler_interrupt_request_0x01();

};

#endif /*INTERRUPTS_H*/
