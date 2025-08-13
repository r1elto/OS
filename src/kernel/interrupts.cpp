#include "interrupts.h"
#include "types.h"

void printf(char* str);

interrupt_manager::gate_descriptor interrupt_manager::interrupt_descriptor_table[256];

void interrupt_manager::set_interrupt_descriptor_table_entry(
  uint8_t interrupt_number,
  uint16_t code_segment_selector_offset,
  void (*handler)(),
  uint8_t descriptor_privelege_level,
  uint8_t descriptor_type)
{
  const uint8_t IDT_DESC_PRESENT = 0x80;

  interrupt_descriptor_table[interrupt_number].handler_address_low_bits = ((uint32_t)handler) & 0xFFFF;
  interrupt_descriptor_table[interrupt_number].handler_address_high_bits = (((uint32_t)handler) >> 16) & 0xFFFF;
  interrupt_descriptor_table[interrupt_number].gdt_code_segment_selector = code_segment_selector_offset;
  interrupt_descriptor_table[interrupt_number].access = IDT_DESC_PRESENT | descriptor_type | ((descriptor_privelege_level&3) << 5);
  interrupt_descriptor_table[interrupt_number].reserved = 0;
}

interrupt_manager::interrupt_manager(global_descriptor_table *gdt)
  : pic_master_command(0x20),
    pic_master_data(0x21),
    pic_slave_command(0xA0),
    pic_slave_data(0xA1)
{
  uint16_t code_segment = gdt->code_segment_selector();
  const uint8_t IDT_INTERRUPT_GATE = 0xE;

  for(uint8_t i = 0; i < 256; i++)
    set_interrupt_descriptor_table_entry(i, code_segment, &ignore_interrupt_request, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(0x20, code_segment, &handler_interrupt_request_0x00, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(0x21, code_segment, &handler_interrupt_request_0x01, 0, IDT_INTERRUPT_GATE);
  
  pic_master_command.write(0x11);
  pic_slave_command.write(0x11);

  pic_slave_data.write(0x20);
  pic_slave_data.write(0x28);

  pic_master_data.write(0x04);
  pic_slave_data.write(0x02);

  pic_master_data.write(0x01);
  pic_slave_data.write(0x01);

  pic_master_data.write(0x00);
  pic_slave_data.write(0x00);

  interrupt_descriptor_table_pointer idt;
  idt.size = 256 * sizeof(gate_descriptor) - 1;
  idt.base = (uint32_t)interrupt_descriptor_table;
  asm volatile("lidt %0" : : "n" (idt));
}

interrupt_manager::~interrupt_manager()
{
}

void interrupt_manager::activate()
{
  asm("sti");
}

uint32_t interrupt_manager::handler_interrupt(uint8_t interrupt_number, uint32_t esp)
{
  printf(" this was an interrupt\n");
  return esp;
}
