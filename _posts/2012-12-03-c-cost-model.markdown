---
layout: post
title: C cost model
---

In chapter [7.2][pp-chapter] of Programming Pearls, Jon Bentley asks:

> You know that the run time of your numerical algorithm is dominated by its
> n^3 square root operations, and n=1000 in this application. About how long
> will your program take to compute the one billion square roots?

I've always been told that calculating square roots is slow, but how slow? A
computer can execute about one billion instructions per second. Doing a sqrt
calculation is probably in the nanosecond range but will it take 10, 100 or
1000 nsecs? Let's do some experiments and find out!

I've used Bentley's [timemod.c][pp-timemod].The idea is to loop over a
statement n times and record the average. It's not meant to be precise but
give an estimation (closest power of ten) of the running time. The tests were
done on my low end ASUS laptop equipped with a [Intel Core 2 Duo SU7300][cpu].

[pop-cputimes]: http://www.netlib.org/research/cputimes.c
[pp-chapter]: http://www.cs.bell-labs.com/cm/cs/pearls/sec072.html
[pp-spacemode]: http://www.cs.bell-labs.com/cm/cs/pearls/spacemod.cpp
[pp-timemod]: http://www.cs.bell-labs.com/cm/cs/pearls/timemod.c
[cpu]: http://ark.intel.com/products/42791/Intel-Core2-Duo-Processor-SU7300-3M-Cache-1_30-GHz-800-MHz-FSB

Integer Arithmetic (n=5000)  

    {}                     5  
    k++                    5  
    k = i + j              6  
    k = i - j              6  
    k = i * j              6  
    k = i / j              7  
    k = i % j              7  
    k = i & j              6  
    k = i | j              6  

Floating Point Arithmetic (n=5000)  

    fj=j;                  6  
    fj=j; fk = fi + fj;    6  
    fj=j; fk = fi - fj;    6  
    fj=j; fk = fi * fj;    6  
    fj=j; fk = fi / fj;   17  

Array Operations (n=5000)  

    k = i + j             6  
    k = x[i] + j          5  
    k = i + x[j]          5  
    k = x[i] + x[j]       6  

Comparisons (n=5000)  

    if (i < j) k++        6  
    if (x[i] < x[j]) k++  6  

Array Comparisons and Swaps (n=5000)  

    k = (x[i]<x[k]) ? -1:1  6  
    k = intcmp(x+i, x+j)   10  
    swapmac(i, j)           9  
    swapfunc(i, j)         12  

Max Function, Macro and Inline (n=5000)  

    k = (i > j) ? i : j    7  
    k = maxmac(i, j)       6  
    k = maxfunc(i, j)     10  

Math Functions (n=1000)  

    fk = j+fi;              4  
    k = rand();            36  
    fk = sqrt(j+fi)        18  
    fk = sin(j+fi)         94
    fk = sinh(j+fi)       468  
    fk = asin(j+fi)       368  
    fk = cos(j+fi)         92  
    fk = tan(j+fi)        116  

Memory Allocation (n=500)  

    free(malloc(16));       80  
    free(malloc(100));     128  
    free(malloc(2000));    128  

According to these experiments, doing one sqrt operation takes about 20 nsecs.
I guess that calculating one billion square roots takes about 20 seconds on my
system.
