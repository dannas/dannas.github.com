---
layout: post
title: What I've been reading in June
---

<!-- excerpt start -->
Here are the articles, videos, and tools that I'm excited about this June.
<!-- excerpt end -->

The format of this entry is stolen from the [Interrupt MemFault blogs monthly roundup](https://interrupt.memfault.com/tags#roundup) posts. You should read those. They're way better than mine!

## Articles & Learning
* [Prefer Consumption Metrics to Time Metrics](https://medium.com/@ricomariani/analysis-prefer-consumption-metrics-to-time-metrics-d9192192b6af) by Rico Mariani<br/>
"Elapsed time is data I can only cry about"! Rico says that even though
elapsed time is what the customers are often interested in, you can't
tell from just looking at the time why things are slow. Rico prefers consumption
metrics, things like disk (read ops, write ops, total bytes, queue length),
CPU (instructions retired, branch predicts, even maybe CPU time), memory (L2,
TLB, page faults), network (round trips, total bytes), GPU (texture sizes,
vertices, shader cost), cardinality (# of draws, # of layouts, # of renders).
There are many more. These data are more sensitive and more probative. And
they're often much more stable - and therefore more useful in lab settings.
* [Minimum Times Tend to Mislead When Benchmarking](https://tratt.net/laurie/blog/entries/minimum_times_tend_to_mislead_when_benchmarking.html) by Laurie Tratt<br/> 
I've been told that noise can only slow a program down. So I should run a
program which I want to measure the execution time of several times and take
the minimum time from those runs since it's closest to the "true"
performance. But Laurie argues that nondeterminism is always present, be it from
the influence of caches; the branch predictor; or memory layout of the
process. So we should always look at the distribution of execution times
rather than just blindly accepting the minimum.
* [Understanding the Glibc Heap Implementation](https://azeria-labs.com/heap-exploitation-part-1-understanding-the-glibc-heap-implementation/) by Maria Markstedter<br/>
Maria describes the Glibc heap implementation as a prerequisite for
understanding heap-based exploits such as use-after-frees, double-frees, and
heap-overflows. The text has very informative illustrations and there are
links to the Glibc source code if you want to understand the allocation
algorithms in more detail. Finally, she has references to five Phrack heap
articles. You should read Phrack! It is (was?) great!
* [You're Doing It Wrong](https://queue.acm.org/detail.cfm?id=1814327) by Poul-Henning Kamp<br/>
Poul-Henning describes how the Varnish HTTP accelerator uses a binary-heap
data structure that is optimized for page usage and achieves significantly
better performance compared to a regular binary-heap. He also touches on the
fact that Varnish uses virtual memory as the caching layer.
* [What's Wrong with 2006 Programming](http://oldblog.antirez.com/post/what-is-wrong-with-2006-programming.html) by Salvatore Sanfilippo<br/>
Salvatore argues that using virtual memory for caching is not a good solution
for Redis. It's bad since the OS paging causes blocking for long periods  ;
you have little control over the kernels decision on what to page out; and the
4K granularity for pages does not match Redis storage format. There's a great
discussion in the comments between Poul-Henning and Salvatore. Poul-Henning
argues that Salvatore's points about VM limitations can be resolved, though he
admits that the POSIX API is far from ideal and he also says that threads
would overcome the problems with blocking during paging. To that, Salvatore
replies that Redis is meant to be run as a cluster, with one instance per
core. So Redis will stay single-threaded.
* [Ad Hoc Profiling](https://blog.mozilla.org/nnethercote/2018/07/24/ad-hoc-profiling/) by Nicholas Nethercote<br/>
A description of a small command-line program for tallying line frequencies
within text files. Nicholas has done a lot of impressive performance
optimizations for the Rust compiler in recent years and he says that counts
have been used for every single investigation. The blog post contains many
examples of what to measure and how to interpret the results.

## Videos
* [Performance Matters](https://www.youtube.com/watch?v=r-TLSBdHe1A&list=WL&index=9&t=0s) by Emery Berger<br/>
Just comparing performance before and after a code change is not enough.
The process layout can bias measurements: Link order and environment variable size have a
larger impact than switching to -O3. Emery's tool stabilizer eliminates the
effect of layout and thus enables sound performance evaluation. Stabilizer
compares the distributions for figuring out if changes has improved the code.
But that's not all: Emery and his team have created the coz profiler which
allows a user to figure out the impact of optimization. They optimized SQLite
by 25%!
* [Designing a Fast, Efficient, Cache-friendly Hash Table, Step by Step](https://www.youtube.com/watch?v=ncHmEUmJZf4&t=3s) by Matt Kulukundis<br/>
A presentation of Google Abseils 2-level N-way associative hash table, using
Robin Hood hashing. A new hash table is constructed step-by-step by loosening the
constraints of the standard library containers and optimizing for data locality.

## Neat Open Source Projects
* [Stb](https://github.com/nothings/stb)<br/>
Single file implementations in C of image loaders, image writers, font text
rasterizers, sound decoders and much more. Mostly written by Sean Barrett.
* [jo_mp1.cpp](https://www.jonolick.com/uploads/7/9/2/1/7921194/jo_mp1.cpp)<br/>
Jon Olicks MPEG-1 Audio Layer1 decoder. 397 LoC. No memory allocation.
* [jo_mpeg.cpp](https://www.jonolick.com/uploads/7/9/2/1/7921194/jo_mpeg.cpp)<br/>
Jon Olicks MPEG writer. 256 lines of code. No memory allocations.
* [jo_jpeg.cpp](https://www.jonolick.com/uploads/7/9/2/1/7921194/jo_jpeg.cpp)<br/>
Jon Olicks JPEG writer. 336 LoC. No memory allocations.
* [jo_gif.cpp](https://www.jonolick.com/uploads/7/9/2/1/7921194/jo_gif.cpp)<br/>
Jon Olicks GIF writer. 398 LoC. No memory allocations.

## Tools
* [Stabilizer](https://github.com/ccurtsinger/stabilizer)<br/>
The dynamic memory layout randomization from the linked Emery Berger video.
* [Coz](https://github.com/plasma-umass/coz)<br/>
The casual profiler that measures optimization potential. It measures
optimization potential for serial, parallel, and asynchronous programs. See
Emery Berger's video for more information.
* [Counts](https://github.com/nnethercote/counts)<br/>
Ad-hoc profiling tool that tallies line frequencies within text files. See the
link under the Articles section.

