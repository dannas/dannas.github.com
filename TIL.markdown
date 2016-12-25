# Today I've learned

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
