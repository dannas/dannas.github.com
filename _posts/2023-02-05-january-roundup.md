---
layout: post
title: What I've been reading in January
---

<!-- excerpt start -->
Here are the articles, videos, and tools that I'm excited about this August.
<!-- excerpt end -->

The format of this entry is stolen from the [Interrupt MemFault blogs monthly roundup](https://interrupt.memfault.com/tags#roundup) posts. You should read those. They're way better than mine!

## Articles & Learning

* [Monitoring context switches on Linux](https://wolf.nereid.pl/posts/linux-context-switch/) by Bartosz Taudul<br/>
  The Superluminal profiler traces context switching and Bartosz (the author of the competing open-source Trace profiler) wanted to have that too. But he didn't settle for Windows support, he went after Linux as well. Tracing context switches is a hard task on Windows but Linux isn't any better.  Bartosz describes several wrinkles in ftrace and perf_event_open.
* [A complete guide to lock convoys](https://davekilian.com/lock-convoys.html) by Dave Kilian<br/>
  Sometimes throughput tanks and you don't know why. You really don't know why. If it happens that throughput restores itself if you pause the process you might have a lock convoy on your hands. It might happen if you have some request processing loop with tight constraints that takes a lock for a short amount of time. But for some reason one of the requests takes the lock for far longer. Now the other requests are starting to queue up: the convoy is forming. So now the latency of each requests must include the convoy wait time. But what's worse: Locks are designed to have a fast-path (for uncontended accesses) and a slow-path (for contended accesses). So not only do the requests have to wait in line: the processing of each request is also slower since it must use the locks slow-path (which involves system calls and are much slower).
* [TCP socket listen: a tale of two queues](http://arthurchiao.art/blog/tcp-listen-a-tale-of-two-queues/) by Arthur Chiao<br/>
  Accepting a TCP connection requires the kernel to allocate some resources. That might lead to DOS-attacks if a remote peer does SYN-flooding. AFAIK, before reading this article, the kernel therefore keeps incoming requests in a  sort of minimal state until an application running on the kernel has accepted the request. This article dives deep into this part of the networking code. It goes through common data structures; the listen system call; and how the TCP 3-way handshake is carried out in code.
* [An introduction to kernel exploitation](https://pwning.systems/posts/an-introduction-to-kernel-exploitation-part1/) by Jordy Zomer<br/>
  Try exploiting a double fetch bug exposed in a simple linux character device written by Jordy. The code is simple but it highlights the principles behind many exploits and it has exercises where you can experiment yourself. This seems to have been planned to be a regular series of posts but unfortunately Jordy has only posted episode one! Here's to hoping he'll get around to writing more posts!
* [Setting up a kernel debugging environment](https://pwning.systems/posts/setting-up-a-kernel-debugging-environment/) by Jordy Zomer<br/>
  Jordy describes how you can use quemu or virtme for running your custom built kernels under an emulator.
* [Linux containers in 500 lines of code](https://blog.lizzie.io/linux-containers-in-500-loc.html) by Lizzie Dixon<br/>
  Lizzie describes namespaces, capabilities, cgroups and setrlimit and how they can be used to run containers on Linux. It's a literate program written using the noweb tools. My oh my, there's a lot happening in this script. It's confusing to see how much overlap in functionality there are between different Linux APIs.