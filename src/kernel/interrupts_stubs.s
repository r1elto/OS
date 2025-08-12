.section .text

.extern _ZN17interrupt_manager17handler_interruptEhj

.macro handler_interrupt_request num
.global _ZN17interrupt_manager18handler_interruptRequest\num\()Ev
  movb $\num, (interruptnumber)
  jmp int_bottom
.endm

.macro handler_interrupt_request num
.global _ZN17interrupt_manager26handler_interruptRequest\num\()Ev
  movb $\num + IRQ_BASE, (interruptnumber)
  jmp int_bottom
.endm

handler_interrupt_request 0x00
handler_interrupt_request 0x01

int_bottom:
  pusha
  pushl %ds
  pushl %es
  pushl %fs
  pushl %gs

  pushl %esp
  push (interruptnumber)
  call _ZN17interrupt_manager17handler_interruptEhj
  movl %eax, %esp

  popl %gs
  popl %fs
  popl %es
  popl %ds
  popa

  iret

.data
  interruptnumber: .byte 0
