.macro DEFINE_OS_FUNC name, addr
    .text
    .balign 4
    .global _\name
    .type _\name, @function
_\name:
    mov.l .addr_\name, r1
    jmp @r1
    nop
    .balign 4
.addr_\name:
    .long \addr
    .size _\name, .-_\name
.endm
