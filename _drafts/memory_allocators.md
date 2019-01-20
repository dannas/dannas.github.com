---
layout: post
title: Memory Allocators
---

## The Cost of Memory Allocation

* Bruce Dawsons article [Hidden Costs of Memory Allocation](https://randomascii.wordpress.com/2014/12/10/hidden-costs-of-memory-allocation/)
* Arseny Kapoulkines article [A Queue of Page Faults](https://zeuxcg.org/2014/12/21/page-fault-queue/)
* Raymond Chens blogpost [A question about avoiding page faults the first time newly-allocated memory is accessed](https://blogs.msdn.microsoft.com/oldnewthing/20170512-00/?p=96146)

## Papers about memory allocation

* The OOPSLA paper by Berger, Zorn, McKinley [Reconsidering Custom Memory Allocation](http://www.allendowney.com/ss08/handouts/berger02reconsidering.pdf)
* Berger, Zoorn, McKinley [Composing High Performance Memory Allocators](https://people.cs.umass.edu/~emery/pubs/berger-pldi2001.pdf)
* 

## Kinds of allocators

Mariano Trebino has written [a taxonomy of different allocators](https://github.com/mtrebi/memory-allocators)

* Bump allocator or Linear allocator.
* Stack allocator
* Arena or Pool allocator. Many allocations, one free. Stores objects of the same size
* String interning

Christian Gyrlings article [Are We Out of Memory](http://www.swedishcoding.com/2008/08/31/are-we-out-of-memory/) presents a few allocators

Andrei Weissflogs article [Handles are the Better Pointers](https://floooh.github.io/2018/06/17/handles-vs-pointers.html)

Robert Nystrom describes pool allocators in [the Object Pool chapter in his Game Programming Pattern](http://gameprogrammingpatterns.com/object-pool.html) book.

https://nfrechette.github.io/2016/10/18/memory_allocators_toc/

Here's a blog series from Molecular Matters about memory allocators:

https://blog.molecular-matters.com/2012/08/14/memory-allocation-strategies-a-linear-allocator/

https://blog.molecular-matters.com/2012/08/27/memory-allocation-strategies-a-stack-like-lifo-allocator/

https://blog.molecular-matters.com/2012/09/17/memory-allocation-strategies-a-pool-allocator/

https://blog.molecular-matters.com/2012/10/02/memory-allocation-strategies-interlude-virtual-memory/

https://blog.molecular-matters.com/2013/01/29/memory-allocation-strategies-a-growing-stack-like-lifo-allocator/

## String Interning

[Bitwise episode 5: Review Changes to String Interning](https://bitwise.handmade.network/episode/bitwise/bitwise005/#2768)

[Bitwise episode 5: Descibe the memory arenas](https://bitwise.handmade.network/episode/bitwise/bitwise005/#3514)

## Allocators in C++

[John Lakos WG21 paper: On Quantifying Memory -Allocation Strategies](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0089r0.pdf)

[Youtube video: CppCon 2017: John Lakos “Local ('Arena') Memory Allocators (part 1 of 2)”](https://www.youtube.com/watch?v=nZNd5FjSquk)

[Youtube video: CppCon 2017: John Lakos “Local ('Arena') Memory Allocators (part 2 of 2)](https://www.youtube.com/watch?v=CFzuFNSpycI)

[Howard Hinnants stack_alloc](https://howardhinnant.github.io/stack_alloc.html)

