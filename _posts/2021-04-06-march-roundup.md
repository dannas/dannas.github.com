---
layout: post
title: What I've been reading in March
---

<!-- excerpt start -->
Here are the articles, videos, and tools that I'm excited about this August.
<!-- excerpt end -->

The format of this entry is stolen from the [Interrupt MemFault blogs monthly roundup](https://interrupt.memfault.com/tags#roundup) posts. You should read those. They're way better than mine!

## Articles & Learning

* [What is too long](https://hackernoon.com/what-is-too-long-4e15418f10ce) by Terry Crowley<br/>
  Terry starts off with a Leslie Lamport quote where he says that "[...] Without physical time, there is no way o distinguish a failed process from one which is just pausing between events".  Terry notes that if you send a message and don't receive a response, then it's a failure. But how long do we have to wait before calling it a failure? Terry doesn't really answer the question but gives some insight onto how to design APIs that give the right amount of acknowledgment of progress to allow the sender to better judge when it's waited "too long".
  
* [Timeouts and cancellation for humans](https://vorpus.org/blog/timeouts-and-cancellation-for-humans/) by Nathaniel J. Smith<br/>
  A request can succeed, fail, or hang forever. For the last one you need timeouts. You need a timeout for every component that interacts with another program, system or person. Nathaniel points out that passing timeout parameters between layers is a problem since we can't be sure whether the timeout to the lower layer will be for one I/O operation or for the whole operation. Using deadlines, e.g. fixed point in time, solves that problem but introduces some burden on the programmer. Nathaniel's solution is to use cancellation tokens to encapsulate timeouts and deadlines. Then he discusses how proper cancellation requires that every single I/O operation supports cancellation and how both Go and C# has problems.
  
* [Insights from the physical reality of computing](https://terrycrowley.medium.com/insights-from-the-physical-reality-of-computing-153c3c4e8375) by Terry Crowley<br/>
  Computing in the end comes down to physical reality: memory, computation that reads and writes that memory and communication between physical computing nodes. Terry argues that every system makes trade offs between moving computation closer to memory or visa versa. Changes in latency and bandwidth affects how systems are designed. Every system is asynchronous and how communication fails is what drives most designs. Terry argues that you should strive to optimize your system for predictable short latency since it allows you to retry more aggressively. Systems are optimized by chunking and batching.  This may give a component hard to understand properties regarding queuing and congestion: It leads to hidden characteristics that can be hard to understand for a large team.
  
* [Synchrony is a myth](https://medium.com/hackernoon/synchrony-is-a-myth-708acaf479f) by Terry Crowley<br/>
  An article on the same theme as the one above: Nothing happens instantaneously. Everything involves some delay, consumes some time. Exposing the asynchrony places more overhead on the programmer, but also allows for customizing the costs and trade offs to the problem domain rather than having the system balance those costs. 
  
* [Time is so everything doesn't happen at once](https://medium.com/hackernoon/time-is-so-everything-doesnt-happen-at-once-f19ad58314fb) by Terry Crowley<br/>

  For asynchronous systems you need queuing and throttling. Terry shares some advice on how to reason about asynchronous designs related to HTTP servers.

* [How TCP sockets work](https://eklitzke.org/how-tcp-sockets-work) by Evan Klitzke<br/>
  A high-level explanation of how Linux TCP/IP stack works, zooming in on listen queue overflows. How do the kernel avoid too much queuing of incoming connections? It's better to disallow connections, thereby throttling the service. Keep in mind that the application won't see rejected connections. You need a way to monitor listen queue overflows. Through /proc/net/netstat you get a system-wide `ListenOverflows` counter but it's an open question how to monitor for overflows per service.

* [Tips for using the sockets API](https://rigtorp.se/sockets/) by Erik Rigtorp<br/>
  A few common pitfalls like dealing with Nagles algorithm; SIGPIPE signals; avoiding blocking hostname resolution. Erik also describes how to asynchronously do TCP connects and how to asynchronously accepting connecti

## Neat Open-Source Projects

* [ORBTrace](https://github.com/orbcode/orbtrace)<br/>
  The gateware for the orbtrace debug tool, an open-source trace probe that hopefully will have the same capabilities as Segger J-Link.
  
* [orbuculum](https://github.com/orbcode/orbuculum)<br/>
  A set of tools for decoding and presenting output flows from the debug pins of a cortex-m MCU.
  
* [cobalt](https://github.com/oxidecomputer/cobalt)<br/>
  A collection of bluespec interfaces and modules from the Oxide computer company.
  
* [bluespec](https://github.com/B-Lang-org/bsc)<br/>
  A compiler, simulator and tools for the Bluespec Hardware Decription Language.
  
  