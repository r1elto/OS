.set IRQ_BASE, 0x20

.section .text

.extern _ZN17interrupt_manager17handler_interruptEhj
.global _ZN17interrupt_manager24ignore_interrupt_requestEv


.macro handle_exception num
.global _ZN17interrupt_manager18handle_exception\num\()Ev
_ZN17interrupt_manager18handle_exception\num\()Ev:
  movb $\num, (interruptnumber)
  jmp int_bottom
.endm

.macro handle_interrupt_request num
.global _ZN17interrupt_manager26handler_interrupt_request\num\()Ev
_ZN17interrupt_manager26handler_interrupt_request\num\()Ev:
  movb $\num + IRQ_BASE, (interruptnumber)
  jmp int_bottom
.endm

handle_interrupt_request 0x00
handle_interrupt_request 0x01

int_bottom:
  mov $0x10, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs

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

_ZN17interrupt_manager24ignore_interrupt_requestEv:

  iret

.data
  interruptnumber: .byte 0
