---
layout: post
title: What I've been reading in December
---

<!-- excerpt start -->
Here are the articles, videos, and tools that I'm excited about this August.
<!-- excerpt end -->

The format of this entry is stolen from the [Interrupt MemFault blogs monthly roundup](https://interrupt.memfault.com/tags#roundup) posts. You should read those. They're way better than mine!

## Articles & Learning

* [Git under the hood](https://articles.foletta.org/post/git-under-the-hood/) by Greg Foletta<br/>

  There are lots of articles describing gits internals. This one is by far the most readable I've encountered. The fundamental git objects are described: A blob contains file content; a tree lists filenames and associated blobs; and a commit points to a specific tree. They're presented using regular command-line utilities.

* [Write yourself a git](https://wyag.thb.lt/) by Thibault Polge<br/>
  Write a git clone using python. The tutorial walks through how to implement 15 of the plumbing and porcelain commands.

* [What is git made of](https://zserge.com/posts/git/) by Serge Zaitsev<br/>
  
  Similar to Greg Folettas blog post. Serge writes a minimal git client in 300 lines of Go code called nanogit. Serge has written several posts where he implements some interesting system in few lines of code. Like...
  
* [Partcl - a tiny command language](https://zserge.com/posts/tcl-interpreter/) by Serge Zaitsev<br/>
  
  .. This article where he implements a simple TCL interpreter in about 700 lines of C. 
  
* [Linux containers in a few lines of code](https://zserge.com/posts/containers/) by Serge Zaitsev<br/>
  
  Serges blog is truly a goldmine. Here he writes code that is able to run a common busybox Docker image without using Docker.
  
* [Parse, don't validate](https://lexi-lambda.github.io/blog/2019/11/05/parse-don-t-validate/) by Alexis King<br/>

  Similar theme to John Ousterhouts principle "define errors out of existence". If we read input and parse it into another representation, then the rest of the program can assume that the input is valid and there's no need for additional validation. If we don't have an explicit parse step the program ends applying "shotgun parsing" where parsing and input-validating code is mixed with and spread across processing code - throwing a cloud of checks at the input, and hoping, without any systematic justification, that one or another would catch all the "bad" cases.
  
* [Custom allocators demystified](https://slembcke.github.io/Custom-Allocators) by Scott Lembcke<br/>

  Describes slab allocators, bump allocators, arena allocators and buddy block allocators with pretty pictures. I don't think there are reasons where you want to write your own buddy block allocator, but the other ones I can totally see the use cases for.

* [Server design](https://web.archive.org/web/20210512030926/http://pl.atyp.us/pages/server-design.html) by Jeff Darcy<br/>

  Jeff describes how he goes about designing "high-performance request-handling programs". It is centered around his four horsemen of poor performance: Data copies; Context switches; Memory allocation; and lock contention. What he describes is the thread-per-core architecture but long before those got popular.

* [Different I/O access methods for Linux, what we choose for ScyllaDB and why](https://www.scylladb.com/2017/10/05/io-access-methods-scylla/) by Avi Kivity<br/>

  Avi discusses the tradeoffs between traditional I/O, mmap, direct I/O and asynchronous I/O with regards to cache control; copying and MMU activity; I/O scheduling; and thread scheduling. 
  
* [How much testing is enough](https://testing.googleblog.com/2021/06/how-much-testing-is-enough.html) by George Pirocanac<br/>

  When is something done? What are the quality requirements? George starts of by pointing out that whatever level of testing you're currently doing it's a good idea to document what you're actually doing. It's hard to improve a process if you don't know what the process is! Then he goes on emphasing Googles test pyramid: that you have  a base of unit tests; somewhat fewer integration tests; and even fewer end-to-end tests. But the most important thing is to have a process where you let failures and outages dictate where you direct your next testing efforts.

* [Your first step to game development starts here](https://www.gamedev.net/articles/programming/general-and-gameplay-programming/your-first-step-to-game-development-starts-here-r2976/) by "Alpha_ProgDes"<br/>

  Lists ten classical arcade games from pong to super mario bros in increasing order of implementation difficulty. Many people who wants to write a game get lost and don't know where to begin (I've seen that happen to my kids). This is a great list. So far we've done pong, worm, breakout and Space invaders in the Scratch environment.


## Neat Open-Source Projects

* [nanoBench](https://github.com/andreas-abel/nanoBench)<br/>

  Evaluate microbenchmarks using hardware performance counters. It's the tool used for running the microbenchmarks for obtaining the latency, throughput, and port usage date that is available on [uops.info](uops.info).

# Tools

* [Fast edge square wave generator](https://community.element14.com/technologies/open-source-hardware/b/blog/posts/building-a-fast-edge-square-wave-generator)<br/>

  Instructions on how to build  a simple circuit that can be used for checking test equipment. It consists of an oscillator and some 74AC logic gates and can achieve a rise time of 550 picoseconds.

* [Solderable in-circuit oscilloscope probes](https://community.element14.com/technologies/test-and-measurement/b/blog/posts/building-solderable-in-circuit-oscilloscope-probes)<br/>

  Some coax cable and a resistor is all you need to make an resistive probe that outperforms the regular 10x passive probe by far!