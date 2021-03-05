---
layout: post
title: What I've been reading in August
---

<!-- excerpt start -->
Here are the articles, videos, and tools that I'm excited about this August.
<!-- excerpt end -->

The format of this entry is stolen from the [Interrupt MemFault blogs monthly roundup](https://interrupt.memfault.com/tags#roundup) posts. You should read those. They're way better than mine!

## Articles & Learning
* [How a Kalman Filter Works in Pictures](http://www.bzarg.com/p/hohow-a-kalman-filter-works-in-pictures/) by Tim Babb<br/>
  Instead of some math-ridden scary explanation read this article and get an intuitive understanding. You have uncertain information about some dynamic system and you want to make an educated guess about what to do next? You want a Kalman filter!
* [API Design](https://gist.github.com/vurtun/192cac1f1818417d7b4067d60e4fe921) by Micha Mettke<br/>
  Describes API design guided by the axis of granularity, redundancy, coupling, retention and flow control.
* [API, a Brief Comment](https://gist.github.com/pervognsen/d57cdc165e79a21637fe5a721375afba) by Per Vognsen<br/>
  How to create an iterator API as a replacement for callbacks when fine interleaving between caller and callee is required.
* [Coroutines and Fibers - Why and When](https://medium.com/software-development-2/coroutines-and-fibers-why-and-when-5798f08464fd) by Mark Papadakis<br/>
  Context switching is expensive. Lock contention and sharing is bad. Mark argues that coroutines solves these problems and that it's superior to task chaining (futures and promises). One advantage is that you don't need heap allocations for coroutines.
* [Coroutine Thoery](https://lewissbaker.github.io/2017/09/25/coroutine-theory) by Lewis Baker<br/>
  Coroutines are a generalisation of a function that has three additional operations - suspend, resume and destroy in addition to the call and return operations provided by normal functions. Lewis says that since coroutines can be suspended without being destroying the activation frame, the activation frame cannot in general be allocated using a stack data-structure and so may need to be stored on the heap instead. With a sufficiently smart compiler most lifetimes can be inferred and then the activation frame can live on the stack.
* [C++ Futures at Instagram](https://instagram-engineering.com/c-futures-at-instagram-9628ff634f49) by Instagram Engineering<br/>
  Describes how they scaled two recommendation services using futures. The first version used synchronous I/O and offloaded work to threads. The kernel scheduling became a bottleneck at around 400 threads and all the threads causes a large memory footprint. They then rewrote the code to use non-blocking I/O and an event loop which increased throughput. But the asynchronous calls lead to the "callback pyramid" (callback hell). With future chaining they ended up with code that was much easier to read.
* [What Color is Your Function](http://journal.stuffwithstuff.com/2015/02/01/what-color-is-your-function/) by Bob Nystrom<br/>
  Bob talks about a made up programming language that provides red and blue functions and how red functions can only be called from another red function. It's an allegory: The red functions are asynchronous ones. Since async functions don't return values and use different error handling they don't compose and you're left with callback hell. He describes how futures allows some composition but you still can't use them in many control flow statements and error handling is still weird. He goes on to describe how the await keyword solves some problems but not all. He's saying that you ultimately end up taking your asynchronous function and smearing it out into a bunch of closures that live over in the heap. Bobs solution? Use threads, green- or OS-level. With threads or fibers you can just suspend and hop straight back to the OS or the event loop without having to return from all those functions.
* [Zero-cost Futures in Rust](https://aturon.github.io/blog/2016/08/11/futures/) by Aaron Turon<br/>
  Do futures and async await really need to rely on heap-allocated closures with the associated overhead? Or do we need the unlimited stack growth of coroutines? Aaron argues that futures can be compiled down into state machines and callbacks giving us ergonomic APIs with zero-cost abstractions. The article ends with a benchmark of a "hello world" HTTP server. The rust implementation beated the Java, Go and Node implementations by a lot.

## Videos
* [Designing and Evaluating Reusable Components](https://caseymuratori.com/blog_0024) by Casey Muratori<br/>
  THE reference on the Internet about API design. You must watch this (even though the quality of the recording is poor).

## Neat Open Source Projects
* [Folly fibers](https://github.com/facebook/folly/tree/master/folly/fibers)<br/>
  An async C++ framework, which uses fibers for parallelism. Includes a task manager.
* [Folly futures](https://github.com/facebook/folly/blob/master/folly/docs/Futures.md)<br/>
  Another async framework from Facebook. Uses futures and promises for task chaining. It provides some compositional building blocks (`thenValue` and `thenTry`) which are missing from the C++ standard.
* [Seastar futures](http://seastar.io/futures-promises/)<br/>
  Yet another futures library. Contrary to the the future implementation in the C++ standard and Boost, Seastar futures require no locking; allocate no memory; and supports continuations.
* [libcoro](https://github.com/clibs/coro)<br/>
  Mark Lehmanns C implementation consists of 2 files offering a very simple 3 functions long API.
* [libtask](https://swtch.com/libtask/)<br/>
  Russ Coxs simple coroutine library.  In addition to coroutine calls it provides non-blocking I/O for files and sockets
