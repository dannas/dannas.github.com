---
layout: post
title: Week 2 Making Embedded Systems
---

<!-- excerpt start -->
Week 2 of 10 of the Making Embedded Systems course.
<!-- excerpt end -->

This weeks schedule was smashed to pieces by a Covid-infection for me. I've had a fever for five days and that has really hampered my progress. 

We've done some data-sheet reading exercises and have written hardware block diagrams for different development boards. I head really hoped to do lots of experiments with the STM32 HAL, but that will have to wait until next week. Such is life.

## Learning computer micro architecture

In the course forum someone asked about resources for learning about computer organization. Here's my reply:

"**Computer organization**" compasses a lot. I guess first thing is that for this course you won't need any deeper insights into what the CPU does. You can treat it as a black box.

But if you want to venture into this area out of curiosity:

If you have 1 hour: To say that a computer "computes" is not telling the whole truth. Most of its time is spent moving data around. Richard Feynman describes a computer as a file clerk that, as he gets increasingly dumber, can perform his tasks faster and faster. It's a great lecture ([video](https://www.youtube.com/watch?v=EKWGGDXe5MA)). He uses the same analogy in his [book](https://www.amazon.com/Feynman-Lectures-Computation-Frontiers-Physics/dp/0738202967) . The four first chapters are a great introduction to computer architecture.

If you have 10 hours and want something more practical: Ben Eater has published a [youtube series where he builds an 8-bit processor and another one where he builds a 6502](https://eater.net/6502). Check out his youtube account. He has great presentations of the SPI protocol, UART, I2C, USB and many other videos highly related to this course.

If you have 20 hours and like me prefer books: Charles Petzold’s book [Code](https://codehiddenlanguage.com/) - the hidden language of computer hardware and software is a great and entertaining introduction to computers. Be sure to check out his javascripts gadgets that explains logic gates, adders, sequential chips, assembly and more on 

If you have another 80 hours: [The Elements of Computing Systems](https://www.amazon.com/Elements-Computing-Systems-second-Principles/dp/0262539802) let's you build a computer using simulators, starting with vhdl and ending up with an OS. I've read the book and done the exercises. It's great, really great! They have an online course as well: https://www.nand2tetris.org/

What you learn from the above material is how a simple processor operates and the principles behind it. If you have another 20 hours and you want to know what makes processors go faster (caches, superscalar execution, pipelines, out-of-order scheduling), I recommend Jon Stokes[ Inside the Machine: An Illustrated Introduction to Microprocessors and Computer Architecture](https://www.amazon.com/Inside-Machine-Introduction-Microprocessors-Architecture/dp/1593276680). It's an easy read that covers a ton of things. If you can't spare 20 hours then this 90 minute introduction might be a good read: [Modern Microprocessors](https://www.lighterra.com/papers/modernmicroprocessors/).

## Adding a shell for board bringup

This week we talked about adding a shell early on in the development process since it makes it much easier to do small experiments with short feedback loops. I used the [hydrabus fork of tokenline](https://github.com/hydrabus/tokenline/tree/master) and manged to get the demo-shell running after doing these steps:

* Src/syscalls contains the function definitions required by newlib.
 There are weak definitions for `__io_putchar` and `__io_getchar` in there.
* I added definitions of  `__io_putchar` and `__io_getchar` in my main.c
* Now I could see the prompt in my serial emulator (tio) but I couldn't write anything.
* Added a `setvbuf(stdin, NULL, _IONBF, 1)` to disallow input buffering and then I could receive data. 
* Had a problem with the CR-NL mapping in tio which was solved by passing some stty-flags: `tio -b 115200 -m ONLCRNL,INLCRNL /dev/ttyACM0`

The STMCubeIDE uses the nano-spec by default. There are options for enabling floating point scanf and printf parameters. I enabled those in the project settings.

My NUCLEO_F401RE board has USART2 hooked up to the on-board ST-Link which exposes it over USB. That took me a while to figure out. I need to read the user manuals more thoroughly! 

Next steps is adding a logging component and hooking up shell command for testing i2c, spi and uart.
