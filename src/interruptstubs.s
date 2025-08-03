.set IRQ_BASE, 0x20

.section .text

.extern _ZN2os10interrupts16InterruptManager15handleInterruptEjj

.macro HandleException num
.global _ZN2os10interrupts26InterruptManager16handleException\num\()Ev
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm


.macro HandleInterruptRequest num
.global _ZN2os10interrupts26InterruptManager26handleInterruptRequest\num\()Ev
    movb $\num + IRQ_BASE, (interruptnumber)
    jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01


int_bottom:

    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp
    push (interruptnumber)
    call _ZN2os10interrupts16InterruptManager15handleInterruptEjj
    # addl $5, %esp
    movl %eax, %esp

    popl %ds
    popl %es
    popl %fs
    popl %gs
    popa

    iret

.data
    interruptnumber: byte 0
