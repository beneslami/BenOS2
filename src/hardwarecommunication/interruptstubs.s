.set IRQ_BASE, 0x20
.section .text

.extern _ZN5BenOS21hardwarecommunication16InterruptManager15handleInterruptEhj
.global _ZN5BenOS21hardwarecommunication16InterruptManager22IgnoreInterruptRequestEv


.macro HandleExceptions num
.global _ZN5BenOS21hardwarecommunication16InterruptManager16HandleExceptions\num\()Ev

_ZN5BenOS21hardwarecommunication16InterruptManager16HandleExceptions\num\()Ev:
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm


.macro HandleInterruptRequest num
.global _ZN5BenOS21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev

_ZN5BenOS21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptnumber)
    pushl $0
    jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x0C

int_bottom:
    #pusha
    pushl %ebp
    pushl %edi
    pushl %esi
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax
    pushl %esp
    push (interruptnumber)
    call _ZN5BenOS21hardwarecommunication16InterruptManager15handleInterruptEhj
    movl %eax, %esp

    popl %eax
    popl %ebx
    popl %ecx
    popl %edx
    popl %esi
    popl %edi
    popl %ebp
    #popa

    add $4, %esp
_ZN5BenOS21hardwarecommunication16InterruptManager22IgnoreInterruptRequestEv:
    iret


.data
    interruptnumber: .byte 0