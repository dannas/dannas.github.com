---
title: Report from the Adhoc Event 23 April
layout: post
---
I attended a remote virtual event organized by Paul Khuong, Samy Al Bahra and
Jessica Natoli. Four short 15 minutes talks followed by Q&A. Great topics,
professional, pedagogic speakers; and the format - short talks with
recommended reading - before attending was great. I only wished that I had more
time to prepare. A big thank you to Paul, Samy, Jessica, and all four
presenters! I hope there will be more events!

## Making Safe Memory Reclamation a Feature Of Your Memory Allocator
Jeff Roberson talked about how he added a novel memory reclamation scheme to
the Free BSD slab allocator. Lots of new things for me here. I've heard about
slab allocators before but I've never taken the time to read up on how they
work. I knew that you're supposed to keep objects of the same type in bins.
What was new to me was that you separate the malloc/free part from the
construct/destruct of the objects. You allocate a slab with a bunch of objects
and call the constructor function on them. Then you hand the objects out as
the clients need them. But when you get an object back, you keep it around in
a "base-constructed" state. So when it's needed again you can just hand it out. 

Slab allocators are a neat idea and I think I could grasp it, but then there's
memory reclamation. Phew! That was way harder. I had never heard of memory
reclamation. As I understand it, for lock-free data structures, we need a way
to know when an object is not used anymore. And we need to figure it out without
blocking. We can use reference counting or we can use hazard pointers or  use
quiescent-state-based grace periods (QBSR) or epoch grace periods (EBR). Each of those ideas
was new to me, but they've existed for a  while and are used in different
forms for non-blocking data structures.

But Jeff has figured out something new. He calls it Global Unbounded Sequence
(GUS). It shares some similarities with the Epoch-based approach but allows a
larger range of sequence numbers. One thing I had a hard time figuring out when
reading the background material was how the actual detection of memory ready to
be reclaimed happens. Do we need a separate thread that scans the list of objects
or can we do it upon every read/write operation?

Jeff describes how GUS can do the checks per slab instead of per object which
increases performance. There's one slab in the per-CPU cache for allocations and
one slab for frees. When the frees slabs fill up, it's passed to a free list
and stamped with the lowest sequence number visible at that time. When the slab
reaches the front of that list, the allocator checks if it can be reused for
allocations by comparing its sequence number to the global sequence number. I so
which that I knew more about memory reclamations, but it sounds like a great idea
to me at least!

## A 15-minute History of Task Isolation on Linux

**Update**: Matt has written a blog post summarising his talk: [Reducing jitter on
Linux with task isolation](https://www.codeblueprint.co.uk/2020/05/03/reducing-jitter-on-linux-with-task-isolation.html).

Matt Flemming described how task isolation has improved in the kernel. When I
first saw the title of the talk I thought Matt was going to talk about process
isolation - how every resource can be virtualized with cgroups and such, but that
was not the case.

What he and others have tried to achieve is to minimize interruptions - a process
should run as if it was the only process on a CPU, preferable it should be
the *only* process. When I heard that I thought of RTOSs and userspace
networking. And yup, that's the kind of users they're targetting. 

They want to get rid of preemption, interrupts, timers, and RCU callbacks. I'm
not quite sure how RCU callbacks are used in the kernel.

When I'm benchmarking I use `taskset` for binding a process to certain cores.
That is done through the sched_setaffinity call. I've also used heard of people
using `cset`. I wonder if it uses the same mechanism? I think I've read somewhere
that it is supposed to block out interrupts as well. Matt talks about the `isolcpu`
kernel command-line parameter. I need to read up on that one.

From version 3.10 there is a nohz_full but it still caused a tick every second.
But in version 4.17, that tick is disabled. But Perf events and timers can still
cause some ticks. There is a task isolation mode that is in the process of
being upstreamed.

I wonder what the future use cases for this will be? Is it possible that more
services besides networking may move into user-space and use isolated CPUs for
running their workloads?

## Why Mmap is Faster Than System Calls
Alexandra Feodorova has written a storage engine for modern fast SSD devices.
She described how due to decreasing SSD wait times, the filesystem code has
become a bottleneck. There is newer Linux file system but they haven't been
tested much yet. I believe the standards for filesystem testing is higher than
for many other components of a software stack.

So instead of rewriting kernel parts, she and her colleagues wrote a new
storage engine for MongoDB. They used `mmap` since that means less system call
overhead, and `memcpy` from a mmapped buffer is substantially faster than read's
copy_to_user. That is because the kernel can't use vector instructions. Todd
Lipcon said that newer CPUs have an enhanced REP MOVSB (ERMSB) which
should perform about as good.

For the implementation of the MongoDB storage engine, they split the threads
into writers and readers. A write was anyone writing beyond EOF, calling
`fallocate`, or `ftruncate`. When a writer is active, I/O is routed to system calls
which are properly synchronized.

The write pseudo-code is executed before a write op
```
wait:
	while mmap_resizing
		spin_backoff(...)
	if cas(mmap_resizing, 1, ...)
		goto wait
	while mmap_use_count
		spin_backoff(...)
	do_write(...)
	unmap(file)
	remap(file, file)
	mmap_resizing = 0

```	
And here's read (something similar needs to be done by writers too for checking if they can use mmap)
```
atomic_add(mmap_use_count, 1)
if mmap_resizing
	atomic_decr(mmap_use_count, 1)
	read_syscall(...)
else
	memcpy(dst_buf, mapped_buffer, ...)
	atomic_decr(mmap_use_count, 1)

```


## jitmap, an execution engine for bitmap expressions
Francois Saint-Jacques was presenting on an interesting subject, but I didn't have the time to do the
background reading or watch the presentation. I'll add a small summary here, once I've watched the recording.
