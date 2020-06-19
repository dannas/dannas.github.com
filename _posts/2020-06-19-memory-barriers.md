---
title: Paper&#58; Memory Barriers, A hardware View for Software Hackers
layout: post
---
<!-- excerpt start -->
In this paper, I review Paul McKenney's memory barrier paper. He describes
in 28 pages why memory barriers exist and how they are implemented on
different platforms. 
<!-- excerpt end -->

[McKenney, Paul E. Memory Barriers: A Hardware View for Software Hackers. 2010.](http://codelabs.ru/reading/whymb.2010.07.23a.pdf)


Why do we need memory barriers? Because processors may reorder memory
operations. Why do processors reorder memory accesses? To improve performance.
How does that reordering improve performance? The processor doesn't have to 
wait as much as it would have to do if every operation happened exactly in 
program order. What exactly are those memory operations? Which operation in
particular do we not have to wait to complete? Do all processors behave the
same?

There's a lot to unpack here. First, the processor can read and write to memory.
There's a >100x difference in latency between the CPU and RAM. So for every load
or store, the processor would have to wait for several hundred cycles without
making progress.

If every load and store were truly unique and random, then there wouldn't be
much hardware engineers could do about this latency mismatch. But a program
often accesses recent addresses again (temporal locality) and it often
accesses nearby addresses (spatial locality).

There are faster memories, but they can't hold as much data. They fit our use-
case perfectly: Use a smaller cache that stores recently accessed data. If it
takes, say 10 cycles to load data from the cache. Then we've shaved off 90
cycles from the RAM access. Great! Problem solved.

But what if we want to store something? Then we're back to 100 cycles again.
Unless... We just store the data to the cache, but we won't forward it until
it's necessary. Then we still have to use 100 cycles but for many cases, we
can postpone those stores and batch them together. That's a write-back cache.

One complication with our cache idea is that we need to keep track of which
memory address is cached. The address is 8 bytes on a 64-bits platform and a
word is 8 bytes. That seems kind of wasteful. If we instead write and read a 
larger chunk, we won't need to use fewer bits for the address. Having caches
operate on these larger chunks (which are called cache-lines) simplifies things.

But if the cache operates on cache-lines and we want to just write one byte?
Then the CPU would have to request the cache-line from memory, write the byte and
then send the cache-line back to memory.

But what if you want to have multiple cores? How do you synchronize the
accesses? You could put in a lock: now only CPU 0 is allowed to access the 
memory and next only CPU 1 can store or load. That takes a lot of time if we 
have many cores.

## Store Buffers
Stores result in unnecessary stalls. A write by a CPU triggers an invalidate
request which has to wait for the acknowledge before the CPU can consider
the instruction committed. But the CPU will overwrite whatever is returned
from the other CPUs, so why wait?

Instead of waiting, the CPU can record its write in a store buffer and
continue with the next instruction. When the cache-line arrives, the data will
be moved from the store buffer to the cache-line

McKenney points out that there are two complications when adding store
buffers. The first involves self-consistency and can be illustrated with this
very short code sequence:

```
a = 1;
b = a + 1;
assert(b == 2);
```

If CPU 0 puts the write on line 1 in its store buffer and then retrieve an old
value of `a` from another CPU, we will get the wrong result. The problem is
that there are two versions of `a`: one in the store buffer and another one in
the cache. Store forwarding solves that problem: each CPU snoops its store
buffers and cache when performing loads.

The second complication can arise from executing `foo` on CPU 0 and `bar` on 
CPU 1.

```
void foo(void) {
    a = 1;
    b = 1;
}

void bar(void) {
    while (b == 0) continue;
    assert(a == 1);
}
```
If `a` is not in CPU 0's cache but `b` is, then we may have a sequence of MESI
messages for which `b == 1` and `a == 0`. The problem is that CPU 0's write to
`a` is not guaranteed to be visible before `b = 1` executes. A memory barrier
will fix that:

```
void foo(void) {
    a = 1;
    smp_mb();
    b = 1;
}
```

## Invalidate Queues
Store buffers decrease the latency of retiring a store, but the time until
commit is unchanged; the store buffer still have to wait for the acknowledge
to arrive from the other CPUs caches.

We can speed up that time though if we add an invalidate queue to each cache.
When an invalidate request arrives, an acknowledge is sent immediately and the
actual invalidation is queued in an invalidation queue.

But the fact that the processing is delayed introduces another opportunity for
memory-mis-ordering. Consider this example where CPU 0 executes `foo` and CPU 1
executes `bar`:

```
void foo(void) {
    a = 1;
    smp_mb();
    b = 1;
}

void bar(void) {
    while (b == 0) continue;
    assert(a == 1);
}
```

If the invalidate requests for `b = 1` are placed in CPU 1's invalidation queue,
then there's the risk that CPU 1 will read stale values of b. This can be fixed
by placing a memory barrier inside bar:

```
void bar(void) {
    while (b == 0) continue;
    smp_mb();
    assert(a == 1);
}
```

## Read and Write Barriers
A full memory barrier acts on both the store buffer and the invalidation queues.
But in our example above `foo` only needs a barrier for the store buffer and
`bar` only needs a barrier for the invalidation queues. We can use write and
read memory barriers instead.

```
void foo(void) {
    a = 1;
    smp_wb();
    b = 1;
}

void bar(void) {
    while (b == 0) continue;
    smp_rb();
    assert(a == 1);
}
```

## Memory-Barrier Instructions for Specific CPUs
This section was a bit hard for me to interpret. I mixed up what ordering
guarantees the different architectures provided and the barrier instructions. To
me, it was clearer to use Jeff Phreshings [categorization of the possible memory
reorderings][preshing-mem]:
* #StoreStore
* #StoreLoad
* #LoadLoad
* #LoadStore

[preshing-mem]:(https://preshing.com/20120710/memory-barriers-are-like-source-control-operations/)

**X86-64** provides a strong memory model. Only #StoreLoad reorderings can occur. A
trailing `mfence` on stores or a `lock xchg` can be used for providing
sequential consistency[^petercordes-sfence].

**ARMv7** has a weak memory model. All four reorderings can occur. Three types
of memory-barriers exists: `DMB` (data memory barrier), `DSB` (data 
synchronization barrier) and `ISB` (instruction synchronization barrier).

---

[^petercordes-sfence]: Peter Cordes writes in [Does the Intel Memory model make SFENCE and LFENCE redundant?](https://stackoverflow.com/questions/32705169/does-the-intel-memory-model-make-sfence-and-lfence-redundant)
    X86-64 provides `SFENCE` and `LFENCE` but those are not useful in normal
    code because x86's acquire/release semantics for regular stores makes them
    redundant unless you are using special instructions or memory types.
    `SFENCE` is only relevant when using `movnt` (Non-Temporal) streaming stores
    or with `clflushopt`.

    The primary use-case for `LFENCE` is not memory ordering at all, it's for
    serializing instruction execution, e.g. for Spectre mitigation or with
    RDTSC.