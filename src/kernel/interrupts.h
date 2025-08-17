#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "types.h"
#include "gdt.h"
#include "port.h"

class interrupt_manager;

class interrupt_handler
{
  protected:
    uint8_t interrupt_number;
    interrupt_manager* interrupt_manager_;

    interrupt_handler(uint8_t interrupt_number, interrupt_manager* interrupt_manager_);
    ~interrupt_handler();
  public:
    uint32_t handler_interrupt(uint32_t esp);
};

class interrupt_manager
{
  friend class interrupt_handler;
  protected:

    static interrupt_manager* active_interrupt_manager;
    interrupt_handler* handlers[256];
      struct gate_descriptor
      {
        uint16_t handler_address_low_bits;
        uint16_t gdt_code_segment_selector;
        uint8_t reserved;
        uint8_t access;
        uint16_t handler_address_high_bits;
      } __attribute__ ((packed));

      static gate_descriptor interrupt_descriptor_table[256];
      
      struct interrupt_descriptor_table_pointer
      {
        uint16_t size;
        uint32_t base;
      } __attribute__ ((packed));

      static void set_interrupt_descriptor_table_entry(
        uint8_t interrupt_number,
        uint16_t code_segment_selector_offset,
        void (*handler)(),
        uint8_t descriptor_privelege_level,
        uint8_t descriptor_type
        );

        port8bit_slow pic_master_command;
        port8bit_slow pic_master_data;
        port8bit_slow pic_slave_command;
        port8bit_slow pic_slave_data;

  public:
      interrupt_manager(global_descriptor_table *gdt);
      ~interrupt_manager();

      void activate();
      void deactivate();

      static uint32_t handler_interrupt(uint8_t interrupt_number, uint32_t esp);
      uint32_t do_handler_interrupt(uint8_t interrupt_number, uint32_t esp);

      static void ignore_interrupt_request();
      static void handler_interrupt_request_0x00();
      static void handler_interrupt_request_0x01();

};

#endif /*INTERRUPTS_H*/
