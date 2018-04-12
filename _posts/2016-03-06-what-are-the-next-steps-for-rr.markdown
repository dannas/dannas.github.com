---
layout: post
title: What are the next steps for rr?
---

Robert O'Callahan is [leaving Mozilla][leaving] to work on rr-related technology. I've been following the [rr project][rr] at a distance as a user and very casual contributor. I got curious, what will happen next? Here are my free-wheeling thoughts on possible directions.

The rr tool only runs on Linux; porting it to Mac [would be hard][mac] but might be doable; porting to Windows would require close cooperation with Microsoft, but maybe it's within reach as well. Being able to use it on programs that share memory with other non-recorded programs can't be done today but might be possible if some sort of kernel api for virtual memory write notifications were introduced into Linux. Adjusting the builtin scheduler to increase the chance of reproducing bugs is probably something that can give good returns. Running rr on ARM is not possible due to the semantics of their atomics instructions - perhaps other platforms might be doable - power8, mips?

All those questions above are interesting but I'd like to focus on something else: What would be the optimal debugger interface?  The debugging process for me boils down to: How can I find the causes of a problem and what are the reason that these causes do exist? Reverse execution greatly speeds up the process of connecting a symptom to a cause. But it does not automatically provide more context into why the system has reached this state.  It still demands that the programmer has a lot of knowledge about the program in their head to make the correct conclusions.  

To make my point: Imagine using rr as an instruction-level debugger with no knowledge of symbols whatsoever. It would be possible to record the execution of a program, and later run it forwards or backwards.  We could inspect the stack, registers and the value of the instruction pointer.  If we can keep all the state of our program in our head, then we can set hardware watchpoints on the appropriate addresses and find our root cause.

With symbols, it's much easier to understand call stacks and keep track of long lived state. I can explore the values of key data structures during the execution of the program; call functions to get deeper insights and modify variables to do experiments. But I'm still bound by gdb's notion of a program as an instruction stream; a set of registers and a stack. In many cases, I'm more interested in the messages passed between two objects; a trace of important interactions. And I'm also bound by gdb's "stop the world and peek" mode of operation. I want to see the interactions happen, not just look at them at a fixed point in time. It's like the insights you can gain by watching [visualizations of algorithms][algo_vis] versa just inspecting the code.

For example: When I'm learning a new code base I often run Google's pprof sampling profiler on an execution, to build a [map of often called components][pprof]. I then inspect those components by setting breakpoints when running the program again.  I get a sense of connections between components, but I don't get any visual help in understanding the execution in time. How can rr provide a better way to understand the execution of a program over time? Can it help me build that mental map of the execution? Perhaps the debugger can infer some properties of the program from just running it; lifetime of objects; major interactions between threads, clustering of calls shown as hot/cold call paths. If we store all events that has happened, then we could use as much time as we want (within reasonable limits of course) to compute more detailed information about the execution trace.  

What if you could open the debugger and click on object A and Object B and ask that all function calls from A to B and B to A should be shown to me on a time axis.  Or if you could mark instance variables a, b, c and say: Whenever any of these values changes I want to see a new state visualized on a time axis. What if we could do a bunch of tiny experiments like that?

So knowing where you are in the system, is sort of the key part I miss from debuggers today. Timeline visualizations would be one way of addressing it.  Some sort of map would be another. Perhaps a reachability analysis of a statement? What things are affected by this assignment? And seeing how different threads interact would be another very useful visualization to have.  The challenge lies in making such a system as malleable as the gdb textual interface. How can we see more context but still preserve the precision in our queries?

In order to understand the code better, you might need more information than what is present in the debug symbols for your language. What to do then? Gdb provides python bindings for formatting compound data types and printing frames. What other options are available for a debugger? Should the source code be annotated, like is done for [rtos tracing][rtos] or for dtrace and systemtap probes? How can key data structures be visualized like is done in DDD? What to do about JIT code?  

Debugging to me, consists of doing a bunch of experiments and evaluating the result. Could the debugger aid in that? Perhaps for each experiment, annotations could be added to the trace? If the traces became portable, then one developer could send his trace, with annotations included, to another developer to show what he had tried.

Can rr somehow minimize the source code involved in triggering a bug, a.k.a.  delta debugging? Can rr chunk a large trace into a smaller one; if we have a three hour recording where the bug is triggered after 2:55, can rr somehow create a trace file that has compressed all the previous events into something much smaller?

I guess, what I'm talking about in this post is some sort of overlap between tracers, logging, profilers and debuggers. The first two provides continuous reporting while the later two samples the execution. Breakpoints and watchpoints gives me a chance to see details. Having a stored execution trace enables us to create a database from which we can conduct even more precise queries about the state of the program. That may be useful, but that extra information can also be useful for creating more fuzzy contexts visualizations that help us understand a complex system.

[leaving]: http://robert.ocallahan.org/2016/03/leaving-mozilla.html
[rr]: http://rr-project.org/
[mac]: https://joneschrisg.wordpress.com/2015/01/29/rr-on-os-x-can-it-be-ported/
[algo_vis]: https://bost.ocks.org/mike/algorithms/
[rtos]: http://www.freertos.org/rtos-trace-macros.html
[pprof]: https://github.com/gperftools/gperftools/wiki
