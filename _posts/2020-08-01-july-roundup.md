---
layout: post
title: What I've been reading in July
---

<!-- excerpt start -->
Here are the articles, videos, and tools that I'm excited about this July.
<!-- excerpt end -->

The format of this entry is stolen from the [Interrupt MemFault blogs monthly roundup](https://interrupt.memfault.com/tags#roundup) posts. You should read those. They're way better than mine!

## Articles & Learning
* [Flatter wait-free hazard pointers](https://pvk.ca/Blog/2020/07/07/flatter-wait-free-hazard-pointers/) by Paul Khuong<br/>
Paul provides references to some of the foundational papers for lock-free memory reclamation and then goes on to implement a wait-free alternative to the usual lock-free hazard pointer mechanism.
* [Nonblocking Algorithms and Scalable Multicore Programming](https://queue.acm.org/detail.cfm?id=2492433) by Samy Al Bahra<br/>
A thorough introduction to lock-free programming. Samy describes the system from bottom-up (cache-coherency protocols; memory models; and cost of atomic instructions due to topology). He then lists the three sets of progress guarantees for nonblocking synchronization: Obstruction Freedom; Lock Freedom; Wait Freedom. There is a total orderering to these classes of algorithms such that any wait-free algorithm is also lock-free and obstruction-free; any lock-free algorithm is also obstruction-free. Finally he introduces the classes of safe memory reclamation techniques that are used for lock-free data structures that operation on dynamically allocated memory.
* [What is Memory Safety](http://www.pl-enthusiast.net/2014/07/21/memory-safety/) by Michael Hicks<br/>
Buffer overflows; null pointer dereferences; use-after-free; and use of uninitialized memory are all examples of things that are not allowed in a memory safe program. But how define memory safety more formally? Michael's definition is based on the notion of defined/undefined memory and the use of pointers as capabilities. This definition is the same as what is used for Intels Memory Protection Extension (MPX).
* [I/O Is Faster Than the CPU – Let’s Partition Resources and Eliminate (Most) OS Abstractions](https://penberg.org/parakernel-hotos19.pdf) by Pekka Enberg<br/>I/O is getting faster in servers that have programmable NICs and non-volatile main memory. Let's eliminate most OS abstractions and use a Parakernel instead. E.g. sockets are too heavyweight for high-speed networks - steer packets to per-process memory buffers and implement the network stack in user space instead. Blocking OS interfaces require kernel threads whose context switches are slow - use async interfaces instead. Skip the virtual memory page cache and let the processes manage data caching.
* [Ptrace Notes](https://shachaf.net/tmp/ptrace-notes.txt) by Shachaf<br/>
A summary of how the ptrace system call works. Goes into detail about PTRACE_SEIZE; the different stop states; and how to restart the tracee.
* [The Definitive Guide to Module Bias and How to Avoid It](https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/) by Yolan Romailler<br/>
A description of know vulnerabilities in cryptography code caused by module bias and how to avoid the problem using rejection sampling and modulo reduction. I knew about rejection sampling, but I wasn't aware of the tricks for optimizing performance mentioned in this article: Mask the random value to contain the amount of bits requested. And if the modulo is a divider of the max random value you can get, then modulo reduction will be fine.
* [Sandboxing and Workload Isolation](https://fly.io/blog/sandboxing-and-workload-isolation/) by Thomas Ptacek<br/>
How do you prevent a vulnerability in one component from compromising every other part? Thomas lists a number of different techniques. Chroot is the original sandboxing technique but you still have access to ptrace, procfs, device nodes and the network. Privilege Separation a'la qmail where you  use different processes where you make the stuff that needs to be privileged as small as possible. The original containers, which is chroot extended to other kernel identifiers - process IDs, user IDs, network interfaces. But you still have access to /sys, can reboot the system and load new kernel modules. The next stage was adding mandatory access control, system call filtering and capabilities to the containers. This modern Docker and Googles nsjail. Containers are large so you might try language runtimes like V8 or Fastly's Lucet serverside WASM framework instead. But with these techniques you still rely on the Linux Kernel to be safe, and it's often not due to the limitations of being written in C and being designed as a monolith. So Google wrote gVisor which emulates the system calls in userspace using a safe subset. Thomas continues to explain why lightweight virtualization was preferable for their workloads. An epic read that clarifies a lot of misconceptions I had.

## Videos
* [Performance Engineering of Software Systems](https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-172-performance-engineering-of-software-systems-fall-2018/) by Charles Leiserson and Julian Shun<br/>
  A set of MIT Open Course Ware (OCW) lectures and course materials from Fall 2018. Previously only the set from 2010 were available. They describe in great detail analysis of multi-threaded algorithms; storage allocation; caching and cache-efficient algorithms; synchronization without locks; and much more. And the introduction lectures is based around Jon Bentleys rules of optimization. You can't go wrong when you base your course around Jon Bentley! A true gold mine!
* [COMP9242 seL4 Lectures](https://www.youtube.com/playlist?list=PLbSaCpDlfd6qLbEsKquVo3--0gwYBmrUV) by Gernot Heiser<br/>
  A set of lectures that introduces microkernels and seL4 in particular. Topics includes : performance analysis; design and implementation; and formal verification. The SeL4 system is truly different from a regular monolithic kernel: SeL4 has ~3 system calls,  just ~10 kSLOC and all memory management is done by a user-level library! Back in 2009 a proof of correctness via formal verification was published. And it uses true capabilities fro fine-grained access control. In a micro-kernel it's crucial that IPC performance is fast enough, seL4 takes 0.1us to do an IPC transfer.

## Neat Open Source Projects
* [Seastar](http://seastar.io/)<br/>
  A C++ framework for high-performance server applications on modern hardware. It has a shared-nothing design; can use DPK for fast user-space networking; uses futures and promises extensively for avoiding blocking calls; and uses message passing between CPU cores in order to avoid time-consuming locking.
* [Manticore OS](https://manticoreos.io/)<br/>
A clean-slate research operating system written in the Rust programming language with the aim of exploring the Parakernel OS architecture. Very much a work in progress but an interesting one: Move OS abstractions to user-space and partition the kernel such that a a process has direct access to NIC buffers in order to avoid the cost of context switching during blocking operations. It's exploring the ideas from the Seastar framework in kernel-space.

## Tools
* [Pomona 2708](https://www.digikey.com/product-detail/en/pomona-electronics/2708/501-1114-ND/604328)<br/>
Holder for test lead wires. If you have around 20 cable packs like me, you sure want one of these.
* [vgrep](https://github.com/vrothberg/vgrep)<br/>
A pager for  grep, git-grep, ripgrep and similar implementations, that allows for opening the indexed file locations in a user-specified editor such as vim or emacs.
* [scc](https://github.com/boyter/scc)<br/>
A tool similar to cloc for counting physical lines of code, blink lines, comment lines and physical lines of source code in many programming languages.  Besides counting lines of code it also performs COCOMO calculations like slocccount and estimates code complexity like cyclomatic complexity calculators.

