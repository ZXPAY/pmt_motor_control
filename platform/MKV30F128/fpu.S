    .syntax unified
    .global enable_fpu
    .global f32_add
    .global f32_sub
    .global f32_div
    .global f32_mul
    .global f32_abs
    .section .text

@ Define Coprocessor Access Control Register
.equ CPACR, 0xE000ED88

enable_fpu:
    ldr r0, =CPACR
    ldr r1, [r0]
    orr r1, r1, #(0xF<<20)
    str r1, [r0]
    bx lr

f32_add:
    vadd.f32 s0, s0, s1
    bx lr

f32_sub:
    vsub.f32 s0, s0, s1
    bx lr

f32_div:
    vdiv.f32 s0, s0, s1
    bx lr

f32_mul:
    vmul.f32 s0, s0, s1
    bx lr

f32_abs:
    vabs.f32 s0, s0
    bx lr


