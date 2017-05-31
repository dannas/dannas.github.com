# Today I've learned

## 31 may 2017
I found a [HN conversations about DJB's optimizing compilers talk](https://news.ycombinator.com/item?id=9397169)
where Mike Pall, Josh Haberman and DannyBee (Compiler Lead at Google). Danny argued that compilers do before
optimizations that humans would miss and that with proper PGO-support it will always outperform a human. Mike and Josh
raised concerns.

Over the last couple of days I've revisited what I learnt in my networking courses at the University. I've realized
that the RFC's are more readable than the textbooks I was forced to read back then. Today I discovered 
[RFC 1122 - Requirements for Internet Hosts](https://tools.ietf.org/html/rfc1122) which defines how the different
parts of the TCP/IP protocol suite fits together. I also found [RFC 793 - Transmission Control Protocol], that
specifies TCP as it is used today (though there are a couple of follow-ups that
address optimizations and security hardening, TCP is still focus of much research).

## 30 may 2017
Large packets needs to be fragmented on the IP level to not exceed the Maximal
Transfer Unit (MTU). The Internet Protocol header has fields to mark a packet
as fragmented and the algorithm for reassembly is outlined in [RFC 815](https://tools.ietf.org/html/rfc815).
But fragmented packets are non-desirable for higher protocols such as TCP,
since one lost packet leads to lots of retransmissions. To remedy that, [RFC 1191](https://tools.ietf.org/html/rfc1191)
describes an algorithm for doing Path MTU discovery when initializing a connection. The Internet Protocol
sends a packet with the MTU of it's closest neighbour and then adjusts the MTU in response to ICMP packets
triggered from hosts with smaller MTUs. Unfortunately, some IP hosts filters out ICMP traffic due to perceived
security problems. That problem is referred to as Black Hole Detection. One way around it, is to rely on the MSS
field of the TCP header, but that breaks the end-to-end principle of the TCP/IP design. See [RFC 2923](https://tools.ietf.org/html/rfc2923)
for a discussion of TCP problems with the MTU Patch discovery algorithm.


## 29 may 2017
Started reading up on how do use UDP as transport layer for a protocol.
[RFC 5404](https://tools.ietf.org/html/rfc5405) provides guidelines for
application protocol designers. That's a start. Glenn Fiedler, author of Gaffer
On Games, has an article series titled [Game Networking](http://gafferongames.com/networking-for-game-programmers/)
and another called [Building a Game Network Protocol](http://gafferongames.com/building-a-game-network-protocol/).
Plan 9 has a protocol called RUDP whose implementation can be found in
[sys/src/9/ip/rudp.c](http://plan9.bell-labs.com/sources/plan9/sys/src/9/ip/rudp.c). Wikipedia has 
a [comparison of Transport Layer Protocols](https://en.wikipedia.org/wiki/Transport_layer#Comparison_of_transport_layer_protocols)
that gives a good "map of the land". Google has the experimental protocol [QUIC](https://en.wikipedia.org/wiki/QUIC) that uses
UDP but provide most of the functionality of TCP but with reduced latency.

Finally, Ilya Grigorik's book High Performance Browser Networking contains a
chapter [Building Blocks of UDP](https://hpbn.co/building-blocks-of-udp/) that
gives a good overview of what services UDP lacks and how to deal with NAT
traversal.

## 24 may 2017
I got curious about how the IP blocks in a SoC communicates. Here's what I
found:

System on a chips (SoC) contains microcontrollers, timing sources, memory,
external interfaces, peripherals, voltage regulators and analog interfaces.
These blocks are connected through a bus. Arteris has published the article
[the SoC Interconnect Fabric](http://www.arteris.com/blog/bid/99797/The-SoC-Interconnect-Fabric-A-Brief-History)
that describes how that bus operates. In the beginning was the AMBA bus where
one bus master at a time could communicate. An arbiter gives exclusive access
to one master at a time. But the arbiters became a bottleneck. Then crossbar
switches was invented. Multiple master can access the bus at the same time, but
it requires wide datapaths. When the number of masters grew, bridges were added
to the cross bar switches. Eventually that didn't scale enough. Then packet-based
Network On Chip (NoC) started to be used. Transactions are sent as serialized
data on one wire.

As for the AMBA bus, it typically consists of an AHB bus for high speed
communications and and APB bus for peripherals. The specifications can be
downloaded from the ARM website and is no longer than 20 pages or so.

## 12 may 2017
David P. Reed wrote in an [email to the end2end-interrest mailing list](http://www.postel.org/pipermail/end2end-interest/2005-February/004616.html)
that the pseudo header used for calculating the TCP and UDP checksums exists
for historical reasons. In the beginning, TCP and IP was one  monolithic
protocol. The source and destination address are part of the meaning of the
UDP/TCP frame. To avoid duplication, the psuedo header was invented. 

There was also an idea that TCP could add end-to-end encryption:

> The plan was to leave the SA and DA in the clear, but 
> encrypt the rest of the TCP payload, including the checksum.  This would 
> protect against a man-n-the-middle attack that delivered valid packets 
> with an incorrect source address, etc. (yes, to be truly reliable, we 
> would have had to use a DSA instead of the current checksum).

> This was a careful design decision, wrecked irrevocably by the 
> terrorists who invented NAT (which doesn't allow end--to-end encryption, 
> because NAT is inherently a "man-in-the-middle" attack!).

## 11 may 2017
I learned a hack for determining endianess from the musl libc:

    bool isLittleEndian() {
        union {int i; char c; } u = { 1 };
        return u.c == 1;
    }

I made good use of [RFC 826](https://tools.ietf.org/html/rfc826) for arp,
[RFC 791](https://tools.ietf.org/html/rfc791) for ip and
[RFC 792](https://tools.ietf.org/html/rfc792) for icmp. I wish I had those
protocol headers memorized.

I also stumbled upon a TCP/IP tutorial published as [RFC 1180](https://tools.ietf.org/html/rfc1180).
I wish I had that document when I started learning networking.

## 10 may 2017
Found Sami Niranens excellent blog serie [Let's code a TCP/IP stack](http://www.saminiir.com/lets-code-tcp-ip-stack-1-ethernet-arp/)
It succinctly describes the main parts needed to implement the lower parts:
ethernet, arp, icmp, ip. I managed to write a working ArpSend method. One
hurdle for me is deciding how to write the buffer API for the stack. Linux and
the BSD Unix derivates all have a skbuff structure that holds a buffer that is
passed between the layers. But how do I choose which buffer to write to and how
do I pass those buffers to the DMA engine?

I read Jason Robert Carrey Patterssons article [Modern microprocessors - a 90
minute guide](http://www.lighterra.com/papers/modernmicroprocessors/). It
describes superscalar and pipelined architectures but goes into a lot more
detail than the resources I've read before. Some takeaways: Each pipeline step
in a processor consists of combinatorical logic and a latch. The clock drives
the signals through the grid to the next latch. There are also some bypass paths
that provides the results from the execution units before they've been written
back to memory. The pipeline depth can vary a lot. 

As for issue width, it's not the same as the number of functional units. That
was a surprise to me! Issue width describes the typical number of instructions
scheduled per clock. It can't always be one ALU, one FPU and one branch for
instance. With more functional units it gets easier to saturate the scheduler.

I've been confused about what instruction latencies refers to in the cpu
reference manuals. It's the number of cycles needed before the result is
available for use. So for instance in a simple 5 stage pipeline. It might take
five clocks to get an add instruction from fetched to retired, but due to the bypass
stage, it's immediately available to the execution units after the execution step.

The article describes three walls that limits the performance a cpu can
achieve: the power wall, the memory wall and the ILP wall. I hadn't thought
about the latter before. Due to load latencies, branch misprediction and
dependencies between instructions there are limits to how much instruction
level parallelism  that can be squeezed out of a cpu.

## 9 may 2017
Peripheral registers for acknowledging, say, interrupts can come in three
forms: As one single register that can be modified and read; as a pair of
write+read registers or as a register where the read has side effects. I wonder
how the later is done on the flip-flop level? How can reading from a register
cause side effects?

## 5 may 2017
Switch debouncing times can vary widely. Jack Ganssle has written [a guide to
debouncing](http://www.ganssle.com/debouncing.htm) where he measured the
bouncing times for 18 different switches. He made 300 presses on each! The
median value was 1.5ms, but I should consult the datasheet for my switch to
be sure. There were outliers that took tenths of ms!

## 28 apr 2017
An empty std::unordered_map takes up 128 bytes. An empty std::map takes 16.
Bret Wilson [wrote](https://groups.google.com/a/chromium.org/forum/#!topic/chromium-dev/rdxOHKzQmRY):

    A lot of code uses std::unordered_map because "hash tables are
    faster." But it has high memory overhead for small sizes which most maps
    are. It's also slightly slower than std::map to insert into, has poor cache
    coherency (every insert is a malloc), and isn't as much faster at querying over
    the alternatives than you might expect (it can even be slower!).

Chromium has a document that discusses what map container to use. It can be
found [here](https://chromium.googlesource.com/chromium/src/+/master/base/containers/README.md).

## 20 mar 2017
The Linux kernel splits ethernet drivers into MAC drivers and PHY drivers.
There is a fixed-PHY driver that can be used when communicating with network
equipment that don't support MDIO, the management interface inside MII.

## 6 mar 2017
You can trigger the 'pure virtual method called' error even if you're not
calling virtual method from the base class constructor or destructor. Here's a
proof of concept.

    #include <pthread.h>
    #include <unistd.h>
    struct base {
        virtual ~base() { sleep(1); }
        virtual void func() = 0;
    };

    struct derived : public base {
        virtual ~derived()  {}
        virtual void func() {}
    };

    static void *thread_func(void* v) {
        base *b = reinterpret_cast<base*>(v);
        while (true) b->func();
    }
    int main() {
        pthread_t t;
        base *b = new derived();
        pthread_create(&t, 0, thread_func, b);
        delete b;
    }

It introduces a race between the updating of the vtable funtion pointers during
destruction and the calling of those methods.

I listened to some more lectures from Barbara Oakleys MOOC "Learning how to
learn". She talks about how interleaving different course sections produces
better results than just reading them block by block - and she has published
results to back up her claims.

## 21 feb 2017
Troubleshooted a wifi performance bug and learnt a few things about the
structure of wifi drivers. They can either have a software MAC layer or let the
firmware in the device handle the MAC layer. For the former, there's a mac80211
framework in the kernel. Access to the driver by userspace can either be
through the data path (regular sockets) or control path (netlink sockets). For
the later there's a cfg80211 framework in the kernel and on top of that sits
yet another framework for dealing with netlink communication, nl80211.


## 31 jan 2017
For a long time, I've struggled with NetworkManager on my Ubuntu laptops. I
want to use two ethernet-interfaces where one should be sharing the internet
access and must use a specified ip address range. Today I found that there is a
ip4.addresses configuration directive; if I specify an ip address (not a range,
just an address) then I can have a shared connection that uses my preferred
address range. For more information see this 
[askubuntu post](http://askubuntu.com/questions/844913/how-to-control-ip-ranges-of-network-managers-hotspots)

## 19 jan 2017
Made some [experiments](https://godbolt.org/g/sQETrj) with how C++ compilers
optimize switch statements. I'm starting to get the hang of how br_table is
generated. But I still don't understand what bug1316804 is about. What is the
"cleanup" that we should avoid?

## 18 jan 2017
Wrestling with the syntax of wast; the S-expression textual format for wasm
programs. I'm trying to simplify the generation of wasm jump tables through the
br_table opcode. Was a little heureka moment when I finally figured out that
labels for the block statement refers to the end of the block, not the beginning.

## 16 jan 2017
[Almost all](https://en.wikipedia.org/wiki/Comparison_of_smartphones) high-end
phones uses QualComm SoC's. To match the specfications of a modern smartphone,
I need to have ~2GHz clock frequency and atleast 2GB DDR3 memory. The 2016
phones uses ARMv8 cores, but the older are on ARMv7. I want to buy a dev board
for running the spidermonkey test suite. Looks like Odroid-C2 gives the best
performance/price ratio.

## 12 jan 2017
Watched first half of Cliff Clicks presentation about modern hardware. 

## 11 jan 2017
Gdb can call functions but it can't deal with [converting const char* to const string&](http://stackoverflow.com/questions/16734783/in-gdb-i-can-call-some-class-functions-but-others-cannot-be-resolved-why),
it needs the arguments to be of the exact right type. Gdb doesn't understand default arguments either.

## 10 jan 2017
The WasmExplorer has a query page. You can use this query for printing a
histogram of constant args for i32.mul operations:

    ;; The $ sigil refers to the current expression. You can access the |value| property to
    ;; refer to its text value.
    (i32.mul * (i32.const {histogram("A", parseInt($.value)) }))

## 1 jan 2017
Made an experiment for determine how the  three compilers - icc, gcc and clang - [optimize multiplication by rhs constant](https://godbolt.org/g/LZpca2)
They always convert multiplication by 2 to shifts as expected. For rhs < 10, they all generated add+shift. For rhs < 19, icc generates only add+shift 
but gcc and clang makes exceptions for cases where there are >2 bit set in rhs. They all treat 7, 15, and 31 as shift+sub. For rhs > 33, they all generate 
imul (except for the previously mentioned case, rhs is a multiple of 2)

So I can do any of these: provide fixed patterns for up to some threshold; optimize powers of two +- 1; optimize all factors with just N bits set by shl+add+shl.

## 30 dec 2016
Division expressions with 1 as divisor [may be emitted by simple compilers](http://logs.glob.uno/?c=mozilla%23ionmonkey&s=30+Dec+2016&e=30+Dec+2016#c127480)

## 27 dec 2016
Counting leading and trailing zeroes can be done by specialised x86
instructions. Bit-scan-reverse (bsr) counts leading zeroes.
I extracted mozilla::Floorlog2 into a [standalone
file](https://godbolt.org/g/5zR0Fr) and was surprised to find out to see that
all the templates was compiled down into a bsr and a neg. Though I don't know
what clang does; how come there's no subtract of 63 or 31?

## 26 dec 2016
Here's an example of conditional execution of instructions on ARM

    int condSet(int cond, int a, int b) {
        return cond ? a : b;
    }

Generates this code:

            cmp     r0, #0
            ite     ne          ; IfThenElse
            movne   r0, r1
            moveq   r0, r2
            bx      lr          ; lr contains return address

icc on x86-64 generates:

        test      edi, edi
        cmovne    edx, esi
        mov       eax, edx
        ret       

ARM has published an [article series on conditional execution](https://www.community.arm.com/processors/b/blog/posts/condition-codes-3-conditional-execution-in-thumb-2)

## 25 dec 2016
The AMD64 ABI uses rdi, rsi, rdx, rcx, r8, r9, xmm0-7 for passing function
parameters. The Linux kernel uses the identical registers, but rcx is replaced
by r10. Why? It's because [rcx is clobbered by the syscall 
instruction](http://stackoverflow.com/questions/38577609/difference-in-abi-between-x86-64-linux-functions-and-syscalls)

In the ARM instruction set, every instruction can be conditionally executed.
That has been [removed in ARMv8](http://stackoverflow.com/questions/22168992/why-are-conditionally-executed-instructions-not-present-in-later-arm-instruction), 
due to excessive code space usage. I'm looking at 
[bug 1316822](https://bugzilla.mozilla.org/show_bug.cgi?id=1316822). Looks like
there should be something equivalent to setcc. ARMv8 has cset. I wonder what
ARMv7 uses?

## 23 dec 2016
Finding a resource that lists most of the linux-related settings that I need to
control before a benchmark hasn't been easy. But the CI tools for the Julia
language has an wiki page
[linuxtips.md](https://github.com/JuliaCI/BenchmarkTools.jl/blob/master/doc/linuxtips.md)
that is very useful. Found two useful articles, [Robust benchmarking in noisy enviroments](https://arxiv.org/abs/1608.04295) and 
[Rigorous benchmarking in reasonable time](https://kar.kent.ac.uk/33611/7/paper.pdf). 

I did some experiments with the [box2d benchmark](https://github.com/joelgwebber/bench2d/) 
compiled for WebAssembly in the [embenchen](https://github.com/kripken/embenchen) benchmark suite. It's
supposed to stress floating point operations and is compute intensitive.
Noticed that there are large variantions when running turbo boost was not
disabled. But high loads was an even larger factor for large variance.

## 21 dec 2016
Since Intel Sandy Bridge, there's a new cpu kernel driver, intel_pstate. The
cpupower userspace tools do not work with that driver; you'll have to read and
write to sysfs directly. You can't set a fixed frequency; for that you'd have
to disable the intel_pstate driver from the kernel commandline.

There's a [long Google+ thread](https://plus.google.com/+TheodoreTso/posts/2vEekAsG2QT)
where Theodore Tso and Arjan van den Veen discuss how Intels p-state management works.

I saw large variations when benchmarking cpu-bound wasm code with 1-10 seconds
of runtime. Those went mostly away - range of measurements decreased by a 10x
factor - when I disabled the turbo boost feature:

    echo "1" | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo

To inspect the current cpu frequency, this command can be used:

    watch -d grep \"cpu MHz\" /proc/cpuinfo 

The powersave governor (the default) can be set with this command

    echo "powersave" | sudo tee /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor 

## 20 dec 2016
Benchmarking problems. The cpu frequency varies wildly and I can't change
governor or set a fixed frequency. This is because Linux uses the [pstate](https://wiki.archlinux.org/index.php/CPU_frequency_scaling#CPU_frequency_driver)
driver on Intel Skylake and newer chips. Found a [redhat bug report](https://bugzilla.redhat.com/show_bug.cgi?id=1378529) that referenced a
blog series, [Haypos notes on microbenchmarking](http://haypo-notes.readthedocs.io/microbenchmark.html)

## 17 dec 2016
Spidermonkey uses the terms nunboxing and punboxing for referring to the
javascript value representation. Rob Sayre has written a good
[overview](https://evilpie.github.io/sayrer-fatval-backup/cache.aspx.htm) and
Andy Wingo has written an article, [value representation in javascript
implementations](https://wingolog.org/archives/2011/05/18/value-representation-in-javascript-implementations)
that contrast the Spidermonkey implementation with other js engines.

## 16 dec 2016
I've had problems when simultaneously installing multilib and cross compilers;
gcc-multilib installs a /usr/include/asm symlink that messes up the include
path for cross compilers. Hence, the deb packages for the cross compilers
conflicts with gcc-multilib. These things make me so tired. I just don't want
to understand. I want it to work. Luckily enough, it turns out that x86 gcc has
/usr/local/include as search path while the cross-compilers don't. So I
installed a symlink from /usr/local/include/asm to
/usr/include/x86_64-linux-gnu/asm.

Managed to build the spidermonkey shell for x86, arm, x63 and arm64. Here are
the configure commandlines:

    PKG_CONFIG_LIBDIR=/usr/lib/pkgconfig CC="gcc -m32" CXX="g++ -m32" AR=ar ../configure --target=i686-pc-linux --enable-debug --disable-optimize --without-intl-api
    AR=ar CC="gcc -m32" CXX="g++ -m32" ../configure --target=i686-pc-linux --enable-debug --disable-optimize --enable-simulator=arm --without-intl-api
    ../configure --enable-debug --disable-optimize --without-intl-api
    ../configure --enable-debug --disable-optimize --enable-simulator=arm64 --without-intl-api

## 15 dec 2016
Found Thomas Ptaceks [Application Security Reading
List](https://www.amazon.com/gp/richpub/listmania/fullview/R2EN4JTQOCHNBA). A
few books that I have high thoughts of like C Programming interfaces, The
Practice of Programming and Javascript - the Good Parts; and then a dozen books
that were unknown to me. I've added TOASSA to my TODO-reading list. Reversing:
the Secrets of Reverse Engineering looks promising as well. Might be useful when
I try to decipher Spidermonkey JIT code.

## 14 dec 2016
You can build a [40-hw thread xeon machine](http://www.techspot.com/review/1218-affordable-40-thread-xeon-monster-pc/)
for about $1000. I have a 8-hw thread i7 Dell XPS 15 laptop. State of the art of
2016, but it still takes *a lot of time* to build the firefox source tree.
Sooner or later, I will buy a PC with enough cores (though I've been saying
that for at least five years without much happening).

There's an environment variable that can be set to control how [Qt5 deals with high DPI displays](https://blog.qt.io/blog/2016/01/26/high-dpi-support-in-qt-5-6/). 
Up until now I've just accepted that QtCreator looks horrible on my laptop. Those days are over! Next step: Fix Firefox too.

Wrote a small python script that mimics the golang net.Listen and net.Dial
functions. The BSD socket API is way too low-level. I have to look up the
functions each time I'm writing a small script.

Found QStandardItemModel; much simpler than inheriting from QAbstractItemModel.

Until now I hadn't ever messed up operator priorities in macros. But now I did big time.

    #define P(x, y) do { printf("%d %d %d\n", x, y, x/y); } while (0)

Always put parenthesis around every variable in a macro. Correct version:

    #define P(x, y) do { printf("%d %d %d\n", (x), (y), (x) / (y)); } while (0)
