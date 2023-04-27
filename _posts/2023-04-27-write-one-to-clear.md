---
layout: post
title: Write One to Clear
---
<!-- excerpt start -->
Memory mapped registers do not always behave like regular memory. Here I
explain why you should avoid using bitfields when clearing W1C status bits.
<!-- excerpt end -->

## The bug
I encountered a bug in my code today related to register fields that have Write
One to Clear (W1C) characterics. Here's a simplified example. It was for an
embedded PowerPC target but I'll use x86 ISA since more people understand it.

Suppose you have an isr status register, which is part of the memory map of
some peripheral...

    struct Uart {
        union {
            volatile uint32_t R;
            struct {
              volatile uint32_t  :28;
              volatile uint32_t PE:1;    // Parity error
              volatile uint32_t FE:1;    // Frame error
              volatile uint32_t BO:1;    // Buffer overrun
              volatile uint32_t NF:1;    // Noise
            } B;
        } ISR;

        // ...
    };

    #define UART (*(volatile struct Uart *) 0xFFFF0000)

..All the flags of the ISR register are defined in the datasheet as W1C. I had
this code:

    int isr() {
        if (UART.ISR.B.PE) {
            UART.ISR.B.PE = 1;
            // ...
        }
    }

The problem is that the compiler lowers the bitfield access to a
read-modify-write sequence.

    isr:
            mov     eax, 0xFFFF_0000
            mov     rdx, [rax]
            test    edx, 0x1000_0000
            je      .out
            mov     edx, [rax]         ; read
            or      edx, 0x1000_0000   ; modify
            mov     [rax], edx         ; write
    .out:
            ret

So if several flags were set to 1, then the above code would have reset all
those flags since it would write back what had been read.

## The solution
With write one to clear register bits, you have to avoid the read. Assigning
the register to a shift expression gives the expected result.

    int isr2() {
        if (UART.ISR.B.PE) {
            UART.ISR.R = 1U << 28;
            // ...
        }
    }

This is lowered to an assignment of the expected 0x1000_0000 constant.

    isr2:
            movabs  rax, [0xFFFF_0000]
            test    eax, 0x1000_0000
            je      .out
            mov     eax, 0xFFFF_0000
            mov     [rax], 0x1000_0000  ; write a 1
    .out:
            ret

## Why use W1C for status bits?
By using W1C you protect against read-modify-write errors occuring on bits set
between reading the register and writing the value back. Because they are
written as zero, they are not cleared.

If you instead could write a 0 to clear a status bit then you'd risk clearing
flags that were set on other status bits if you did a write.

If you instead decided to clear status bits by reading from them you might get
unintentional side-effects when a debugger accesses the register.

