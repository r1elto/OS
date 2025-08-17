#include "interrupts.h"
#include "types.h"

void printf(char* str);

interrupt_handler::interrupt_handler(uint8_t interrupt_number, interrupt_manager* interrupt_manager_)
{
  this->interrupt_number = interrupt_number;
  this->interrupt_manager_ = interrupt_manager_;
  interrupt_manager_->handlers[interrupt_number] = this;
}

interrupt_handler::~interrupt_handler()
{
  if(interrupt_manager_->handlers[interrupt_number] == this)
    interrupt_manager_->handlers[interrupt_number] = 0;
}

uint32_t interrupt_handler::handler_interrupt(uint32_t esp)
{
  return esp;
}

interrupt_manager::gate_descriptor interrupt_manager::interrupt_descriptor_table[256];

interrupt_manager* interrupt_manager::active_interrupt_manager = 0;

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
  {
    handlers[i] = 0;
    set_interrupt_descriptor_table_entry(i, code_segment, &ignore_interrupt_request, 0, IDT_INTERRUPT_GATE);
  }
  set_interrupt_descriptor_table_entry(0x20, code_segment, &handler_interrupt_request_0x00, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(0x21, code_segment, &handler_interrupt_request_0x01, 0, IDT_INTERRUPT_GATE);
  
  pic_master_command.write(0x11);
  pic_slave_command.write(0x11);

  pic_master_data.write(0x20);
  pic_slave_data.write(0x28);

  pic_master_data.write(0x04);
  pic_slave_data.write(0x02);

  pic_master_data.write(0x01);
  pic_slave_data.write(0x01);

  pic_master_data.write(0xFC);
  pic_slave_data.write(0xFF);

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
  if(active_interrupt_manager != 0)
      active_interrupt_manager-> deactivate();
    active_interrupt_manager = this;
    asm("sti");
 }

void interrupt_manager::deactivate()
{
  if (active_interrupt_manager == this)
  {
    active_interrupt_manager = 0;
    asm ("cli");
  }
}

uint32_t interrupt_manager::handler_interrupt(uint8_t interrupt_number, uint32_t esp)
{
  if(active_interrupt_manager != 0)
  {
    return active_interrupt_manager->do_handler_interrupt(interrupt_number, esp);
  }
  return esp;
}

uint32_t interrupt_manager::do_handler_interrupt(uint8_t interrupt_number, uint32_t esp)
{
  if(handlers[interrupt_number] != 0)
  {
    esp = handlers[interrupt_number]->handler_interrupt(esp);
  }
  else if(interrupt_number != 0x20)
  {
    char* foo = "unhandled interrupt";
    char* hex = "012345678ABCDEF";
    foo[22] = hex[(interrupt_number >> 4) & 0x0F];
    foo[23] = hex[interrupt_number & 0x0F];
    printf(foo);
  }
  if(interrupt_number != 0x20)
  {
    printf("\n this was an interrupt");
  }
  if(0x20 <= interrupt_number && interrupt_number < 0x30)
  {
    pic_master_command.write(0x20);
    if(0x28 <= interrupt_number)
      pic_slave_command.write(0x20);
  }
  return esp;
}
