.set IRQ_BASE, 0x20
.section .text

.extern _ZN16InterruptManager15handleInterruptEhj
.global _ZN16InterruptManager22IgnoreInterruptRequestEv


.macro HandleExceptions num
.global _ZN16InterruptManager16HandleExceptions\num\()Ev

_ZN16InterruptManager16HandleExceptions\num\()Ev:
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm


.macro HandleInterruptRequest num
.global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev

_ZN16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptnumber)
    jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01

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
    call _ZN16InterruptManager15handleInterruptEhj
    movl %eax, %esp

    popl %eax
    popl %ebx
    popl %ecx
    popl %edx
    popl %esi
    popl %edi
    popl %ebp
    #popa
_ZN16InterruptManager22IgnoreInterruptRequestEv:
    iret


.data
    interruptnumber: .byte 0