---
layout: post
title: What I've been reading in May
---

<!-- excerpt start -->
Here are the articles, videos, and tools that I'm excited about this May.
<!-- excerpt end -->

The format of this entry is stolen from the [Interrupt MemFault blogs monthly roundup](https://interrupt.memfault.com/tags#roundup) posts. You should read those. They're way better than mine!

## Articles & Learning
* [A Survey of Recent iOS Kernel Exploits](https://googleprojectzero.blogspot.com/2020/06/a-survey-of-recent-ios-kernel-exploits.html) by Brandon Azad.<br/>
I've always heard that iOS is more secure than Linux. But even the sun has its spots! Here, 24 exploits are listed since iOS 10. There is also a list of kernel exploit mitigations including the usual ones: stack canaries, ASLR and W^X. But there's also Privileged access-never (PAN), Kernel Text Readonly Region (KTRR), Access Protection Rerouting (APRR), Page Protection Layer (PPL), Pointer Authentication Codes (PAC) and zone_require; all of which are explained in the article.
* [Embedded Application Security Best Practices](https://scriptingxss.gitbook.io/embedded-appsec-best-practices/) by OWASP<br/>
Still in its infancy but will hopefully mature to a useful resource on how to do secure firmware updates; harden C toolchains; use transport layer security and more. The section on threat modeling has a good description of threat enumeration and risk rating.
* [MMU gang wars: the TLB drive-by shootdown](https://bitcharmer.blogspot.com/2020/05/t_84.html) by Wojciech Kudla<br/>
I've known for a long time that reusing memory is better for performance than having the OS free it. But I haven't been able to motivate why. Until now! It has to do with TLBs. When a memory region is deallocated (munmap,sbrk) the TLB entries need to be flushed. Unlike for regular caches, there's no bus snooping, so other cores need to stop what they're doing while the TLB entries are invalidated. Wojciech goes into great detail about how the OS and hardware interact and does an experiment that confirms that threads not touching the memory being freed are slowed down during a TLB shootdown.
* [B-trees](https://shachaf.net/w/b-trees) by Shachaf<br/>
What's your desert island data structures? What would you use if you had only C and no libraries? I'd go for stretchy buffers and an open-address hash table. But there are times when you want to store and lookup items in-order. Shachaf gives a great intuitive explanation behind how to implement B-trees.
* [Robin Hood Hashing should be your default Hash Table Implementation](https://www.sebastiansylvan.com/post/robin-hood-hashing-should-be-your-default-hash-table-implementation/) by Sebastian Sylvain<br/>
A variant on open-addressing based hash tables. You can use higher load factors without affecting performance. Unlike other open addressing schemes, looking for non-existent elements is still fast. The clever trick: When you probe for a position to insert an element, if the probe length for the existing element is less than the current probe length for the element being inserted, swap the two elements and keep going.
* [Performance Speed Limits](https://travisdowns.github.io/blog/2019/06/11/speed-limits.html) by Travis Downs.
Reminds me of Intels Top-down Microarchitecture Analysis method, but with more "meat" to it. As usual, Travis's post is - as always - filled with lots of details. I especially liked the Out of Order section.


## Videos
* [Virtual Memory Introduction to Computer Architecture](https://www.youtube.com/watch?v=qcBIvnQt0Bw&list=PLiwt1iVUib9s2Uo5BeYmwkDFUh70fJPxX) by David Black-Schaffer<br/>
A 14 part video series that describes page tables, address translation, page faults, and memory protection. The best presentation of the subject that I've ever seen.
* [How NOT to Measure Latency](https://www.youtube.com/watch?v=lJ8ydIuPFeU&list=WL&index=23&t=0s) by Gile Tene<br/>
Clients will see way more latency than is predicted from looking at load averages. You need the nines and many of them!


## Neat Open Source Projects
* [Google/safeside](https://github.com/google/safeside)<br/>
Practical demonstrations of known side-channel attacks. References to research that describes what causes side-channel attacks. Ideas and prototypes for how to find and stop side-channel attacks. It's a real treasure-trove: The code is much clearer that what is attached to the Spectre/Meltdown/Foreshadow/MDS papers.
* [Glscopeclient](https://github.com/azonenberg/scopehal-cmake)<br/>
Andrew Zonenberg has created a remote oscilloscope utility capable of decoding USB, JTAG, Gigabit Ethernet, and does FFT and eye pattern analysis. Currently, LeCroy and Keysight scopes are supported but it looks like there's code for talking to Rigols and Siglents as well.
* [Per Vognsens benchmark.cpp gist](https://gist.github.com/pervognsen/b28a5ff0c9705269bb7f70d809f217ce)<br/>
Neat unit conversion and time estimation in a small amount of code.
* [Per Vognsens BTree.cpp gist](https://gist.github.com/pervognsen/e7883b3de183fcd601c1edf7f7e9508b)<br/>
One of the cleanest B-tree implementations I've seen. Not packaged to be reused but a great starting point for your implementation.

## Tools
* [Desmos](https://www.desmos.com/)<br/>
A graphing calculator that has an intuitive user interface that is easy to use both from a regular desktop computer as well as using a touch interface.
* [Pico TA102 Two-Footed Probe Positioner](https://www.tequipment.net/Pico/TA102/Passive-Oscilloscope-Probes/)<br/>
Probe your circuits without having to solder on wires or hold the oscilloscope probes in your hands!
* [PCBite](https://sensepeek.com/)<br/>
Probes for your DMM, logic analyzer and oscilloscope. A base plate in stainless steel with magnetic PCB holders and probes. The probes have a flexible metal arm and a strong magnet in the base. The weight of the head makes the compressible needle maintain constant pressure at the probing point.
* [Pfi](https://github.com/s7ephen/pfi)<br/>
The Matasano Port Forwarding Interceptor. When you want to sneak a peek at a TCP stream as it happens. Is Wireshark too clunky? Pfi can intercept, pause, modify, and resend TCP streams.
