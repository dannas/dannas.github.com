---
layout: post
title: Flush+Reload
---

Recent exploits like Spectre, Meltdown, Foreshadow and MDS have all used cache
timing techniques for leaking data. There are several cache timing techniques
like Evict+Probe, Prime+Probe and Flush+Flush. The idea is that the cache
memory is a shared resource. The content that one process writes is only
supposed to be readable by that process, but since the cache is not flushed
during a context switch, it can still be read via indirect means.

## Preliminaries

I'm going to use a desktop system as basis. My Skylake Core-i7 has 6MB L3, 256
KB L2 and 32 KB L1d and L1i.

Accessing memory takes roughly

* Several layers of cache with different access times
* Caches uses physical addressing
* The clflush instruction will clear all caches from their content
* Need to share pages and caches
* https://stackoverflow.com/questions/61190976/does-the-store-buffer-hold-physical-or-virtual-addresses-on-modern-x86
* https://stackoverflow.com/questions/54876208/size-of-store-buffers-on-intel-hardware-what-exactly-is-a-store-buffer/54880249#54880249
* https://stackoverflow.com/questions/11105827/what-is-a-store-buffer

## Measuring Cache Access Times

* RDTSCP is partially ordered - it will not execute until all prior instructions in program order have executed
* RDTSCP can be executed later than expected, but not earlier
* LFENCE orders instructions
* LFENCE will not run until all prior instructions have completed locally and no later instructions will begin execution until the LFENCE instruction completes.
* SFENCE is needed if stores should be globally visible

There is a `__rdtscp` intrinsic function but it takes a memory address as parameter for storing core_id. We don't need it.

```
inline __attribute__((always_inline)) uint64_t rdtscp() {
   uint64_t a, d, c;
   __asm__ volatile("rdtscp" : "=a" (a), "=d" (d), "=c" (c));
   return (a | (d << 32));
}
```

An address can be measured with this function

```
uint64_t probe(char *address) {
    uint64_t t0 = rdtscp();
    *(volatile char*)address;
    uint64_t now = rdtscp();
    return now - t0;
}
```



## The Probe Function

