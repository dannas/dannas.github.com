---
layout: default
title: TIL - Today I've Learned
use_math: true
---
# Today I've learned

## 19 June 2019

I was discussing endianess with a colleague and realized that I had  a fuzzy idea about why there are little-endian and big-endian systems. A quick search led me to Danny Cohens [On Holy Wars and a Plea for Peace](https://tools.ietf.org/rfc/ien/ien137.txt) that introduced the endianess concept.

## 17 June 2019

A process under Linux has an upper limit on number of allowed open file descriptors. Finding a reliable way to read out the highest open file descriptor has turned out to be a bit of a challenge. There is the `OPEN_MAX` define but it is set at compile time - the system can change the limit at runtime. Using `sysconf(_SC_OPEN_MAX)` gives a value that may be too high if `setrlimit(RT_NOFILE)` has set a lower value.  And on some systems `sysconf` just return `INT_MAX` .You can iterate over the symbolic links in `/proc/self/fd/` but `proc` is not guaranteed to be mounted in a `chroot` environment for example. 

## 14 June 2019

Here's a small script written by Rob Landley for listing functions in a binary and for printing the disassembly of a chosen function. I've added options to `objdump` for printing in Intel syntax; showing in C++ symbols in plain-text; and for printing relocation entries.

```
[ $# -lt 1 ] || [ $# -gt 2 ] && { echo "usage: showasm file function"; exit 1; }
[ ! -f $1 ] && { echo "File $1 not found"; exit 1; }
if [ $# -eq 1 ]
then
  objdump -d $1 | sed -n -e 's/^[0-9a-fA-F]* <\(.*\)>:$/\1/p'
  exit 0
fi
objdump -drwC -Mintel $1 | sed -n -e '/./{H;$!d}' -e "x;/^.[0-9a-fA-F]* <$2>:/p"
```

## 13 June 2019

Serial ports drivers consists of a tty, line discipline and uart layer.

* tty deals with job control and signals

* There's a `serial_core` framework that is part of the `tty` leayer. It eases the development of serial (uart) drivers.
* N_TTY is the line discipline for normal serial ports (others exists for IR, SLIP, etcetera)
* uart controls speed, modem control lines (RTS, CTS, DTD, etcetera), break signals, xon/xoff flushing, CSIZE, CSTOPB, PARENB, PARODD, 

Bootlin (former Free Electrons) has published [document describing Linux serial drivers](https://bootlin.com/doc/legacy/serial-drivers/linux-serial-drivers.pdf). Nelson Elhages blog article [A Brief Introduction to Termios and Stty](https://blog.nelhage.com/2009/12/a-brief-introduction-to-termios-termios3-and-stty/) gives a good summary of how which setting belongs to which layer in the serial driver stack.

## 11 June 2019

Arjun Narajans blog post [A History of Transaction Histories](https://ristret.com/s/f643zk/history_transaction_histories) has long been on my reading list. To me a transaction means a set of actions that can be done atomically: either they all succeed or the transaction is undone (rolled back in database-parlance). But what happens if there can be multiple transactions in flight at the same time? What level of isolation can the system provide? Here, things start to get complicated. ANSI SQL has defined four isolation levels: serializable; repeatable reads; read committed; and read uncommitted. Those were defined in terms of the locking strategies needed to avoid dirty reads, non-repeatable reads and phantom reads. But that definition was not good enough. The isolation levels should have been defined in terms of  the transaction histories (the interleaving) that fulfil the isolation requirements. That didn't happen until just a few years ago.

This blog post was talking about things that I haven't thought much about before. Atleast now I'm familiar with some terms and definitions, but there's much left to learn.

## 10 June 2019

Mike Actons talk at GDC 2019 [Everyone Watching this is Fired](https://gdcvault.com/play/1025778/Everyone-Watching-This-Is) was just as confrontational and harsh as every other recorded talks I've watched by him. And just as the others, packed with ton of useful insights!  Mike asks the audience to keep track of their answers to 50 yes/no questions that he poses. Everyone that answers no to any of those questions are fired! It's questions like "can you articulated precisely what problem you're trying to solve", "Do you have a plan B if your current solutions doesn't work out", "can you articulate the steps needed to solve your problem", "can you articulate the various latency and throughput requirements"? And the list of questions goes on and on.

Some questions are about knowing the machine, some are about having work ethics, some are about making room for the unexpected and a couple of the most important ones are about how to minimize uncertainty about what it is really you're trying to achieve. Those reminded me of Brian Hooks blog post [Productivity vs Uncertainty and Apathy](http://bookofhook.blogspot.com/2012/09/productivity-vs-uncertainty-apathy.html). Brian writes about high-level uncertainty - when you don't know where you are and what you want to do; about strategic uncertainty - when you have a defined goal but are unsure how to go about it; and low-level uncertainty - when you know what you're supposed to do but are just staring at your editor, unsure what to do next. Luckily, he has suggestions for solutions to all of those (which unfortunately is more than what Mike Acton offers).

## 9 June 2019

I've been doing som Scratch-programming with my daughter. We've built simple maze games, platform games, paint programs etcetera. It's been fun but I've felt a bit uncertain about the underlying computation model: How many threads are running in a Scratch program? How are events handled? David J. Malans tutorial [Scratch for budding Computer Scientists](https://cs.harvard.edu/malan/scratch/index.php) says that each sprite can have multiple threads and that each thread is started by a when block. These threads can communicate via events that are sent using the broadcast block and received using the "When I receive"-block.

[Scratch wiki section for Computer Science](https://en.scratch-wiki.info/wiki/Category:Computer_Science) goes into more detail about these things.

## 28 May 2019

 Marc Costas blogpost [Tips on Writing Code for Data-Oriented Design](https://marccgk.github.io/blog/tips-on-writing-code-for-data-oriented-design/) discusses common themes when writing Data Oriented code. Write straight forward code and wait with factoring it until logic has been duplicated three times. Use simple allocation strategies like temp allocators and arena allocators. Avoid RAII since it ties allocation to initialization making it harder to write custom allocators. Prefer discriminated unions or composing types with an explicit type header to class hierarchies. Keep data that is processed similarly together. 

## 27 May 2019

The Bradfield School of Computer Science has published [Teach Yourself Computer Science](https://teachyourselfcs.com/), a guide for acquiring the skills you'd obtain from getting a Computer Science degree from a university. They recommend books and online courses. It's a guide I wish I had read before I started picking courses for my degree. I prefer Bryant and O'hallarans [Computer Systems: a Programmers Perspective](https://csapp.cs.cmu.edu/) over the Patterson and Henneys book for computer architecture and Appels [Modern Compiler Implementation in ML](https://www.cs.princeton.edu/~appel/modern/ml/) over the Dragon Book. Those books are more applied and the expense of some depth.

## 26 May 2019

Rico Mariani has written ton of useful articles about optimizing computer programs. In [Holistic Approaches to Performance Analysis](https://blogs.msdn.microsoft.com/ricom/2015/05/31/holistic-approaches-to-performance-analysis/) he says that many tools and methodologies for evaluating performance misses the "when". You get summary data that hides both the variance and the time stamps when things went awry. I've found that time-line graphs of consumption metrics like CPU time, disk accesses and invocation of interrupt handlers makes it much easier to pinpoint problems. Adding [invasive tracing that generates data that can be viewed by Google Chromes builtin tracer](https://www.gamasutra.com/view/news/176420/Indepth_Using_Chrometracing_to_view_your_inline_profiling_data.php) is one easy way to get that kind of time-line. I've also used [ITM and ETM tracing with sigroks PulseView](http://essentialscrap.com/tips/arm_trace/theory.html) for getting time-line graphs for embedded systems.

## 25 May 2019

Rico Marianis [Approximately Correct Notes on Threading Models](https://medium.com/@ricomariani/approximately-correct-notes-on-selected-threading-models-f96543c7c4a2) describes what he thinks are the main kind of choices you have for designing threading solutions. The idea is that you're going to get an object O from some provider P and then use it.

**Apartment Threaded**. When you ask for an O from P it must allocate the memory for O on your thread. You can then use O but only from your thread. O does not need any locking, it's constrained to be used in one thread. Most UI objects are allocated this way.

**Free Threaded**. When you ask for O from P it can give you any O it has lying around. It may or may not have been allocated on your thread and it may or may not be currently in use by some other thread. Lots of shared collections and caches try to do this business.

**Rental**. It’s kind of a blend. This is Ricos favorite threading model. P can keep a cache of objects O to hand out. It manages them. Once an O has been given out no other thread is allowed to use that O. That means O doesn't need any locking. It is being used by exactly one thread, different threads over its lifetime (so it can’t use thread local storage) but only one at a time. Sometimes database wrappers are rented, or file wrappers.

## 24 May 2019

Gregory Szorcs article [Absorbing Commit changes in Mercurial 4.8](https://gregoryszorc.com/blog/2018/11/05/absorbing-commit-changes-in-mercurial-4.8/) describes a new subcommand that will make the task of incorporating changes after coding reviews easier. Right now I use `hg histedit` and `hg commit --interactive`  but it's tedious work. With `hg absorb` the version control system can automatically figure out which changes ought to go to which commits, and most of the time get it right! Even better - it just skips changes that are ambiguous so it will never produce merge conflicts. That's because it operates on the history of lines instead of doing a three-way merge like `hg histedit` or `git rebase`. Gregory has examples to demonstrate the effectiveness of the new tool.

## 23 May 2019

Philip Johnstons article [Beyond Continual Improvement](https://embeddedartistry.com/blog/2019/2/5/beyond-continual-improvement) summarizes a talk by Russ Ackhoff. The definition of a system as "a whole with interdependent parts that can have an effect on the overall system behavior" was enlightening to me. Ackhoff uses the human body as an example: what the lungs are doing are affected by what the brain is doing and what the heart is doing. 

When an organization tries to optimize something they rarely take the whole system into account, but when a system is taken apart it looses its essential properties. Take the car: the essential property of a car is that it can carry you from one place to another, no part of a car can do that. You can write, your hand can't write. It's easy to demonstrate, just cut it off, put it on the table and watch what it does.

A system is not the sum of its part, it's the product of their interactions. If you use the best engine from all available cars, the best transmission, the best battery and so on, you won't get the best car. When a client comes in to an architect and says I want to build a house for my family with three bedrooms, two toilets and a garage, the architect won't start to design each room individually. No, he draws the house and make the rooms fit into the house.

Finding deficiencies and getting rid of them is not a way of improving the performance of a system. What you need to do is to sit down and think to yourself: what would I do if I could do whatever I wanted. Some people can't answer that, but if they can't figure out what they want, how are they gonna know what they want under constraints?

Discontinuous improvement are more important than continual improvement. When you look at what the Japanese has done in the sixties and seventies there is no doubt that they have improved the quality of the automobile, but it's the wrong kind of quality. There's a fundamental difference between doing things right and doing the right thing. The car is destroying urban life all around, just visit Mexico City and you'll find congestion and pollution so bad that children has to be kept home from school. Quality ought to contain a notion of value, not just efficiency.

## 22 May 2019

I always learn something new when reading Nicholas Nethercotes blog posts about the performance optimizations he's done on the Firefox web-browser. In [Ad Hoc Profiling](https://blog.mozilla.org/nnethercote/2018/07/24/ad-hoc-profiling/) he describes a small utility, `counts`, that he uses to tally line frequencies within a text file, an improvement over the UNIX command chain `sort | uniq -c`.

With `counts` you can get a distribution of lines counts or a distribution of weights. It's advantage is the swiftness with how you can obtain data. Alternatives like tracing frameworks or histogram hash tables takes time to setup. With `counts` you just have to insert `printf` statements and do the post-processing on the resulting file.

## 21 May 2019

Whenever I need to measure elapsed time for some ad-hoc benchmark, I always create at least two silly bugs when converting between `timespec` values to intervals. From now on I'm gonna use this template.

```
struct timespec now() {
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  return now;
}

long interval_ns(struct timespec tick, struct timespec tock) {
  return (tock.tv_sec - tick.tv_sec) * 1000000000L
      + (tock.tv_nsec - tick.tv_nsec);
}

int main(){
  struct timespec tick, tock;
  
  tick = now();
  // ### Code to benchmark
  tock = now();
  long ns = interval_ns(tick, tock)
  printf("It took %.2fus\n",  (double) ns / 1e3);
}
```

## 20 May 2019

Michael Kerrisks book The Linux Programming Interface has a long table in section 28.4 that describes how process attributes are changed during the `fork` and `exec` system calls.  Things related to the process address space are pretty intuitive: an `exec` replaces the process image so therefore the text, stack, heap and data segments are overwritten.  A `fork` doesn't replace anything so they are left intact. But memory locks  are not preserved across a `fork`.  Why?

I had a fuzzy idea about what memory locks are used for. Michael describes in the notes section of `mlock` that there are two use-cases: You have a secret in memory that you don't want  the system to swap to disk or you have a real-time process and don't want to risk having delays introduced by paging. But `fork` prepares the address space for a copy-on-write operation. Any write that follows will cause a page fault. So I guess in the light of that, it makes sense to drop the memory locks since they're not obeyed by the system anyway.

## 18 May 2019

Neil Brown describes in [Ghosts of UNIX Past: a historical search for design patterns](https://lwn.net/Articles/411845/) how the UNIX operating system rests on two key ideas: a hierarchal file system incorporating demountable volumes and compatible file, device and inter-process I/O. He  mentions that Richie and Thompsson in their original paper pointed out that the UNIX system isn't bringing new inventions but rather, it fully exploits a few fertile ideas. Neil thinks that many of the shortcomings of the UNIX design comes from places where an idea wasn't fully exploited. The filesystem for devices is one of these shortcomings. It's a separate namespace that is not accessible from the filesystem. The top hierarchy would be the type  (block, character, etcera); the next level is the major id identifying the driver; then the minor id for blocks or identity of a controller. Various solutions to this shortcoming has been invented like devpts, udev, sysfs and devtmpfs but there are still rough edges.

## 17 May 2019

Martin Cracauer makes a case in [Proper Handling of SIGINT/SIGQUIT](https://www.cons.org/cracauer/sigint.html) for writing commandline programs that let the parent know if they've been killed by `SIGINT` or `SIGQUIT`. If they don't and the program is part of a shell script, then the shell don't know if they've run successfully or been aborted. Here's an example.

```
#! /bin/sh
# let's assume there are 300 *.dat files
for file in *.dat ; do
	dat2ascii $dat
done
```

If the shell wouldn't end when the user hits `^C`then he would have to press `^C` 300 times! The solution is to implement signal handlers to re-trigger the signal if they want to terminate the program.

```
void sigint_handler(void) {
    struct sigaction sa;
    sa.sa_handler = SIG_DFL;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    kill(getpid(), SIGINT);
}
```

The shell should call `WIFSIGNALED(status)` and `WTERMSIG(status)` to tell whether the child exited due to `SIGINT` or `SIGQUIT`.  If so, the script should be discontinued.

## 16 May 2019

While writing a function for translating `enum` values into strings, I started thinking about what types string literals have in C and C++. A function such as `f1` will compile in C but give a warning in C++. In C, string literals have type `char*` while in C++ they are `const char*`. You can add the `-Wwrite-strings` as an option to gcc to make it treat literals like C++. Const was added to C in C90. I assume that the types of string literals weren't changed to prevent too much code churn in legacy code bases due to [const poisoning](https://www.airs.com/blog/archives/428).

```
char* f1(size_t i) {
    char* a[] = { "s1", "s2" };
    return a[i];
}
```

CERTS [STR30-C Do not attempt to modify String Literals](https://wiki.sei.cmu.edu/confluence/display/c/STR30-C.+Do+not+attempt+to+modify+string+literals) says that modifying any portion of a string literal is undefined behavior. They are L-values in the sense that you can take the address of them, but you can't modify them. This function will  replace `'s`' with `'t'` in clang 8.0, but gcc 9.1 will ignore the assignment.

```
char* f1_mod(size_t i) {
    char* a[] = { "s1", "s2" };
    a[i][0] = 't'; // Undefined
    return a[i];
}
```

It's worth keeping in mind the distinction between string literals and char arrays. A statement such as `char buf[] = "abc"` means that the `"abc"` literal will be copied to the address of `buf`. So this code will return a stack address!

```
char* f2(size_t i) {
    char a[][3] = { "s1", "s2" };
    return a[i]; // BUG!
}
```

Here's a  [Compiler Explorer project that experiments with string literals](https://godbolt.org/z/BbNUgs)

## 15 May 2019

Downloading photos from my Samsung S7 phone to my Ubuntu 18.10 laptop using Nautilus or Shotwell stalls indefinitely if any of the photos contains parenthesis, [here's an Askubuntu post reporting the same findings](https://askubuntu.com/questions/995383/nautilus-hangs-on-accessing-dcim-camera-on-android). Renaming or removing the files with parenthesis fixed the problem for me.

I also noticed that downloading files using Shotwell by importing from the DCIM/Camera directory misses many files that are downloaded when using Shotwells "Importing from Camera" action.

My end goal was to create a photo book using [Fujifilms online Photobook creator](https://myfujifilm.se/). For that I needed a directory of the selected photos. I obtained such by first tagging the wanted photos using `Ctrl-T` in Shotwell. I then ran this small shell script. 

```
srcdir=$1
dstdir=$2
tag=$3

find "$srcdir" -name "*.jpg" | while read fname ; do
    if identify -verbose $fname | grep --quiet "$tag" ; then
        echo $fname
        cp $fname "$dstdir"
    fi
    
done
```

The identity program is part of the ImageMagick suite which was already installed. It was very slow (each file took more than 2 seconds to inspect), so I should look into writing a small program or installing something like exiftool instead.

## 14 May 2019

Red Hats article [Recommended Compiler and Linker Flags for GCC](https://developers.redhat.com/blog/2018/03/21/compiler-and-linker-flags-gcc/) mentions `-D_GLIBCXX_ASSERTIONS` that enabled non-intrusive assertions into the standard C++ containers. It does not have the performance penalty as enabling `-D_GLIBCXX_DEBUG`. Using STL in non-optimized builds is often a chore; without inlining the library is unbearable slow. Extra assertions tends to amplify the problem. I wonder what the performance cost of `_GLIBCXX_ASSERTIONS` are?

## 13 May 2019

Rachel Kroll demonstrates in [Multithreaded Forking and Environment Access Locks](https://rachelbythebay.com/w/2014/08/16/forkenv/) how locks held by threads can cause deadlocks after a fork. Here's a small program she wrote to demonstrate the problem.

```
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
 
static void* worker(void* arg) {
  pthread_detach(pthread_self());
 
  for (;;) {
    setenv("foo", "bar", 1);
    usleep(100);
  }
 
  return NULL;
}
 
static void sigalrm(int sig) {
  char a = 'a';
  write(fileno(stderr), &a, 1);
}
 
int main() {
  pthread_t setenv_thread;
  pthread_create(&setenv_thread, NULL, worker, 0);
 
  for (;;) {
    pid_t pid = fork();
 
    if (pid == 0) {
      signal(SIGALRM, sigalrm);
      alarm(1);
 
      unsetenv("bar");
      exit(0);
    }
 
    wait3(NULL, WNOHANG, NULL);
    usleep(2500);
  }
 
  return 0;
}
```

The worker thread acquires a lock which is never released by the subprocess since the thread holding the lock is not forked - remember, a fork only duplicates the running thread.

## 12 May 2019

Tudor Bosman describes how threads were introduced after fork in [Why Threads and Fork dont mix](https://sigsegv.me/2013/12/02/why-threads-and-fork-dont-mix/). He mentions that Solaris originally had both a `fork1` and a `forkall` system call, but the idea of forking more than one thread was soon abandoned: if there's a thread that's writing to a socket and it gets forked, then now there's a thread in the parent and one in the child writing to the socket. Data will be written twice! Bad.

Another bad thing is if another thread in the parent holds a mutex. No one will be around to release it in the child, so if the child needs to call a function that needs to acquire the same lock - the child will deadlock.

A library can call `pthread_atfork` but then all libraries must do that. And before the fork you need to acquire all mutexes in `pthread_atfork` handlers and hold them across the fork. But in what order should those mutexes be held?

## 11 May 2019

Theo De Raadts [Exploit Migration Techniques](https://www.openbsd.org/papers/ru13-deraadt/index.html) slides describes how OpenBSD added random stack gaps; W^X address policies; ASLR; stack protectors. They were first and the rest followed (atleast according to Theo). 

## 3 May 2019

Today I read Neil Browns LWN article [Ghosts of UNIX past part three: Unfixable designs](https://lwn.net/Articles/414618/) where he points out that the original UNIX signal design had two problems: when should a signal handler be re-armed and what should a system call do if it was interrupted by a signal? The set of signals grew somewhat uncontrolled and the semantics of their delivery remained relatively unspecified. The `sigaction` system call was introduced to allow more control over signal handlers; `pselect` and `ppoll` where created to overcome the race condition commonly fixed with the self-pipe trick; finally `signalfd` was added to make signal delivery synchronous.

## 29 Mars 2019

Rob Pike has written a blog post [The Byte Order Fallacy](https://commandcenter.blogspot.com/2012/04/byte-order-fallacy.html) where he says that you don't need to know the byte order of the computer the program is running on when decoding or encoding a byte stream. What does matter is the byte order of the stream you're decoding. So to decode a 32 bit integer for a little-endian stream you would write:

```
uint32_t val = (a[0] << 0) | (a[1] << 8) | (a[2] << 16) | (a[3] << 24);
```

And to decode a big-endian stream you would write:

```
uint32_t val = (a[3] << 0) | (a[2] << 8) | (a[1] << 16) | (a[0] << 24);
```

## 28 Mars 2019

Here's a challenge from the blog post [The Ksplice Pointer Challenge](https://blogs.oracle.com/linux/the-ksplice-pointer-challenge-v2). What does this program print?

```
#include <stdio.h>
int main() {
  int x[5];
  printf("%p\n", x);
  printf("%p\n", x+1);
  printf("%p\n", &x);
  printf("%p\n", &x+1);
  return 0;
}
```

The first two are straightforward, `x` is an array that decays into a pointer and `x+1` is a pointer to memory located at `x + sizeof(int)`. But when we apply the `sizeof` or `&`-operator (address of) then there's no decay to pointer: `&x` means take the address of the array and `x+1` means take the address of the memory location that we get by incrementing the address of x by the `sizeof` the x array.

There's also a similar quirky behavior for how function designators (function names) decays into pointers to functions. Here's a snippet taken from the Stackoverflow question [Function Pointer and Memory Address in C](https://stackoverflow.com/questions/55396741/function-pointer-and-memory-adress-in-c):

```
#include <stdio.h>
void foo(int arg) { }
int main() {
    printf("%p\n", &foo);
    printf("%p\n", foo);
    printf("%p\n", *foo);
}
```

The function designator (the function name) always decays into a pointer unless the `sizeof` or `&` operator is applied to it. So `&foo` takes the address of the function; `foo` decays into a  pointer; and for `*foo`  the `*` is applied to a  pointer giving a function designator, which then again decays into a function pointer.

## 3 February 2019

Avery Pennarun has written the article [An Epic Treatise on Scheduling, Bug Tracking, and Triage](https://apenwarr.ca/log/20171213) which he presented at SRECon as [The Math behind Project Scheduling, Bug Tracking, and Triage](https://www.usenix.org/conference/srecon18europe/presentation/pennarun). The main conclusions from him:

*  People are bad at estimating numbers, but good at estimating ratios: It's better to express the effort needed to finish a product as a ratio of the time it took to do another project.
* Setting goals is fine but setting time limits leads to student syndrome.
* Restricted multi-tasking is important
* Increase the  value provided to customers: ship early

Steve Klabnik has written about [How to be an Opensource Gardener](https://words.steveklabnik.com/how-to-be-an-open-source-gardener) where he describes how he does triage for issues.

## 18 January 2019

Rachel Kroll writes in [strtime's alpha sorted man pages vs well meaning people](https://rachelbythebay.com/w/2018/04/20/iso/) that the libc function strftime may print the incorrect year with the `%G` parameter if the last week of the year is split with the next year.

## 17 January 2019

Same stats, different graphs. https://www.autodeskresearch.com/publications/samestats

https://github.com/tjwei/Animation-with-Identical-Statistics/blob/master/Animation%20with%20Identical%20Statistics.ipynb

TODO: finish

## 15 January 2019

When I've wanted to align pointers or scalars, I've used macros like these:

```
#define ALIGN_DOWN(val, a) ((val) & ~((a)-1))
#define ALIGN_DOWN_PTR(ptr, a) (void*)ALIGN_DOWN((uintptr_t)ptr, a)
```

I read the though [Seastars core/align.hh](https://github.com/scylladb/seastar/blob/master/include/seastar/core/align.hh) which contained the following templates:

```
template <typename T>
inline constexpr T align_down(T val, size_t align) {
    return val & ~(align-1);
}
template <typename T>
inline constexpr T* align_down(T *val, size_t align) {
	static_assert(sizeof(*val) == 1, "align byte pointers only");
    return reinterpret_cast<T*>(align_down(reinterpret_cast<uintptr_t>(val), align));
}
```

These conversions are typesafe at the expense of the compile time needed for instantiating the templates. I wonder why they only allow byte pointers as inputs?

## 14 January 2019

The C specification defines an abstract machine that says that at each sequence point, all side effects of previous evaluations shall be complete. But what is a side effect? Writing and reading to a file sure is, and writing to an reading from volatile memory is too. But what about reading and writing to global memory? John Regehr writes in the comment section of [Nine Ways to Break Your Systems Code using Volatile](https://blog.regehr.org/archives/28) that all optimizing compilers  treats writes and reads to global memory as a non-side-effect. So the compiler can rearrange those accesses relative to a volatile variable. A [simple example with a buffer and a ready flag](https://godbolt.org/z/gEruMY) does show the problem.

```
volatile int ready;
int message[100];

// In the generated assembly, ready is set before message is assigned
void reordered (int i) {
  message[i/10] = 42;
  ready = 1;
}

// With a compiler barrier, the correct sequence is generated.
void sequenced (int i) {
  message[i/10] = 42;
  asm(""::: "memory");
  ready = 1;
}
```

## 13 January 2019

The AMD64 ABI specification says that only the lower 48 bits of an address shall be used. Andy Wingo describes in [Value Representations in Javascript Implementations](https://wingolog.org/archives/2011/05/18/value-representation-in-javascript-implementations) how the Javascript engines JavascriptCore and SpiderMonkey uses those upper 16 bits for tagged pointers. But according to the StackOverflow post [Using the Extra 16 Bits in 64-bit Pointers](https://stackoverflow.com/questions/16198700/using-the-extra-16-bits-in-64-bit-pointers) those upper 16 bits must all be copies of bit 47 (in a manner akin to sign extension), or the processor will raise an exception. Here's an example that will trigger a segmentation fault.

```
#include <stdint.h>

int main(int argc, char *argv[]) {
    int *ptr = &argc;
    ptr = (int*)((uintptr_t)ptr | (1ULL << 48));
    return *ptr;
}
```

## 12 January 2019

Brendan Gregg points out that `strace` consumes a lot of system resources in his article [Strace Wow Much Syscall](http://www.brendangregg.com/blog/2014-05-11/strace-wow-much-syscall.html) and that `perf trace`, the newer tracing tool is way less taxing. Here I'm reproducing Brendans "upper-bound" test for system call overhead and comparing it to `perf trace`.

```
$ dd if=/dev/zero of=/dev/null bs=1 count=500k
512000+0 records in
512000+0 records out
512000 bytes (512 kB, 500 KiB) copied, 0.31733 s, 1.6 MB/s

$ strace -eaccept dd if=/dev/zero of=/dev/null bs=1 count=500k
512000+0 records in
512000+0 records out
512000 bytes (512 kB, 500 KiB) copied, 12.0646 s, 42.4 kB/s

$ sudo perf trace -eaccept dd if=/dev/zero of=/dev/null bs=1 count=500k
512000+0 records in
512000+0 records out
512000 bytes (512 kB, 500 KiB) copied, 0.513642 s, 997 kB/s
```

On my machine, `strace` runs 40x slower and `perf trace` is 1.6x slower. Brendan reports that his strace is 442x slower. I don't know why there's a 10x difference between his results and mine. The `perf trace` time looks great though, but when I tried tracing all system calls I noticed that many events are reported as lost.

```
$ sudo perf trace dd if=/dev/zero of=/dev/null bs=1 count=500k
...
LOST 23 events!                                                                         6.178 ( 0.010 ms): dd/27737 read(buf: 0x55f3d4cb7000, count: 1                                    ) ...                                                                           LOST 28 events!                                                                         6.188 ( 0.012 ms): dd/27737 read(buf: 0x55f3d4cb7000, count: 1                                    ) = 1                                                                           LOST 1 events!                                                                           6.188 ( 0.014 ms): dd/27737  ... [continued]: write()) = 1                               LOST 44 events!
LOST 47 events!                                                                         6.220 ( 0.019 ms): dd/27737 read(buf: 0x55f3d4cb7000, count: 1                                    ) ...                                                                           LOST 32 events! 
```

## 11 January 2019

I found a function Inside the logging component in [Googles Abseil C++ library](https://abseil.io/) that computes the `basename` of a path at compile time. It's a `constexpr` statement, something I was aware existed but had never used myself. A `constexpr` function can only contain a return statement so it needs to use recursion for any non-trivial logic. So we're trading compilation time for execution time, but I wonder how much is saved? The function is used for logging. Will the time savings matter, give that

```
constexpr const char* basename(const char* fname, int offset) {
  return offset == 0 || fname[offset - 1] == '/' || fname[offset - 1] == '\\'
             ? fname + offset
             : basename(fname, offset - 1);
}
```

## 8 January 2019

The number of binary search trees of size N can be calculated with the following function. It's neat that you don't have to actually create the trees. It reminds me of a Jon Bentley presentation where he reduced quicksort down to a recurrence relation for reasoning about the performance. He didn't even have to run the code to calculate the runtime!

```
int count_trees(size_t N) {
    if (N <= 1) return 1;
    int sum = 0;
    for (size_t i = 0; i < N; i++) {
        int left = count_trees(i);
        int right = count_trees(N-i-1);
        sum += left * right;
    }
    return sum;
}
```

I had an off-by-one bug where I recursed with `N-i` for the right subtree. That's the sort of bug that I write very often. I've done it for mergesort, quicksort, quickselect and other algorithms.

## 7 January 2019

The PowerPC instruction set for embedded systems, "Book E", has a Variable Length Encoding (VLE) extension. 16-bit instructions stats with `se_` and 32 bit instructions starts with `e_`.

I wrote some simple functions in [Compiler Explorer for experimenting with PowerPC code generation](https://godbolt.org/z/JWwis0). Some examples of simple simple PowerPC assembly. A return from a function is handled by `blr`, Branch to Link Register.

```
void empty() { }
```

```
empty:
	blr
```

The first parameter is passed in `r3` and the return value is stored in `r3` as well.

```
int return_param(int x) {
    return x;
}
```

```
return_param:
	blr
```

Constants are loaded with `li` Load Immediate.

```
int return_constant() {
    return 42;
}
```

```
return_constant:
	li r3, 42
	blr
```

The instructions are always 32 bits. Immediate values can only be 16 bit wide.

```
int return_large_constant() {
    return 0x1ffff;
}
```

```
return_large_constant:
	lis r3, 0x1
	ori r3, r3, 0xffff
```

When referencing globals, the constant must be loaded in two steps as well. The expression `X@ha` means high 16-bits sign-bits sign-extended and `X@l` means low 16 bits. The instruction `lis` means Load Immediate Shifted 16 bits and `lwz` is Load Word and Zero extend. 

```
int X;
int return_global() {
    return X;
}
```

```
return_global:
	lis r9, X@ha
	lwz r3, X@l(r9)
```

## 3 January 2019

You can avoid single stepping into the C++ standard library by adding [skip directives to your .gdbinit according to a blog post on reversed.top](https://reversed.top/2016-05-26/skipping-standard-library-in-gdb/)

```
skip -gfi /usr/include/c++/*/*/*
skip -gfi /usr/include/c++/*/*
skip -gfi /usr/include/c++/*
```

## 2 January 2019

The Modern C++ twitter debate about debuggers led me to the MSDN article [Debugging Tips and Tricks for C++ in Visual Studio](https://blogs.msdn.microsoft.com/vcblog/2016/07/11/debugging-tips-and-tricks-for-c-in-visual-studio/). I use QtCreator and its gdb-based debugger. It works fairly well but I wish I could have the some Visual Studio features such as: always see function return values; set next statement to be executed by just clicking on the line; make simple edits and just continue debugging without having to recompile; pinning data hover tips for iterative code paths;  easily inspecting how long each line took to execute from within the debugger.

## 1 January 2019

The GCC compiler provides many [Function attributes](https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#Common-Function-Attributes) that can improve performance and helps detect common bugs. The format string attribute is commonly used for `printf` or `scanf`-like functions but another way to provide variable number of arguments is by placing a sentinel at the end of the argument list, most commonly `NULL`. GCC provides a function attribute, `sentinel` for ensuring that the caller really has provided a NULL argument.

## 30 December 2018

Integer division in the C language rounds towards zero. So `3 / 2 == 1` and `-3 / 2 == -1`.  That isn't the only option, [Python for instance, has floor rounding](http://python-history.blogspot.com/2010/08/why-pythons-integer-division-floors.html). It's common to divide a factor by 2 and these are commonly optimized to use shifts. But the `sar` operation in x86 does not round towards zero. So for integer division we add 1 to the dividend if the number is negative.

```
int idiv2(int x) {
    return x / 2;
}

unsigned udiv2(unsigned x) {
    return x / 2;
}
```

Here's the assembly generated. Note how idiv2 has a `shr eax, 31` operation for extracting the sign bit. This pattern has been known for a long time, Mike Abrash mentions it in the chapter [Signed Division with Sar in the book Zen of Assembly](http://www.jagregory.com/abrash-zen-of-asm/#signed-division-with-sar)

```
idiv2:
        mov     eax, edi
        shr     eax, 31
        add     eax, edi
        sar     eax
        ret
udiv2:
        mov     eax, edi
        shr     eax
        ret
```

## 29 December 2018

Ever since I read K&R in my first university course, I've known that you have to be careful about parenthesizing parameter names for function macros in C. A definition like `#define SQUARE(x) x * x` will give the wrong answer for an expression like `SQUARE(1+2)` due to the higher precedence of the multiplication operator. But a case where you don't need to parenthesize parameter names is inside function calls, like `#define SQUARE(x) square_helper(x, x)`.

You also have to take be mindful about parenthesizing the whole replacement list. The improved  `#define SQUARE(x) (x) * (x)` definition still fails for `6 / SQUARE(1+2)`. One thing that has bitten me in the past is the fact that  C does not have negative literals. So `-1` is `1` with the `-` operator applied to it. It means that a macro definition like `#define EOF -1` could give unexpected results if used in an expression with operators of lower precedence.

Even `#define SQUARE(x) ((x) * (x))` will fail for `SQUARE(i++)` since i will be incremented twice. One solution is to rely on the [GNU specific typeof keyword](https://gcc.gnu.org/onlinedocs/gcc/Typeof.html) together with the equally [GNU specific compound statement extension](https://gcc.gnu.org/onlinedocs/gcc-2.95.3/gcc_4.html#SEC62). But you're probably better of, just using inlined functions.

```
#define SQUARE(x) ({typeof (x) _x = (x); _x;})
```

## 28 December 2018

If you have a buffer with some meta-data in front, it's convenient to declare the buffer as a flexible array member of a header struct. In that way you only need one allocation for buffer and header.

```
typedef struct BufHdr {
  size_t len;
  size_t cap;
  uint64_t data[];
} BufHdr;
```

When you allocate such a buffer, you might calculate the size as the sum of the header-size and the total size of the array.

```
BufHdr *hdr = malloc(sizeof(*hdr) + num_elems * elem_size);
```

But that might allocate some more bytes than needed, due to alignment. Use `offsetof` instead.

```
BufHdr *hdr = malloc(offsetof(BufHdr, data) + N * sizeof
```

## 27 December 2018

Iterate over a bitmap can be done using the find first set bit function. I thought that you had to use a compiler intrinsics or inline assembly for that but there's a `ffs` function in glibc.

## 26 December 2018

Stack canaries fs:28h TODO

## 19 December 2018

Per Vognsen made some personal comments about productivity in [Bitwise Day 7: More Order-Independent Declarations](https://bitwise.handmade.network/episode/bitwise/bitwise007/#5887). He said that he had improved substantially over the past few years and that progress was due to finding ways to get "unstuck". Here's a transcript:

> [...]I feel like it's a pretty normal middle-stage for a lot of programmers, because when you start out, regardless of how natively smart or whatever you are, you really don't have good taste. So it's very easy to just blast out code and that's why you hear a lot of people who are very productive in say, their teens. But objectively they were terrible. But they didn't have the perception and the understanding that what they were doing was trash, but they were eventually gonna get stuck and not be able to do anything because they were so deep down a hole with their bad decisions. They were making a lot of decisions basically, but they were really bad decisions. And then at some point you grow self-awareness, through experience and what not, and you can easily become kinda paralyzed by thinking about everything that could be better and all that stuff. And hopefully you come out on the other side of that and you can be thoughtful but not completely paralyzed by your thoughtfulness.

## 18 December 2018

Jonathan Corbets LWN article [The problem with prefetch](https://lwn.net/Articles/444336/) describes how Andi Klee, Linus Torvalds and Ingo Molnar figured out that the prefetch directives for linked list traversal was actually hurting performance. The skbuff traversal routines, the linked lists scans for the hash tables and the for_each_x linked list macros used prefetch calls that all made the code slower.

```
#define list_for_each(pos, head) \
	for (pos = (head)->next; prefetch(pos->next), pos != (head); \
            pos = pos->next)
```

TODO: write about my own experiments

## 17 December 2018

Nick Parlante describes in his [Linked List Problems](http://cslibrary.stanford.edu/105/LinkedListProblems.pdf) document, 18 different "etudes" for practicing linked list manipulation. Exercise number 6 is about inserting a node into a sorted list. It requires us to handle: an empty list; insertion at first position; insertion somewhere in the middle of the list; and insertion at the end. My solution handles all four cases but requires three if-statements.

```
void sorted_insert(Node **head, Node *node) {
    if (*head == NULL) {
        *head = node;
        return;
    }
    Node *p;
    Node *prev = NULL;
    for (p = *head; p != NULL; p = p->next) {
        if (p->val >= node->val) {
            break;
        }
        prev = p;
    }
    if (prev == NULL) {
        node->next = *head;
        *head = node;
    } else {
        node->next = prev->next;
        prev->next = node;
    }
}
```

The `if-else` statement at the end is needed for differentiating between insertion at the beginning and insertion elsewhere. Nicks first solution instead checked for both an empty list and insertion at the beginning, at the top of the function.

```
void sorted_insert2(Node **head, Node *node) {
    if (*head == NULL || (*head)->val >= node->val) {
        node->next = *head;
        *head = node;
        return;
    }
    Node *p;
    for (p = *head; p->next != NULL; p = p->next) {
        if (p->next->val >= node->val) {
            break;
        }
    }
    node->next = p->next;
    p->next = node;
}
```

His solution got rid of the `if-else` statement and the `prev` pointer. But we can do even better if we introduce a dummy node. 

```
void sorted_insert3(Node **head, Node *node) {
    Node dummy = {.next = *head};

    Node *p;
    for (p = &dummy; p->next != NULL; p = p->next) {
        if (p->next->val >= node->val) {
            break;
        }
    }
    node->next = p->next;
    p->next = node;
    *head = dummy.next;
}
```

My original function had 19 lines of code in the body. `sorted_insert3` has 10 lines. Can we do even better? Ye we can, if we iterate using a pointer to a pointer.

```
void sorted_insert4(Node **head, Node *node) {
    Node **p;
    for (pp = head; *pp != NULL; pp = &((*pp)->next)) {
        if ((*pp)->val >= node->val) {
            break;
        }
    }
    node->next = *pp;
    *pp = node;
}
```

This latest version is a variation on Linus Torvalds list-removal code that I wrote about yesterday.

## 16 December 2018

Linus Torvalds wrote in the  Slashdot interview [Linus Torvalds answers all your Questions](https://meta.slashdot.org/story/12/10/11/0030249/linus-torvalds-answers-your-questions)  how he feels that many people don't understand pointers.

> [...] I've seen too many people who delete a singly-linked list entry by keeping track of the "prev" entry, and then to delete the entry [...]. People who understand pointers just use a "pointer to the entry pointer", and initialize that with the address of the list_head. And then as they traverse the list, they can remove the entry without using any conditionals, by just doing a "*pp = entry->next".

The bad way of doing a list remove-function would be:

```
void remove(Node **head, int val) {
    Node *prev = NULL;
    for (Node *p = *head; p != NULL; p = p->next) {
        if (p->val == val) {
            if (prev) {
                prev->next = p->next;
            } else {
                *head = p->next;
            }
        }
    }
}
```

It needs to differentiate between removing an element at the front or somewhere else in the list. Torvalds solutions is leaner. The `pp` pointer points to head at the beginning, on the next iterations it points to the `next` field of the previous node. So we inspect the current value, but still have access to `prev->next`. Very clever!

```
void remove2(Node **head, int val) {
    for (Node **pp = head; *pp != NULL; pp = &(*pp)->next) {
        if ((*pp)->val == val) {
            *pp = (*pp)->next;
        }
    }
}
```

Note that neither if these functions actually `free` the deleted node. If the node should instead be detached, it's next field should be set to `NULL`.

## 14 December 2018

In C and C++, arrays passed as parameters decays to pointers. That is a problem if you wish to pass a multidimensional array to a function. Here are the solutions I know of.

If the size of the rows and columns are known globally, then you can declare the parameter as pointer to array of columns

```
enum {NROWS = 16, NCOLS = 16 }

void f(int *array[NCOLS]) {
    //
    int x = array[row][col];
}
```

If the size is not known, then you can pass a pointer to a pointer together with `num_rows` and `num_cols` parameters. But then you have to do the address calculations manually.

```
void f(int **array, size_t n, size_t m) {
    //
    int x = array[row * n + col]
}
```

In C++ you can use a reference to array parameter.

```
enum {NROWS = 16, NCOLS = 16 }
void f(int (&array)[NROWS][NCOLS]) {
    // 
    int x = array[row][col];
}
```

If the size of the rows and columns is not known globally, then you can use a template function.

```
template <size_t NROWS, size_t NCOLS>
void f(int (&array)[NROWS][NCOLS]) {
    //
    int x = array[row][col];
}
```

In C99, you can specify the size of the array as a parameter, but it must precede the array parameter.

```
void f(size_t len, int *array[len]) {
    //
    int x = array[row][col];
}
```

## 27 November 2018

According to Daniel Miesslers blogpost [The Craziest Thing You'll Ever Learn About Pi](http://danielmiessler.com/blog/the-craziest-thing-youll-ever-learn-about-pi) ,pi calculated out to only 39 decimal places would allow one to compute the circumference of the entire universe to the accuracy of less than the diameter of a hydrogen atom. 16 decimal places (approximately what you get with a `double`) should be enough to calculate the diameter of the Solar System with the error less than a hair-width

## 20 November 2018

Brian Canthrill warmly recommends rust in [Falling in Love with Rust](http://dtrace.org/blogs/bmc/2018/09/18/falling-in-love-with-rust/). My own feelings about the language are a bit mixed, but nevertheless I've decided to give it a go. Brian suggest that you start with the exercises in the code repository [Rust Koans](https://github.com/crazymykl/rust-koans) so I did. It consists of unittests and it's your task to make them pass. They give introductions to integers, strings, arrays, vectors, traits, structs and borrowing. Some first observations

* Having strict definitions for integer overflow looks useful.
* The Vec struct has way too many functions for my taste and they deviate from the STL naming convention.
* 



## 19 November 2018

I read about [Load-Hit-Store](https://en.wikipedia.org/wiki/Load-Hit-Store) delays today: The Wikipedia page says that reading from a  memory address that has just been the target of a store operation, may stall the pipeline until the store has been finished. I had a misconception here: I believed that the value that was placed in the store queue would be accessible to the cache before the store had been committed but that was not the case. Or, it can be - but it requires store-forwarding which is complicated since the processor must search the store queue for a certain memory address which can be unaligned on x86-processors.

## 14 November 2018

Per Vognsen used labels with small integer values in his assembler for the bitwise project. I had never seen that scheme before. Sinclair Target mentions in [The Source History of Cat](https://twobithistory.org/2018/11/12/cat.html) that Ken Thompsson used the same kind of labels for his assembler in Unix v7.

> Thompson added support for “temporary labels” using the digits 0 through 9. These are labels that can be reused throughout a program, thus being, according to the Unix Programmer’s Manual, “less taxing both on the imagination of the programmer and on the symbol space of the assembler.” From any given instruction, you can refer to the next or most recent temporary label `n` using `nf` and `nb` respectively. For example, if you have some code in a block labeled `1:`, you can jump back to that block from further down by using the instruction `jmp 1b`. (But you cannot jump *forward* to that block from above without using `jmp 1f` instead.)

## 13 November 2018

I stumbled across an issue for [replacing llvm::iplist with TaggedList](https://github.com/pytorch/glow/issues/1766) that Jakob Stoklund had written. I had never heard of tagged lists before. They're supposed to be a solution to the [Order Maintenance problem](https://en.wikipedia.org/wiki/Order-maintenance_problem). Jakobs code is based on Michael Bender, Richard Coles et.als. paper [Two Simplified Algorithms for Maintaining Order in a List](https://scholar.google.com/scholar?cluster=5225046542682967685&hl=en&as_sdt=0,5).

## 12 November 2018

The  ST Microelectronics Cortex-M MCUs are the most sold MCUs in the world. They're great, but I wish that there were readily available HAL-drivers of  higher code quality than the CubeMX generated software. There is a Low-Level API hidden within ST:s HAL drivers. Maybe that's worth reading up on? I've seen many references to it in the eevblog and reddit/r/embedded forums.

## 8 November 2018

Inline functions and template functions in C++ are marked as weak symbols by the linker.  They are defined in multiple translation units and at link-time, only one of the definitions are kept. If the symbols were not marked as weak, then there would be collisions.

```
inline void f() { }
void g() { }
template <typename T> void h() { }

void use() {
    f();
    g();
    h<int>();
}
```

Notice here how the inline function `f` and the template function `h` are marked as weak.

```
$ nm foo.o -C
0000000000000000 W f()
0000000000000000 T g()
0000000000000000 W void h<int>()
0000000000000007 T use()
```

## 5 November 2018

I read Edgar Dijkstras EWD 196: The structure of the 'THE'-multiprogramming system, which Wikipedia claims to be one of the earliest description of a layered design for a software system. Dijkstra describes how his group designed an operating system by dividing the code into layers that each only depended on the layer beneath it. This lead to code that was easier to verify, inspect and test in isolation. Dijkstra proposed that all software should be modularized in such a way that the set of allowed inputs were small enough to allow exhaustive testing. Now, that's clearly not feasible for most of the code I've read and written, but the idea of layering software has proved it's usefulness over the decades.

I wonder if there are more articles about software design in a similar vein? C.A.R. Hoare has written about his experiences working as a manager for a software project that drowned in it's own complexity in [The Emperors Old Clothes](http://zoo.cs.yale.edu/classes/cs422/2010/bib/hoare81emperor.pdf) which provides some insights about how we always must manage complexity. Niklaus Wirths article [A Plea for Lean Software](https://cr.yp.to/bib/1995/wirth.pdf) is another interesting article about complexity.

## 29 October 2018

https://www.movable-type.co.uk/scripts/latlong.html

https://gis.stackexchange.com/questions/25494/how-accurate-is-approximating-the-earth-as-a-sphere#25580

http://www.movable-type.co.uk/scripts/latlong-os-gridref.html

http://www.movable-type.co.uk/scripts/latlong-vectors.html

http://www.movable-type.co.uk/scripts/latlong-vincenty.html

http://www.movable-type.co.uk/scripts/latlong-utm-mgrs.html

https://cs.nyu.edu/visual/home/proj/tiger/gisfaq.html

[DrMath on Deriving the Haversine formula](http://mathforum.org/library/drmath/view/51879.html)



## 25 October 2018

I use QtCreator as my IDE for all C++-projects at work. We have  a bunch of them, and I've been annoyed for a while about the hazzle of setting up `make -j` options and using ccache as wrapper for our cross-compilers.  Today i found out that each kit can have settings that applies to all projects that uses that kit. I've  set the build tool to ninja (since it autodectects the number of cores and compiles faster than make) and I've set `CMAKE_CXX_LAUNCHER=ccache`. Now things are blazingly fast!

Today I got surprised by a compile error when I tried to const-override a method for a typedef that I thought was an alias for a reference to the type. I read up and learned in the [Stackoverflow post Function overloading in C Const Pointers](https://stackoverflow.com/questions/37913596/function-overloading-in-c-const-pointers) outermost cv-qualifiers in a parameter declaration are ignored during overload resolution. The thing that tripped me up is how we interpret C declarations: `const int *` is a pointer to const int. So the const is embedded in the declaration, and therefore is significant. But `int * const` is a const pointer to int. There the const specifier is outermost, and should therefore be ignored.

```
void fun_ptr(int *x) { }
void fun_ptr(const int *x) { }

void fun_cptr(int *x) { }     
void fun_cptr(int * const x) { } // compile error

void fun_int(int x) { }
void fun_int(const int x) { } // compile error
```



## 24 October 2018

I attended the [Meet Qt Stockholm seminar](https://www.qt.io/events/meet-qt-stockholm--1537353592/)  today where representatives for the Qt company and KDAB presented the road-map and "insights into the latest trends in embedded development".

Vesa Kyllönen started with presenting a chart of the Qt company's growth. They've increased their revenue substantially over the last couple of years and the Qt software is used in lots of different industries.

Santtu Ahonen described how the touchscreen interfaces of smart-phones affects the user expectations on embedded devices. He notes that the Qt-based devices are often run with with less resources. They see a trend where Cortex-M processors are also being equipped with touch-screens. Some Cortex-M are even more powerful than Cortex-A processors. There's a lot of buzz around Augmented Reality (AR) and they have worked on a few projects involving that.

Kalle Dalheimer, CEO of KDAB, gave examples of how Qt is being used in the medicine and biotech industry. An inspiring talk with lots of advanced 3D visualizations.

Santtu continued after lunch with a presentation of the Qt road map. They've created a Qt Studio tool that will morph with the Qt 3d Studio tool. The LTS releases span 3 years, but longer support contracts can be bought. There are plans for further "componentizing" the Qt distribution in order to allow even more resource-constrained systems to be targets.

Tino Pyssysalo rounded up the day by a demonstration of the integration between Photoshop and Qt Studio.

## 23 October 2018

I was trying to explain a C++ linking error today at work and came up short. After reading up, I realized that it was due to how name lookup differs for qualified and unqualified names.  Here is a minimal example of my problem. What is the exported symbol for `x`?

```
namespace outer { extern int x; }
using namespace outer;
int x;
```

It's `x` and not `outer::x` as I had expected. This struct definition on the other hand has the expected exported symbol `outer::S::x`

```
namespace outer {  struct S { static int x; }; }
using namespace outer;
int S::x;
```

The difference lies in how qualified and unqualified name lookup is carried out. If the `int x` definition had used qualified name-lookup, then it would have gotten the expected type. An example that will export `outer::inner::x:`, the C++ syntax does not permit `int ::x:` so I had to add an inner namespace to demonstrate my point

```
namespace outer { namespace inner { extern int x; } }
using namespace outer;
int inner::x;
```

## 19 October 2018

[Mike Hommey writes in How to Waste a lot of Space Without Knowinng](https://glandium.org/blog/?p=2361) about the difference between how pointers to const and const pointers are allocated in ELF files. Consider these three declarations:

```
const char *ptr = "ptr to const";
const char * const cptr = "const ptr to const";
const char buf[] = "char array";
```

The `ptr` will live in .data while the content it points to will be placed in .rodata. But `cptr` and `buf` will both live in .rodata. That's a net save of 8 bytes on 64bit platform. 

Another source of space waste is the fact that external symbols occupy space in the symbol table (and therefore also occupies space in the string table, and the  gnu hash table). That can be avoided by declaring constants local to a file as static, and constants local to a library should be annotated with visibility=hidden.

## 9 September 2018

The Linux Input API (e.g. /dev/input/event0)  allows multiple processes to open the device and they can all read the input events. Multiple processes can open a tty device as well, but a process that calls `read` consumes the read bytes. This difference in design can be motivated by the different expectation models:

* The event subsystem is designed for unidirectional notification of simple events from multiple writers into the the system with very little (or no) configuration options
* The tty subsystem is intended for bidirectional end-to-end communication of potentially large amounts of data and provides a reasonable flexible (albeit fairly baroque) configuration mechanism.

The tty subsystem should be viewed as as a point-to-point link between moderately intelligent endpoints who will agree on what the data passing between them will look like. While there are circumstances where "single writer, multiple readers" would make sense (GPS receiver connected to a serial port, continually reporting its position for instance), that's not the main purpose of the system and those use cases can be easily implemented in user space.

Given this, it makes sense to only allow a single process to access a serial-port. I've noticed that `GNU screen` will complain if another instance  has already opened the device, but `cat` can access the device file just fine - How come? It's because `GNU screen`uses file locking, something I've hardly encountered so far. Chapter 55 File Locking of Michael Kerrisks Linux Programming Interface book gives a good introduction. Linux uses advisory file locking using the `flock` and `fcntl` system calls.

## 8 September 2018

A colleague was investigating a problem with a Linux Ethernet driver oscillating between reporting that a carrier signal was present and then not present. I started reading up on carrier detection and auto-negotiation and here's what I found: The Ethernet standard consists of many different standards that specify how the physical layer operates. Auto-negotiation works for all twisted-pair media (10Base-T, 100Base-TX, 1000Base-T, 10GBase-TX), but not for fiber optic. The negotiation only works for link segments; the initialization takes place prior to any other communication; and it uses its own signaling system. That signaling system is based upon the link integrity test pulse, which was developed for 10Base-T to help the Ethernet device at either end detect the presence of a communication failure. For faster Ethernet systems there's a continuous stream of signals even when there is no data being sent, which means the link pulse is not needed on faster systems.

The Auto-Negotiation system uses a burst of the normal link pulse (NLP) signal which is called a fast link pulse (FLP). The last link pulse in a burst of fast link pulses is timed as to appear to older 10Base-T equipments as regular link integrity link pulses. The Auto-Negotiation system continuously monitors the link status, and will notice if a link goes down and comes back up (as when disconnecting a patch cord to a station). Once a link comes back up, the Auto-Negotiation process will start again from the beginning.

## 14 September 2018

The `ifconfig` tool only shows interfaces that are up, while  `ip link` shows every interface. That tricked me for a while. You can use `ifconfig -a` to list every interface. On recent Linux systems, the `ifup`/`ifdown` scripts has been replaced with `systemd-networkd`. I 

## 13 September 2018

Today while porting a WiFi driver to a new board, I had trouble with the firmware loading and had to read [The Linux Kernel docs for the Linux Firmware API](https://www.kernel.org/doc/html/v4.17/driver-api/firmware/index.html). I was under the impression that every firmware load was triggered by an udev event, but that functionality was removed from the kernel back in 2014. Instead, the driver makes a call through the `request_firmware` function which takes a path as argument and return a firmware object. Internally it calls `kernel_read_file_from_path` which reads the file from the root file-system.

The firmware loading may be subject to races with the mounting of the file system that the firmware file resides on. These races are particularly common when the kernel resumes from suspend. To work around these, a firmware cache is placed behind the `request_firmware` function.

An alternative to placing the firmware file under /lib/firmware is to include it into the kernel, thereby speeding up the loading phase at the expense of a larger kernel image. If the driver that requires the firmware is built into the kernel, then you should place the firmware image in the initramfs as well, to avoid races with root file-system mounting during upstart.

## 12 September 2018

I had built an image using the Yocto build system but when I booted it, a USB module was not detected by a driver I had assumed had been built. How do you figure out if a certain driver has been built or not? Either you search the `/lib/modules/$(uname -r)/` directory if it was built as a module. Or you can search in the `/lib/modules/$(uname -r)/modules.builtin` file. But the easiest way is probably to use `modinfo` which does both for you. One thing you can not rely on is the content of `/sys/module/`: only standalone modules or builtin modules that takes parameters are listed there.

## 5 September 2018

According to the Unix&Linux StackExchange post [USB C to Displayport driver support](https://unix.stackexchange.com/questions/403318/usb-c-%E2%86%92-displayport-adapter-support), I'll have to wait until kernel 4.19 before the kernel can support my Displayport to USB C adapter. The good news is that the driver will support every cable present on the market.

## 4 September 2018

When I've  edited files whose names was piped to  `vim` using `find -name pattern | xargs vim` , the terminal has gotten borked afterwards. Today I learned from the Stackexchange question [Invoking vi through xargs - vi breaks my terminal, why](https://superuser.com/questions/336016/invoking-vi-through-find-xargs-breaks-my-terminal-why) that when you invoke a program via `xargs`, the programs stdin points to `/dev/null` (since xargs doesn't know the original stdin, it does the next best thing).  Vim reads the tty settings from `/dev/null` and later applies those to its controlling terminal when exiting. A solution to this problem is to invoke `xargs` with the `-o, --open-tty` option, which will reopen `/dev/tty` as stdin.

## 29 August 2018

Today a colleague asked why a `struct` containing five `uint8_t` members occupied 8 bytes instead of 5 bytes. For code involving structures, the compiler may need to insert gaps in the field allocation to ensure that  each structure member satisfies its alignment requirement. But `uint8_t` can be aligned on each byte boundary. What's going on?

The thing to know about bit-fields is that they are implemented with word- and byte-level mask and rotate instructions operating on machine words, and cannot cross word boundaries. The compiler will try to pack the bit-fields as tightly as possible, but it won't cross word boundaries

```
struct S {
  char c[3];
  char c4 :5;
  char c5 :5;
  char c6 :5;
};

int main(int argc, char *argv[]) {
  return sizeof(struct S);
}
```

Clangs diagnostic command shows this layout on my Linux x64 laptop. Note that the size is 6 instead of 5 bytes.

```
*** Dumping AST Record Layout
         0 | struct S
         0 |   char [3] c
     3:0-4 |   char c4
     4:0-4 |   char c5
     5:0-4 |   char c6
           | [sizeof=6, align=1]
```

But that's not the whole story. The compiler we use added padding to structs containing bitfields so that the total size woul be a multiple of the wordsize.

## 27 August 2018

I was trying to boot a board from  a regular Debian image at work today and the kernel got blocked on MMC-readings. In the logs there were lines with the text `INFO: task kworker/3:1:79 blocked for more than 120 seconds`, followed by a back-trace for the specific kernel thread. I've seen those messages in the past for different reasons. Now I got curious about which part of the kernel is responsible for printing them.

The file [kernel/hung_task.c](https://elixir.bootlin.com/linux/latest/source/kernel/hung_task.c) creates the `khungtaskd` thread which gets scheduled every 120 seconds and checks if each task that is in interruptible sleep has been scheduled for the last 120 seconds. If it hasn't, then `khungtaskd` dumps the hung tasks call stack. This kernel code is easy to explain, but there's still a couple of details that are fiddle, such as guaranteeing an upper bound on the number of tasks that are traversed and ensuring that the RCU lock is not held for too long, and updating the timeout values in an atomic fashion.

## 23 August 2018

I tried to solve the [Chain Matrix Multiplication problem](https://en.wikipedia.org/wiki/Matrix_chain_multiplication) and wow that was a challenge for me. The earlier DP problems I've solved has involved linear recursion in one or two dimensions, but this problem requires tree recursion. But just understanding matrix multiplication was a chore. I've never taken a linear algebra course and it shows. [Kalid Azads article An Intuitive Guide to Linear Algebra](https://betterexplained.com/articles/linear-algebra-guide/) explains matrix multiplication as that a matrix can either consist of data (organized in columns) or operations (organized in rows). When we multiply two matrices, he imagines it as if we're "pouring" the input data through the operations matrix to create the result. It's easier to understand if you see his illustrations.

The Chain Matrix Multiplication problem is about placing parenthesis for optimizing the order in which we perform the calculations. Those parenthesized expressions makes up a tree and the objective is to construct an optimal tree from subtrees.

```
@memo
def chain_matrix_mult(M):
    def C(i, j):
        if i == j: return 0
        return min([C(i, k) + C(k+1,j) + M[i-1] * M[k] * M[j] for k in range(i, j)])
    return C(1, len(M)-1)
```

## 22 August 2018

Dynamic programming solutions often has two dimensional arrays and the expressions gets hard to read with all the nesting of the bracket operators, `table[i][j]=table[i][j-1]`. [Numpy](http://www.numpy.org/) arrays allows me to use two indexes inside the brackets which cleans up the the code, `table[i, j] = table[i, j-1]`.

```
import numpy as np
def Table(num_rows, num_cols):
	return np.zeros((num_rows, num_cols), dtype=np.int)
```

Here's a solution for  Knapsack without repetitions.

```
def knapsack(items, W):
	N = len(items)
	K = Table(N+1, W+1)
	for  i in range(1, W+1):
		for j in range(1, N+1):
			v, w = items[i-1]
			if w > W:
				K[i, j] = K[i, j-1]
			else:
				K[i, j] = max(K[i, j-1], v + K[i-w, j-1])
	return K[W, N]
```

## 21 August 2018

Dasgupta et.al. writes that every dynamic programming problem has an underlying DAG structure: each node represents a subproblem and each edge represents a precedence constraint on the order in which the subproblems can be tackled. I don't know if that will help me in the  future when I try to solve DP problems, but I guess it's worth to keep in mind. I solved the Knapsack with Repetition problem. You're allowed to take several items of the same kind. You can solve that problem by recursing on the max allowed weight `Ẁ`.

```
@memo
def knapsack(values, weights, W):
	if W == 0: return 0
	return max([v + knapsack(values, weights, W - w) 
		for v, w in zip(values, weights) if w <= W] or [0])
```

 It took some head scratching before I had convinced myself that the algorithm is correct. But once I had done that and had written the top-down version, then writing the bottom-up code was easy. I feel like I'm finally making progress. Yay!

```
def knapsack(values, weights, W): 
	K = [0] * (W+1)
	for i in range(1, W+1):
	K[i] = max([v + K[i-w] for v, w in zip(values, weights) if w <= i] or [K[i-1]])
	return K[W]
```

## 18 August 2018

Brian Deans [Dynamic Programming Practice Problems](https://people.cs.clemson.edu/~bcdean/dp_practice/) contains descriptions of a couple of common DP problems and has short recordings which shows Brian solving them using intuitive table descriptions. Might be worth investigating. By now I've spent a little too much time trying to get an orientation on what dynamic programming really is. The central concepts are recursive definitions and the tables with data flows. I probably just need to work through the examples in CRLS and Dasgupta to get a good grasp on those fundamentals.

## 17 August 2018

Edward Z. Yang writes in [DP Zoo Tour](http://blog.ezyang.com/2010/11/dp-zoo-tour/) that you can almost completely characterize a dynamic programming algorithm by the shape of its table and how the data flows from one cell to another. He has collected a bunch of algorithms from Introduction to Algorithms and drawn up the tables and data flows. I especially liked the arrow notation he used for describing data flow.  The Longest Increasing SubSequence problem (LIS) and knapsack with repepetion has dataflow dependencies on all previous cells. Just seeing the arrows made it much easier for me to reason about those.

## 16 August 2018

Steven Skiena writes that learning Dynamic Programming (DP) requires that you see  a lot of solved exercises before "it clicks" and you understand how to do them yourself. Well, there's been no "clicking" for me yet! 

The [Longest Increasing Subsequence problem](https://en.wikipedia.org/wiki/Longest_increasing_subsequence) (LIS) is one of the first DP problems that one gets introduced to. It can be viewed as finding the longest path through the directed graph of all permissible transitions! I wrote a recursive version that recurses on both `i` and `j` . I tried replacing the `j` recursion with a for loop but failed. I've noticed that even though I have  a recursive relation for a sequence, I'm still having trouble with implementing it in code.

```
def lis(X):
	@memo
	def _lis(i, j):
		if i == 0: return 1
		if j < 0: return 0
		m = 1 + _lis(i-1, i-2) if X[i] > X[j] else 0
		return max(m, _lis(i, j-1))
	return _lis(len(X)-1, len(X)-2)
```

The recursive version recurses from right to left. I then wrote a bottom-up solution that goes from left to right. Many of the DP problems assumes there's an inherent left-to-right ordering. For bottom-up you have to figure out what the search space is. For LIS, it's a table of i * j but here we only store the max of each column of that table in `L`.

```
def lis(X):
	L = [0] * len(X)
	for i, x in enumerate(X):
	L[i] = max([y for j, y in enumerate(L[:i]) if X[j] < x] + [0]) + 1
	return max(L)
```

The [Longest Common Subsequence problem](https://en.wikipedia.org/wiki/Longest_common_subsequence_problem) (LCS) is a problem I've wanted to understand for a long time. I've written patch and diff programs in the past but they've relied on already implemented LCS algorithms. The recursive relation can be easily written in python.

```
@memo
def lcs(X, Y):
	if len(X) == 0 or len(Y) == 0:
		return 0
	if X[0] == Y[0]:
		return 1 + lcs(X[1:], Y[1:])
	return max(lcs(X[1:], Y), lcs(X, Y[1:]))
```

The iterative version can be done with a two dimensional table just like the LIS solution. Each coordinate (i, j) in that table represents the LCS for `X[:i]` and `Y[i:]`. 

```
def lcs(X, Y):
	N, M = len(X), len(Y)
	L = [[0] * (M+1) for _ in range(N+1)]
	for i in range(N+1):
		for j in range(M+1):
			if i == 0 or j == 0:
				L[i][j] = 0
			elif X[i-1] == Y[j-1]:
				L[i][j] = L[i-1][j-1] + 1
			else:
				L[i][j] = max(L[i-1][j], L[i][j-1])
	return L[N][M]
```

## 14 August 2018

The [Discrete Optimization](https://www.coursera.org/learn/discrete-optimization) course covers [dynamic programming](https://en.wikipedia.org/wiki/Dynamic_programming), [constraint programming](https://en.wikipedia.org/wiki/Constraint_programming), [linear programming](https://en.wikipedia.org/wiki/Linear_programming) and and [local search](https://en.wikipedia.org/wiki/Local_search_%28optimization%29). The lectures are somewhat chatty and unorganized. I'll probably need to read some books while going through the material.

Dynamic programming is covered in all my algorithm books: CRLS, Skiena, Dasgupta and Kleinberg&Tardos. Linear programming is covered in CRLS, Dasgupta, Tardos and on a high-level in Skiena. There is a local search chapter in Kleinberg&Tardos. For constraint programming the best resource I've found so far is Roman Bartaks [Online Guide to Constraint Programming](http://kti.ms.mff.cuni.cz/~bartak/constraints/index.html) and Peter Norvigs book [Artifical Intelligense a Modern Approach](http://aima.cs.berkeley.edu/).

David P. Williamsson and David B. Shmoys book [The Design of Approximation Algorithms](http://www.designofapproxalgs.com/) seems to cover a lot of the material for the course as well.

## 12 August 2018

While doing some dynamic programming exercises involving the calculation of [Bell numbers](https://en.wikipedia.org/wiki/Bell_number) and [Binomial coefficients](https://en.wikipedia.org/wiki/Binomial_coefficient) I noticed that the recursive definitions  for Stirling numbers and Binomial coefficients very much resemble each other. Here is the definition for Stirling numbers:

```
S(n, k) = S(n-1, k-1) + k * S(n-1, k)
S(n, 1) = S(n, n) = 1
```

`S(n, 1)` means putting n items in one pile. That can only be done in one way, and `S(n, n)` means putting n items on n piles. That can also only be done in one way. If `1 < k <n` then we can either create a new pile `S(n-1, k-1)` or put the item on one of the existing piles `k * S(n-1, k)`.

And here is the definition for Binomial Coefficients:

```
C(n, k) = C(n-1, k-1) + C(n-1, k)
C(n, 0) = C(n, n) = 1
```

## 11 August 2018

Today in the [Discrete Optimization](https://www.coursera.org/learn/discrete-optimization) course I learned about [dynamic programming](https://en.wikipedia.org/wiki/Dynamic_programming) (DP).  A problem must have overlapping subproblems and optimal subproblems must lead to the optimal solution in order for dynamic programming to be applicable. They can either be solved via a bottom-up or a top-down approach. Let's solve the most typical DP problem: Fibonacci numbers. They can be calculated using a  slow recursive algorithm. We can speed it up by adding memoization to avoid recomputing the overlapping subproblems.

```
def fib_naive(n):
	if n < 2:
		return n
	return fib_naive(n-1) + fib_naive(n-2)

@memo
def fib_memo(n):
	if n < 2:
		return n
	return fib_memo(n-1) + fib_memo(n-2)
```

Even with memoization there's still a lot of  recursive calls. If we use a bottom-up approach we can get rid of the recursion.

```
def fib_tabular(n):
	table = [0] * (n+1)
	table[1] = 1
	for i in range(2, n+1):
		table[i] = table[i-1] + table[i-2]
	return table[n]
```

One thing to keep an eye for with dynamic programming is the size of the tables required. Here we can see that we only need the two previous Fibonacci numbers to calculate the next one.

```
def fib_tabular_optimized(n):
	a, b = 0, 1
	for _ in range(2, n+1):
		a, b = b, a + b
	return b
```

Figuring out how to express the problem as a recurrence relation appears to be the hardest step in solving a DP problem. Nikola Otasevic has a good explanation of that thought process in [Dynamic Programming - 7 steps to solve any DP interview problem](http://blog.refdash.com/dynamic-programming-tutorial-example/).

## 10 August 2018

I signed up for [Pascal Van Henterycks Coursera course Discrete Optimization](https://www.coursera.org/learn/discrete-optimizationhttps://www.coursera.org/learn/discrete-optimization).  He started with a motivating example where Indiana Jones was trying to fill a knapsack with precious art items. Using a greedy algorithm, I can select the items by max value, max weight or by max value-weight density. But none of those three approaches led to the optimal solution. That was an eye opener. 

```
items_by_worth = sorted(items, key=lambda x: x.value, reverse=True)
items_by_weight = sorted(items, key=lambda x: x.weight)
items_by_density = sorted(items, key=lambda x: x.value/x.weight, reverse=True)

def fill_knapsack(seq, capacity=10):
	weight = 0
	sack = []
	for elt in seq:
	if weight + elt.weight <= capacity:
		sack.append(elt)
		weight += elt.weight
	return sack, sum(x.value for x in sack)
```

Pascal said that you should start with a greedy approach. It's often easy to code, but seldom closest to the optimal solution. I can always find the optimal solution using brute force, but that has exponential running time.

```
max((fill_knapsack(x) for x in permutations(items)), key=lambda x: x[1])
```

## 6 July 2018

I watched [Bitwise Day 43, Domain Specific Languages in Python](https://bitwise.handmade.network/episode/bitwise/bitwise043/). There Per describes how you can create an embedded DSL inside Python. I learned that many things can be overloaded in Python, but the logical operators cannot. If you call `or`, `and` or `not` on an object then it will convert that object to a boolean by calling `__bool__`. If that operator is not defined then it uses `__len__` and if that is not implemented it converts the object to `True`.

```
class Node:
   def __and__(self, other):
		return Node()
	def __bool__(self):
		raise TypeError("Cannot treat nodes as logical values")
x, y = Node(), Node()
z = x & y
z = x and y
```

Another thing is that Python provides special operators for the reverse ordering of arguments. So `x & 1` will work for the code above, but `1 & x` will raise a `TypeError`. By adding a `__rand__` overload, the code will work as intended. What happens under the hood is that the int `__and__` operator gets called and throws and exception, that is caught by the runtime and it tries the left hand side `__rand__` operator instead.

A cool thing about Python is that everything is an object, even types are objects. Per uses that for his interpreter where he does a lookup on the type in a dictionary. Here's a simplified example of his code to show the idea:

```
evaluators = {
	int : lambda x : x,
	str : lambda x : len(x)
}
def evaluate(x): return evaluators[type(x)](x)
```

So `evaluate(42)` will return 42 and `evaluate("hello world")` will return 11.

## 5 July 2018

I read [Michael Nielsens essay Augmenting Long-Term Memory](http://augmentingcognition.com/ltm.html) wherein he describes his experiences of using a "Personal Memory System", here [the software Anki](https://apps.ankiweb.net/), which is a computerized flashcard program that automatically manages the review schedule. Michael mentions concepts that I recognize from taking [Barbara Oakleys online course Learning How to Learn](https://www.coursera.org/learn/learning-how-to-learn), like spaced repetition; the value in memorizing the basics; how different memories are triggered by small variations of cues; and chunking and making connections between chunks. What was new to me was Michaels view that Anki helps him to form better chunks by forcing him to create "atomic" question-answer pairs that are then easier to combine. He sees Anki as not just a system for remembering simple facts, but a way to build up arbitrary complex knowledge. He also describes how "Ankifying" information in a paper by doing several passes and write questions for things  he's learned has helped him to absorb information more quickly.

## 4 July 2018

Object layout for C++ classes can be inspected with the same clang options as I used yesterday. Here is a class hierarchy without inheritance:

```
class Base {
public:
  int x;
};

class Derived : public Base {
  public:
    int y;
};
```

Clang shows the following data layout for Derived:

```
*** Dumping AST Record Layout
         0 | class Derived
         0 |   class Base (base)
         0 |     int x
         4 |   int y
           | [sizeof=8, dsize=8, align=4,
           |  nvsize=8, nvalign=4]
```

If I add a virtual member function to `Base` and `Derived` then a vtable is inserted at the beginning of the object.

```
*** Dumping AST Record Layout
         0 | class Derived
         0 |   class Base (primary base)
         0 |     (Base vtable pointer)
         8 |     int x
        12 |   int y
           | [sizeof=16, dsize=16, align=8,
           |  nvsize=16, nvalign=8]
```

## 3 July 2018

I got confused about how alignment works in C and wrote this small experiment.

```
struct E { _Alignas(16) char c; };

struct P1 { int i; char c; int j; char d; };
struct P2 { int i; char c; char d; int j; };
struct P3 { short w[3]; char c[3]; };
struct P4 { short w[3]; char *c[3]; };
struct P5 { struct P1 a[2]; struct P2 *p; };
struct P6 { struct E a[3]; };

int main(int argc, char *argv[])
{
  int r = 0;
  r += sizeof(struct P1);
  r += sizeof(struct P2);
  r += sizeof(struct P3);
  r += sizeof(struct P4);
  r += sizeof(struct P5);
  r += sizeof(struct P5);
  r += sizeof(struct P6);

  return r;
}
```

Linux requires that a type of K bytes must be aligned on a K-byte boundary. For compound types, this means that padding may be required between members or after the last member. The `_Alignas` directive can be useful for dictating stricter alignment, for example when using DMA buffers.

Eli Bendersky shows in [Dumping a C++ objects memory layout with Clang](https://eli.thegreenplace.net/2012/12/17/dumping-a-c-objects-memory-layout-with-clang/) how clangs cc1 frontend has a -f command that can be used for inspecting the memory layout. Here is the output for the test cases above.  `P1` has tail padding added.  `P3.c` starts at offset 6, while `P4.c` starts at offset 8: the member with largest alignment dictates how the other members will be laid out.

```
$ clang -cc1 -fdump-record-layouts test.c
*** Dumping AST Record Layout
         0 | struct P1
         0 |   int i
         4 |   char c
         8 |   int j
        12 |   char d
           | [sizeof=16, align=4]

*** Dumping AST Record Layout
         0 | struct E
         0 |   char c
           | [sizeof=16, align=16]

*** Dumping AST Record Layout
         0 | struct P2
         0 |   int i
         4 |   char c
         5 |   char d
         8 |   int j
           | [sizeof=12, align=4]

*** Dumping AST Record Layout
         0 | struct P3
         0 |   short [3] w
         6 |   char [3] c
           | [sizeof=10, align=2]

*** Dumping AST Record Layout
         0 | struct P4
         0 |   short [3] w
         8 |   char *[3] c
           | [sizeof=32, align=8]

*** Dumping AST Record Layout
         0 | struct P5
         0 |   struct P1 [2] a
        32 |   struct P2 * p
           | [sizeof=40, align=8]

*** Dumping AST Record Layout
         0 | struct P6
         0 |   struct E [3] a
           | [sizeof=48, align=16]
```



## 16 June 2018

Simon Tatham describes in [Coroutines in C](https://www.chiark.greenend.org.uk/~sgtatham/coroutines.html) how a coroutine can be implemented by using a variant of Duffs Device. The limitations is that it can't yield from inside a called function and there can't be multiple active coroutines for the same function.

```
int function(void) {
    static int i, state = 0;
    switch (state) {
        case 0: /* start of function */
        for (i = 0; i < 10; i++) {
            state = 1; /* so we will come back to "case 1" */
            return i;
            case 1:; /* resume control straight after the return */
        }
    }
}
```

Adam Dunkels describes in [Protothreads under the hood](http://dunkels.com/adam/pt/expansion.html) how PT overcome the "only one active coroutine" limitation by having the state variable `pt` be a parameter. But it's still a stackless coroutine.

```
struct pt { unsigned short lc; };
#define PT_THREAD(name_args)  char name_args
#define PT_BEGIN(pt)          switch(pt->lc) { case 0:
#define PT_WAIT_UNTIL(pt, c)  pt->lc = __LINE__; case __LINE__: \
                              if(!(c)) return 0
#define PT_END(pt)            } pt->lc = 0; return 2
#define PT_INIT(pt)           pt->lc = 0
```
Here's an example coroutine:
```
static int counter = 0;
PT_THREAD(example(struct pt *pt)){
  PT_BEGIN(pt);
  while(1) {
    PT_WAIT_UNTIL(pt, counter == 1000);
    counter = 0;
  }
  PT_END(pt);
}
```

## 15 June 2018

I posted [a question to the Bitwise forum about dealing with inversion of control flow](https://bitwise.handmade.network/forums/t/3207-dealing_with_inversion_of_control_flow), where I expanded on my thoughts from yesterday. The distinction between granularity of concurrency; high level abstractions over control flow; and syntactic sugar over callbacks is something that I've been thinking about for a long time without really coming to any conclusions. 

Martin Süstrik makes the case in [The Callback Hell](http://250bpm.com/blog:24) that callbacks will eventually introduce loops in a system which will cause bugs because of unexpected invariant breakage when the callee calls back into the caller.  I've written a program that uses libsigc++ for callbacks. When I later replaced some of those libsigc++ callbacks with an event loop I had a system that was easier to reason about, but not as easy to follow through a debugger. In [Event Driven Architecture, State Machines et.al.](http://250bpm.com/blog:25) Martin says that the difference between a callback-based system and an event system is that in an event-based system, each event are processed in a sequential manner: the next event isn't processed until the previous has finished. He also mentions that in nanomsg, he uses callbacks when moving from root to leaves and events when moving from leaves to root. That is the exact same design choice as I've made. I've phrased that as: use function calls when you want to tell a component what to do, and use events when you want to inform  a component about an update in state. Martin also says that using state machines is about providing a  narrative for a system, it introduces a way to describe the phases a program goes through; more about psychology than about computer science.

After Martin has described the advantages of state machines, he then goes on to bash them too, in [Getting Rid of State Machines I](http://250bpm.com/blog:69) and [Getting Rid of State Machines II](http://250bpm.com/blog:70). His argument is that splitting linear code into a smaller sets of steps makes for code that is hard to maintain in the long run. State machines have been favored over processes and threads for performance reasons but with coroutines (a.ka. fibers) that argument is moot. But the remaining problem from replacing all state machines is the one of cancellation. Martin has had problems finding reliable ways to cancel work in progress. I've worked in programs that have accidentally evolved to extensive usage of threads and cancellation really is the main pain point there. 

Martin wrote libdill to address the problem of cancellation which he describes in [Structured Concurrency](http://250bpm.com/blog:71) and [Structured Concurrency in High Level Languages](http://250bpm.com/blog%3A124). 

## 14 June 2018

In [Day 32 More Simulator Features of Per Vognsesns Bitwise screencast series](https://bitwise.handmade.network/episode/bitwise/bitwise032/), Per describes a trick for introducing callbacks without inverting control. You have a command loop for your simulator and when you enter a breakpoint callback, you re-enter the command loop.  Note that you can re-enter the command loop, but you can't re-enter the bitwise simulator since it's not designed to be re-entrant.

```
var is_nested : bool;
func breakpoint_callback(hart: Hart*) {
    is_nested = true;
    cmd_loop(hart);
    is_nested = false;
}
func cmd_loop(hart: Hart*) {
    for (;;) {
        c := getchar();
        switch (c) {
        // ...
        case 'c':
        	if (is_nested) {
                return;
        	} else {
                step(hart);
        	}
        }
      	// ...
    }
}
```

Avoiding inversion of control flow seems to be a recurring theme in the bitwise screen-casts:  Per uses tagged unions instead of inheritance; he uses the above mentioned callback trick as a sort of poor mans co-routine; and he created a platform layer that works like immediate mode GUI's.

## 11 June 2018

I've placed `setxkbmap` calls in my .profile file, for swapping the  Caps Lock and Escape buttons and for remapping the Ctrl-keys to the Win tangents. But since Ubuntu switched back to using the Gnome desktop environment, the settings are sometimes restored to their original values. I'm not sure why it happens but it sure is annoying. I found the Ask Ubuntu question [How to Permantently switch Caps Lock and Escape](https://askubuntu.com/questions/363346/how-to-permanently-switch-caps-lock-and-esc) where they use the Gnome Tweak Tool. Let's hope I've gotten rid of that problem now!

## 30 may 2018

I was watching [Day 30 Assembler Macro Expansion of Per Vognsens Bitwise screencast series](https://bitwise.handmade.network/episode/bitwise/bitwise030/). He is writing a static assembler for risc-V, which is a three operand instruction set. But since he's accustomed to x86 and x64, he constructed the assembler language so that it omits one of the operands. He also created pseudo-instructions for call and jump to make the language less verbose. I hadn't thought about an assembler as an abstraction over the instruction set, instead of as a one-to-one mapping. Per also mentioned that some assemblers provides control flow directives like `.if` and `.else`, some even have `.case` directives. I scanned the NASM, YASM and GAS manuals but didn't find any references to such. The MASM have them though. 

## 29 may 2018

Today I was investigating some missing lines from a log file. The last lines that should have been printed immediately before the Linux system was killed by a supervising microprocessor, did not get printed. It turns out that the Linux page cache doesn't write back to disk until the data is more than 30 seconds old and the threads are woken up every 5 seconds. Further some flash-specific file systems has caches that themselves prolong the lifetime of dirty data with 3-5 seconds. I hadn't expected the write-back intervals to be that long! The `/proc/sys/vm/` directory contains four files that starts with `dirty_` which controls the write back timeouts.

[Gustavo Duarte describes in Page Cache, the Affair Between Memory and Files](https://manybutfinite.com/post/page-cache-the-affair-between-memory-and-files/) how the address space of a process maps to the page cache. The Linux page cache is indexed by something similar to an inode (but more general) and a block offset . It uses a structure called `address_space` but a better name would have been `physical_page_of_a_file`.  While a single file may be represented by  ten `vm_area_struct` structures (if say five processes mmap it twice), the file has only one `address_space` structure - just as the file may have many virtual addresses but exist only once in physical memory.

## 28 may 2018

While reading [the Wikipedia article on Optimistic Concurrency Control](https://en.wikipedia.org/wiki/Optimistic_concurrency_control#Web_usage) I found a reference to [the W3C note Detecting the Lost Update Problem Using Unreserved Checkout](https://www.w3.org/1999/04/Editing/). Therein it's described how HTTP being stateless uses a form of Optimistic Concurrency Control when updating resources. First a PUT request is sent with a `If-Match` header containing an etag. If that fails, the client can either abort, merge the content or force an update. The latter case is done by a second PUT request  with a `If-None-Match` header containing the same etag.

## 27 may 2018

A few days back I started writing my first web application in Node.js. One of the main confusion points for me was the caching mechanism in the HyperText Transfer Protocol (HTTP). I read the RFC's but they didn't make things much clearer. Now, finally I found [Mark Nottinghams Caching tutorial for Web Authors and Web Masters](https://www.mnot.net/cache_docs/). He starts of by explaining that there are two key concepts: freshness and validators.

The headers `Cache Control` and `Expires` controls the freshness of a document. The caches in your browser and proxies inspect those headers (where `Cache Control` is a HTTP/1.1 replacement to the old HTTP/1.0 `Expires` header) to decide if the document should be fetched or  not.

The `Last-Modified` and `ETag` headers are validators that the client can use for determining if a document whose freshness has expired needs to be refetched. The `If-Modified-Since` header is used for   timed-based validators and `If-None-Match` is used for ETag based validators.

## 18 may 2018

Marcus Frödin has written [slides for a lightning talk where he illustrates latency times in a computer system as weights](https://www.slideshare.net/marcusf/nonblocking-io-event-loops-and-nodejs):

* L1-cache A big squirrel
* L2-cache A medium sized cat
* RAM Mattias, basically
* Disk Like a 100 blue vales
* Network Belarus yearly wheat export

He then says that since a web server is I/O intensive, it's latency profile can be described as squirrels buried in cats, fat, vales and wheat. 

## 14 may 2018

[Nicolas Léveillé  has gathered a long list of links realted to The Problem with UI](https://gist.github.com/uucidl/8d4ca362c8341d1bb7bd0553eb3e9a1c). I wonder why User Interface programming is so hard? It's always a state synchronization problem where there is some backing state that's supposed to be reflected in the UI. But there are other axes of evaluation such as composition, data flow,  layout, painting, styling and extension.

[Per Vognsen lists a bunch of paradigms in Slate Notes](https://gist.github.com/pervognsen/279156b894c5d04ca73df7afc12a37ee). I don't quite understand what defines the Document/symbolic model and I'm still a bit lost when it comes to understanding what a reactive UI is.

* Ad-hoc with few reusable parts (e.g. lots of old games).
* Inheritance-oriented OO toolkits. Subclass if you want to override button click behavior. The darkest days of OO in early 90s.
* Slots-and-signals OO toolkits. Flatter hierarchies, more composition and reuse. Button click event fires a delegate.
* Document/symbolic model. Popularized by HTML and rarely copied. Less well known example: Mathematica Notebooks.
* Immediate mode. Ideal for fully dynamic UIs, no persistent identity for widgets, no inversion of control, fully code driven.
* Reactive UI toolkits. Born in the functional world. FB's React is a good mainstream example. Surprisingly close to immediate mode.



## 11 may 2018

I've written an interpreter for the mouse language. Here are the bugs I encountered while doing so:

* Had an off-by-one when I calculated the number of registers as `NUM_REGS = 'Z' - 'A'` instead of `NUM_REGS = 'Z' - 'A' + 1`.
* Both lowercase and uppercase are allowed for register and macro names, but lowercase and uppercase letters alias each others. I didn't canonicalize them.
* I had a `BINARY_OP` preprocessor macro for generating arithmetic and compare operations. But for the `=` case I generated an assignment statement instead of a compare expression.
* For loop statements, I only kept track of the outermost start location, so nested loops didn't work. When I added a `controlStack` for those, I created several bugs where I used `pop()` instead of `top()`.

## 8 may 2018

Jasper St. Pierre has created [the interactive site Explanations - play don't show](https://magcius.github.io/xplain/article/index.html) where he explains how the X Window System creates windows and handles input events, but also more advanced topics such as how to add transparency using the COMPOSITE extension and support alpha blending using the X_RENDER extension and basic 2D rasterization. He links to Sean Barrets [How the stb_truetype Anti-Aliased Software Rasterizer v2 works](http://nothings.org/gamedev/rasterize/). 

The most important takeaway for me was reading about pixmaps which allows for memory-backed pixel storage on the server. That storage is memory mapped into the process meaning each process can write data directly to the storage instead of having to copy it over the UNIX domain socket.

[The XCB project has a tutorial](https://xcb.freedesktop.org/tutorial/) which describes how to create windows and handle events, text and fonts and colors and pixmaps. I downloaded the XCB source code. The code can either use a push or a pull API. I didn't quite get how they handle events and request/response pairs arriving out of order. There appears to be a cookie identifier that is used for matching requests to responses. The code for emitting and parsing the protocol messages is auto-generated from xml-files via a python script.

Christophe Tronches [Xlib programming: a short tutorial](https://tronche.com/gui/x/xlib-tutorial/) gives a step-by-step introduction on how to use the X library. I had a peek inside the Xlib source code as well, and it's a bit harder to follow. I expect much of the code to be redundant. The core API as used by for example SDL appears to not have too much of a surface area.

## 7 may 2018

A GUI-program running on Linux uses the X-server for drawing screen content and receiving keyboard and mouse events. The X-server will be replaced by Wayland but it will take a while: Ubuntu enabled the Wayland display server for 17.10 but had to disable it for 18.04 due to system instability issues. Today, I had to debug  how an application received keyboard events from X. Here is a list of useful tools that I found:

* **xtrace** displays in clear-text the communication between the X server and a program. It does so by acting as a fake X-server,  a proxy.
* **xev**, the event tester, displays events triggered by mouse, touch pad and keyboard actions.
* **Xvfb** is useful for testing graphical applications in an environment where there is no X server present, so called headless testing.
* **xinput** has  a --list option that is useful for displaying which input devices exists on the computer. Peter Hutterers [Input Event Processing in X](https://who-t.blogspot.se/2010/07/input-event-processing-in-x.html) describes how the system is organized: Each physical device is added a a slave. A master device is the union of all attached slave devices. When a slave device generates an event, this event is sent through the master device as well. The XTEST devices are hard coded for each master device and serve as input devices for the XTEST extension. Below is an example from my Dell XPS 15 9550 laptop:

```
$ xinput --list --short
⎡ Virtual core pointer                          id=2    [master pointer  (3)]
⎜   ↳ Virtual core XTEST pointer                id=4    [slave  pointer  (2)]
⎜   ↳ HID 04b4:0033                             id=11   [slave  pointer  (2)]
⎜   ↳ Logitech HID compliant keyboard           id=13   [slave  pointer  (2)]
⎜   ↳ ELAN Touchscreen                          id=15   [slave  pointer  (2)]
⎜   ↳ DLL06E4:01 06CB:7A13 Touchpad             id=16   [slave  pointer  (2)]
⎣ Virtual core keyboard                         id=3    [master keyboard (2)]
    ↳ Virtual core XTEST keyboard               id=5    [slave  keyboard (3)]
    ↳ Power Button                              id=6    [slave  keyboard (3)]
    ↳ Video Bus                                 id=7    [slave  keyboard (3)]
    ↳ Video Bus                                 id=8    [slave  keyboard (3)]
    ↳ Power Button                              id=9    [slave  keyboard (3)]
    ↳ Sleep Button                              id=10   [slave  keyboard (3)]
    ↳ Logitech HID compliant keyboard           id=12   [slave  keyboard (3)]
    ↳ Integrated_Webcam_HD: Integrate           id=14   [slave  keyboard (3)]
    ↳ Intel HID events                          id=17   [slave  keyboard (3)]
    ↳ Dell WMI hotkeys                          id=18   [slave  keyboard (3)]
    ↳ AT Translated Set 2 keyboard              id=19   [slave  keyboard (3)]
    ↳ Logitech HID compliant keyboard           id=20   [slave  keyboard (3)]
```

* **xdotool** lets you programmatically (or manually) simulate X11 keyboard/mouse events. It does this using the previously mentioned X11 XTEST extension.

Jasper St. Pierre describes in [The Linux Graphics Stack](http://blog.mecheye.net/2012/06/the-linux-graphics-stack/) how he X server is hidden behind several API layers. GUI frameworks uses the Xlib library which uses the XCB library for doing the actual X11 protocol exchanges. There are lots of extensions to the original protocol and by now almost no one understands it fully. I remember reading in the book Coders at Work how Joe Armstrong when faced with writing a GUI-framework for Erlang, decided to bypass the XLib and KCB software and instead implement the X11 protocol directly. I tried searching for example programs that uses the X11 protocol directly but didn't find anything.

## 6 may 2018

Eric Shane has created [the entr tool for running arbitary commands when files change](http://www.entrproject.org/).  Rebuilding a project when any of the source files changes can be done with:

```
$ ag -l | entr make
```

The inotify system call can be used for implementing programs that wait for file and directory changes. But there is a lot of minutia that `entr` does for us, like detecting when an editor replaces a file upon write; making sure to not run the action command too soon when we're watching a whole directory; hiding the fact that NFS directories  are handled differently than regular directories by inotify.

## 3 may 2018

[Niklas Grays blogpost One Button Source Code Builds](http://ourmachinery.com/post/one-button-source-code-builds/) describes how [Our Machinery](http://ourmachinery.com/) uses a C++ program for downloading all dependencies and generating a Visual Studio project. When I was working on Subversion we had a script for fetching dependencies, for Firefox you can invoke `mach bootstrap` and have all needed dependencies downloaded to your computer. It's a very useful thing to have. At work, we rely on the Ubuntu package manager for fetching dependencies which causes headaches when we upgrade our systems.

Niklas mentions that they use premake for generating the actual build files. It's similar to cmake but more limited and not as widely used, though I've seen several game developers making references to it. Here's what I'd like to have in my build system

- A bootstrap command for downloading dependencies

- A  tool that can generate project files for different IDEs

- A simple language for describing build components and targets

- Caching of previously built artifacts similar to ccache

- Ability to distribute build jobs - probably requires something more reliable than timestamps for identifying what needs to be rebuilt

## 27 april 2018

[Casey Muratoris talk Designing and Evaluating Resuable Components](https://caseymuratori.com/blog_0024) evaluates an API according to the following principles:

- Granularity (A or BC)
- Redundancy (A or B)
- Coupling (A implies B)
- Retention (A mirrors B)
- Flow Control (A invokes B)

Micha Mettke expands on Caseys ideas in the gists [API Design: Granularity](https://gist.github.com/vurtun/192cac1f1818417d7b4067d60e4fe921) and [API Design: Modular Data-oriented processes](https://gist.github.com/vurtun/9782db089430167453cff6785b37bb46) and [Library Writing](https://gist.github.com/vurtun/2aeafb0d66272dc29a942e04f94cbe06). 

While reading Michas gists I found a link to Jon Olicks website where he has published [small code snippets for writing and decoding MPEG, JPEG, GIF and WAW data](https://www.jonolick.com/code.html). I was surprised to see how little code was needed.

## 25 april 2018

Since C99 a function that takes an array parameter can use the expression `char buf[static 10]` for expressing that the function must be called with an array of at least size 10. Clang will issue warnings if you call the function with an array of fewer elements or with a NULL pointer. GCC and ICC don't issue a warning. https://hamberg.no/erlend/posts/2013-02-18-static-array-indices.html

## 24 april 2018

I read Rico Martinis  articles [The side benefits of comprehensive unit tests](https://medium.com/@ricomariani/the-side-benefits-of-comprehensive-unit-tests-de6c32730b6e) and [How to make ANY code in ANY system unit-testfriendly](https://blogs.msdn.microsoft.com/ricom/2014/11/20/how-to-make-any-code-in-any-system-unit-test-friendly/ow) where he describes how to factor out code using what Michael Feathers refers to as seams. What I hadn't heard was the term [Ambient authority](https://en.wikipedia.org/wiki/Ambient_authority) which Rico uses when he refers to the seams. A program uses ambient authority if it only needs to specify the name of an object that it wants to perform an action on. The authority is "ambient" in the sens that it exists in a broadly visible environment where any subject can request it by name.

In contrast to Ricos opinion on the the usefulness of unit-tests, there's [Write tests, not too many, mostly integration tests](https://blog.kentcdodds.com/write-tests-not-too-many-mostly-integration-5e8c7fff591c) by Kent C. Dodds that makes the case for concentrating on integration tests for the best "bang for the buck". In his view, there's a limiting return when you go above 70% test coverage and unit tests in particular takes a lot of time to write. Martin Sustrik makes similar claims in [Unit Test Fetish](http://250bpm.com/blog:40). 

##  23 april 2018

I was designing two moderately complex state machines consisting of  roughly ten states each and I started thinking about how to verify the correctness of them. I can verify the correctness of an algorithm consisting of sequential, iterative and conditional statements using preconditions, postconditions, invariants and induction. But how can I verify the design of the state machine before I've written it?

I read David Harels Statecharts: a Visual Formalism for Complex Systems. It gives suggestions on how to draw state charts that can handle more states, but it says nothing about correctness. I guess you just have to check case by case the transitions into the state; the transitions out of the state; and that the program won't get stuck inside the state.

The [Harel statecharts wikipedia article](https://en.wikipedia.org/wiki/State_diagram#Harel_statechart) lists [YAKINDU Statecharts Tools](https://en.wikipedia.org/wiki/YAKINDU_Statechart_Tools) under the "See Also" section. I downloaded the program and tried a few small FSM:s. It has a simulator that can be used for interactively simulating a state chart. Looked interesting but I didn't have time to learn enough of it to use for my own design.

## 17 april 2018

Firefox Webassembly JIT-compiler, SpiderMonkey, consists of an interpreter, a baseline compiler and a fullblown compiler. The baseline compiler makes a  trade off between compilation speed and execution speed of the generated code. It generates decent code blazingly fast. It uses a "latent stack" for keeping track of register values. If we don't have any more registers, then they are "spilled" to memory. Today I read through the code and I believe that I finally understand it. It uses a discriminated union for keeping track of whether 

```
struct Stk {
    enum Kind {
        MEM,
        REG,
        CONST,
        LOCAL
    };
    Kind kind;
    union {
        uint32_t offset;
        Reg reg;
        uint32_t val;
		uint32_t slot;
    };
};
```

A set keeps track of how many registers are unused at the moment. When that count reaches zero, the compiler syncs all  Stk entries to memory. When a value is supposed to be popped from the stack it is loaded from memory if not available. I initially got confused by the many different stacks in the compiler: there is a stack of call frames, a stack of latent values and the targets runtime stack.

## 16 april 2018

I watched [Bitwise Day 15: More Compiler Hacking](https://bitwise.handmade.network/episode/bitwise/bitwise015/) where Per Vognesen describes arithmetic conversion rules in C, compund literals

A function declaration on the form `void f()` means different things in C and C++. In C it means "could take any number of parameters of unknown type" and in C++ it means the same as `void f(void)`. Always use  `void` parameters when coding in C!

Compound literals in C99 are lvalues. You can do `int *ip = (int){42}` and the value pointed to by `ip` is guaranteed to have the lifetime of the surrounding scope.

## 9 april 2018

[The stackoverflow post Why are 5381 and 33 so important in djb2](https://stackoverflow.com/a/4825477/582010) 

## 8 april 2018
I've gone through chapter 2 of the book [Computer Systems - A Programmers Perspective](http://csapp.cs.cmu.edu/). Exercise 2.73 is about implementing a saturating add function that don't rely on conditionals (if or ?:), relative comparison operators, casting or division and multiplication. I came up with a somewhat long-winding solution, but then I found [this solution](http://zhangjunphy.github.io/csapp/chap2.html#org2fd5283)

    int saturating_add(int x, int y) {
        int sum = x + y;
        int pos_over = !(x & INT_MIN) && !(y & INT_MIN) && (sum & INT_MIN);
        int neg_over = (x & INT_MIN) && (y & INT_MIN) && !(sum & INT_MIN);
    
        (pos_over && (sum = INT_MAX)) || (neg_over && (sum = INT_MIN));
    
        return sum;
    }

Note the line before the return statement. It sort of cheats by using a logical expression for constructing a ternary conditional operator.

I watched [Day 12: More Optimizations & Cleanup](https://bitwise.handmade.network/episode/bitwise/bitwise012/) from Per Vognsens Bitwise project. He's replaced a few linear searches with hash table lookups and the way the hash tables were constructed was new to me: They use open-addressing and maps pointers to pointers instead of allowing arbitrary objects as keys and values.

One other thing was the hash function Per uses for 64 bit unsigned numbers. He says it's based on the [Fowler-Noll-VoFNV hash function](http://isthe.com/chongo/tech/comp/fnv/). I realized that I have no idea how to design a hash function. For future reading I found [Bob Jenkins Dr Dobbs article on designing a hash function](http://www.burtleburtle.net/bob/hash/doobs.html) and [Peter Kankowskis thorough benchmarks of various hash functions](http://www.strchr.com/hash_functions)

    uint64_t uint64_hash(uint64_t x) {
        x *= 0xff51afd7ed558ccdul;
        x ^= x >> 32;
        return x;
    }

## 26 mars 2018
I wrote a small string interning module. It consists of less than 30 lines of code. The idea is that we keep a global list of known strings. Once a string has been registered, it can be compared to other interned strings by a simple pointer comparison. Each interned string has a length and a pointer to the string.

    typedef struct InternStr {
        size_t len;
        char *str;
    } InternStr;

They are stored in a global stretchy buffer.

    static InternStr *interns;

Upon calling `str_intern()`, we first compare the string to all existing strings, and return the the pointer of a match. If no match is found , then we allocate memory for the string and append a `InternStr` object to the list of interns.

I have never used string interning in C before. I'm surprised how little code was required. But I wonder if it's feasible to use a global list of intern-strings or if they should  be partitioned into several arenas?

## 25 mars 2018
Fabien Giesen describes in [an aside for a gist note](https://gist.github.com/anonymous/2edc9e9d52a93c126ff486cfb4a2c65b) how he views error handling. He points out that it may be beneficial to only provide a small set of error codes and that the selection of those should be dictated by
the question "what should I do next?". E.g. there are many ways a network connection can fail but providing a giant taxonomy of error codes won't help the calling code to decide what to do. Logging should be as specific as possible but the users of an API just needs to decide what to do next.

Fabien wrote [in a blog comment](http://cbloomrants.blogspot.se/2010/06/06-07-10-exceptions.html?showComment=1275971007229#c4849051810850075252) that having stack unwinding do the cleanup on errors is a bad design that costs lots of resources and is hard to control.

One problem I've had with error handling is that it introduces a lot of noise that hides the main  computation in the source code. Fabien suggests in the article [Buffer Centric I/O](https://fgiesen.wordpress.com/2011/11/21/buffer-centric-io/) that you can use something similar to the null object pattern that just does nothing after the first error has been encountered. Rob Pike describes a similar approach in [Errors are Values](https://blog.golang.org/errors-are-values).

## 23 mars 2018
I tried to implement stretchy buffers from scratch. These are the problems I encountered:

* I didn't parenthesize `(b) = buf__grow((b), (n), sizeof(*b))` when it was used as the second expression in a ternary expression. It triggered a compilation error about non-existing lvalue for `buf__push`: the b expression was parsed as belonging to the ternary expression.
* I returned `new_hdr` instead of `new_hdr->buf` from `buf__grow`.
* I didn't initialize `new_hdr->len` and `new_hdr->cap` correctly.
* You can't use macros like `offsetof` as QtCreator watch expressions
* QtCreator choked on `*(BufHdr)((uintptr_t)vals - 16)`. You can't dereference a
  cast.
* I initialized `new_hdr->len` to `len` instead of `0`. The `len` should be incremented in  `buf_push`.
* I called `realloc` with `buf` as argument instead of `buf__hdr(buf)`.

That's for 30 lines of code, with a simplified capacity calculation compared to the original. It's humbling to see on display how many errors my code had.

## 21 mars 2018
I watched Per Vognsens recorded stream for bitwise [Day 5: Ion Parser/AST Code Review (March 21, 2018)](https://github.com/pervognsen/bitwise/blob/master/notes/streams.md#day-5-ion-parserast-code-review-march-21-2018).
In there he shows how to split out a large C files into several included C-files. It makes for rapid grouping into physical C files but you have to mark those C-files as headers in the build system and you have to keep track of include dependencies in the main file.

I found that reading Pers AST-code helped me internalizing some parts of the C syntax, that I've learned by osmosis but never thought more explicitly about. The C language syntax is composed of declaration, statements and expressions. The declaration includes type specifiers, optional type qualifiers and
optional storage class specifiers.

Thankfully the ion language that Per is building doesn't use C's declarators syntax. But even so, it still feels like a little too many concepts if I want to shadow-implement parts of ion as I follow along. I think I'll start with an expression language, then build on it by introducing identifiers and assign statements, then I'll introduce selection statements and iteration statements.

The code is pretty repetitive. It would be interesting to compare with the corresponding ML code (which supports tagged unions natively) would look like.

## 20 mars 2018
F6 can be used for iterating over elements in one of QtCreators output panes (compile issues, search results,...). Ctrl-F10 is used for "Run to Line" in debugger mode. The debugger key bindings are the same as those found in Visual Studio. F9 is used for toggling breakpoints.

I watched Per Vognsens recorded stream for bitwise [Day 4: Lexer, Parser, AST marathon](https://github.com/pervognsen/bitwise/blob/master/notes/streams.md#day-4-lexer-parser-ast-marathon-march-19-2018).
I'm somewhat overwhelmed by that large set of AST data types that are necessary. One neat thing though is how he handles the expression types: Per makes an `Expr` discriminated union that can handle int, unary, binary, ternary and sizeof expressions (and some more). Seems like a good factoring to me.

Speaking of factoring. This was the first time that I realized that the word factor used in this context, resembles the meaning of the factor operation for simplifying expressions in math. All this years, I've been reading about 'refactoring' and have never given a thought to what that word really
represents.

Per had a _NONE field for each kind enum that was placed first. So I zero initialized variable would have a sensible default value. I'll try to use that in the future.

## 19 mars 2018
The C99 standard introduces a new syntax for initializing and reassigning structs and unions that I've heard of but haven't ever used before now. Here's a definition of a `struct Foo`:

    typedef struct Foo {
        int val;
        char c;
    } Foo;

You can initialize it with a brace enclosed comma separated list of initializers for each member:

    Foo foo = {1, 'a'};

But that gets tedious when you have many members, and for unions, you can only initialize the first member. With designated initializers you can initialize them in your own chosen order and the explicit designator makes the code more readable:

    Foo foo = {.val = 1, .c = 'a'};

For reassigning the value of `foo` to new values, in C89, you would have to do an explicit `memset` and assign each member separately which makes for very "flat" repetitive code.

    memset(&foo, 0, sizeof(foo));
    foo.val = 2;
    foo.c = 'b';

With C99 compound literals we can instead write the above as this one-liner:

    foo = (Foo){.val = 2, .c = 'b'};

The array initialization has been improved as well with explicit designators. Here's a small example for mapping enum values to a string description.

    enum { RED, GREEN, BLUE };
    char *colors[] = {
        [RED] = "red",
        [GREEN] = "green",
        [BLUE] = "blue"
    }

## 15 mars 2018
I watched Per Vognsens recorded stream for bitwise [Day 3: More Programming & Parsing (March 15, 2018)](https://github.com/pervognsen/bitwise/blob/master/notes/streams.md#day-3-more-programming--parsing-march-15-2018).
He starts off by listing the only three data structures that you'll need when programming in C:

* stretchy buffers
* pointer/uintptr to hash tables (uintptr -> uintptr key-value mapping)
* string intern table

He does the string interning by keeping a list of Intern structs that each points to a canonical version of a string.

    typedef struct Intern {
        size_t len;
        char *str;
    } Intern;

Per mentioned that you can't use a flexible array member for str: it would allow compact storage of the string in consecutive memory, but once the storage is full and we reallocate we loose the property that the pointers are stable.

Per went through parsing expressions by creating separate production rules for each precedence level. The code is clean, but there will be lots of recursive calls to resolve each expression type. How can I avoid all those recursive calls?

## 14 mars 2018
The C standard, $6.7.2.1 p16 says that an empty array member placed last in a struct can be used is called a "flexible array member". The size of the struct is as if the array had not existed (except for maybe some additional padding). But the array can be accessed through the '.' or '->' operator and would then behave as if the array takes up remaining place of the storage allocated. Neat!

QtCreator has the same key-binding, Ctrl+F10, as Visual Studio for "running until cursor". Very useful for inspecting loops. I experimented with adding a few watch variables to QtCreator. I hadn't done that before. I also noted that -Og does not preserve macros. I had a hard time trying to place a breakpoint inside a loop to a stretchy buffer `buf_push()` call.

## 13 mars 2018
I watched Per Vognsens second screen-cast about his just started Bitewise project where he aims to create an early 90's computer from scratch and write all the software for it. He will start out by creating his own systems language, a spin-off from C with some nuances removed. His pet peeves about C are the same as mine: the operator precedence for bitwise operations are wrong; there is too much undefined behavior; the lack of a module system and reliance on the textual replacement by the preprocessor; the complicated
declarator syntax; too much implicit conversions are allowed; the cast operators are too allowing.

I'd add the limited functionality of the typedef keyword to that list.


## 12 mars 2018
This question got me stomped: Given an array of integer doublets find the single integer that  is not a doublet. Do it in linear time and without extra storage. I had to look up the  solution: You keep a running XOR result. Every doublets will cancel itself and in the end only the bits of the requested integer will remain set.

    int findSingle(int* nums, int N) {
        int x = 0;
        for (int i = 0; i < N; ++i)
            x ^= nums[i];
        return x;
    }

It feels like magic to me. How can I prove to myself that this algorithm works?

## 7 mars 2018
In the past I've used `~0` for getting the complement for an integer. An integer literal without suffix has type int so the size if guaranteed to be `sizeof(int)`. But if the expression where we want to use the complement has another type than int, then we may be in trouble. `~(0 & val)` solves that, where val is a variable that has the same size as our expression.

I implemented arithmetic shifts in terms of logical shifts and visa versa. I was glad to see that my `sra` function used the same formula as one of the formulas given in Hank Warrens book Hackers Delight.

    int32_t sra(int32_t x, int n) {
        // Logical shift.
        int32_t srl = (u32)x >> n;
    
        uint32_t msb = (u32)x >> 31;
    
        // Set to all ones if msb=1.
        int32_t leading = 0 - msb;
    
        // Mask for upper k bits.
        leading = leading << (31-k);
    
        return leading | srl;
    }

This can be written succinctly as below, which is formula 4 from chapter 2.7 Shift right signed from unsigned.

        return (u >> n) | (-(u >> 31) >> (31 - n))

## 6 mars 2018
Alexander Gromnitsky has written [Notes for new Make users](http://gromnitsky.users.sourceforge.net/articles/notes-for-new-make-users/). That article and Chris Wellons [A tutorial on portable Makefiles](http://nullprogram.com/blog/2017/08/20/) gives good advice on writing a make based build-system. I mostly end up using CMake for its IDE integration and dependency detection, but I have an affection for make, kind of how I feel about AWK: a small well designed tool that only does the absolute necessary.

Here's a Makefile skeleton that auto-detects dependencies and creates the build directory if it doesn't exist. I really wish that make could do something more clever about missing directories.

And I just learned that the autovars are visual mnemonics:  `$@` looks like a target, a ring with a bullseye. `$<` is pointing left, so it is the first prerequisite. `$^` is like a horizontal bracket that groups all prerequisites.

    FLAGS += -MMD -MP
    CFLAGS += $(FLAGS)
    CXXFLAGS += $(FLAGS)
    
    OBJECTS += $(patsubst %, build/%.o, $(SOURCES))
    DEPS = $(patsubst %, build/%.d, $(SOURCES))
    
    $(TARGET): $(OBJECTS)
    	$(CXX) -o $@ $^ $(LDFLAGS)
    
    -include $(DEPS)
    
    build/%.c.o: %.c
    	@mkdir -p $(@D)
    	$(CC) $(CFLAGS) -c -o $@ $<
    
    build/%.cpp.o: %.cpp
    	@mkdir -p $(@D)
    	$(CXX) $(CXXFLAGS) -c -o $@ $<

## 5 mars 2018
I read a newly published article by Fabien Giesen called [A whirlwind introduction to dataflow graphs](https://fgiesen.wordpress.com/2018/03/05/a-whirlwind-introduction-to-dataflow-graphs/). It describes a conceptual model for how to reason about performance for inner loops, also called compute kernels. Fabien makes the simplifying assumption that all ALU operations takes 1 cycle and that loads and stores takes 4 cycles. Here's a simple sum function for an array of integers:

    loop:
        rCurInt = load(rCurPtr)             // Load
        rCurPtr = rCurPtr + 8               // Advance
        rSum = rSum + rCurInt               // Sum
        if rCurPtr != rEndPtr goto loop     // Done?

A dataflow diagram is a graph of the dependencies between instructions. A diagram for the kernel above shows that the Advance step is the critical dependency chain. The Load has a latency of 4 cycles, but the compiler and CPU can schedule around that and fill the slots with other instructions. Each cycle, 4 instructions can execute. On a platform with enough execution units, the limitation will be the data dependency. On a platform with fewer units, we say that the kernel is throughput bound.

Here's another kernel that computes the sum over a linked list. Note that it has two loads. Here the dependency chain will be four cycles long and that's worse. Fabien says that people often points out that linked lists has poor locality and causes lots of cache misses, but that's beside the main point, he
says. Cache misses is not a problem if we can find meaningful work to do in the meantime. But since we're blocked on loading the next node, we can't really make progress.

    loop:
        rCurInt = load(rCurPtr)             // LoadInt
        rSum = rSum + rCurInt               // Sum
        rCurPtr = load(rCurPtr+8)           // LoadNext
        if rCurPtr != rEndPtr goto loop     // Done?

## 2 mars 2018
C and C++ don't initialize local variables automatically. That can lead to hard to trace down bugs.  Fortunately the compiler can assist in finding expressions that access uninitialized variables. I've noticed when building Firefox that an optimized build triggers lots of false positives. Why is that? Why can't the compiler distinguish between real uninitialized variable accesses and benign cases?

Engineering a Compiler, section 8.6 says that the fundamental problem is that data-flow analysis of the Control Flow Graph (CFG) expects all paths through the graph to be reachable, which is not true in practice. So the compiler knows too little about the program to avoid false positives. Here is an example from that chapter:

    int main() {
        int i, n, s;
        scanf("%d", &n);
        i = 1;
        while (i<=n) {
            if (i==1)
                s = 0;
            // Some compilers will flag this as an uninitialized read of |s|
            s = s + i++;
        }
    }

Another problem is when a value is available through another name and initialized through that name. For instance this code from the book, might trigger a false positive:

    int x;
    int* p = &x
    *p = 42;
    x = x +1

I tried running both these examples in Compiler Explorer, but I didn't get any false positives.


## 28 february 2018
[The grammar for Webassemblys text format](https://webassembly.github.io/spec/core/text/values.html#integers) says that hexadecimal constants can have a '-' prefix and include '_' chars as separators. That doesn't work in Firefox.

## 21 february 2018
I've used substraction when comparing integers in the past. I learned it from Robert Sedgewicks Algorithms book (I belive, I might misremember). But [Ted Unangst points out that overflow will give incorrect results](https://www.tedunangst.com/flak/post/subtraction-is-not-comparison)

## 13 january 2018
C and C++ doesn't have negative integer literals. An expression like `-42` is treated as unary minus applied to the literal `42`.

## 12 january 2018
Iterating over a binary search tree is easy, just use a recursive function for visiting each element.

    void visit(node)
        if (!node)
            return
        visit(node->left)
        use(node)
        visit(node->right)

But what if you want to create an iterator for your tree? You have to know where you are when you're returning, so that the next invocation knows where to continue. Here's three different solutions

* Traverse the tree and store the nodes in a queue that you then return one by one.
* Use a stack for keeping track of which nodes you've visited
* Store the latest visited node and do a search for the next node that is larger.
* Store the latest visited node and walk the tree from there.
* Mark each visited node and keep track of current position. TAOCP Volume 1, 2.3.1, exercise 21 has some tricks that I belive are in that direction. I'll need to  investigate that further.

## 11 january 2018
On the stm32f1 microprocessor, and most other ones as well I assume, different IP-blocks have assigned default pins for interfacing with the outside world.  There can be alternatives to the default pins and those can be specified using a register. In the case of stm32f1, that register is named AF Remap and Debug I/O Configuration Register (AFIO_MAPR).

I wanted to generate a PWM signal on a specific pin, but that pin could not be connected to any of the timer blocks, so I had to resort to using a timer block and manually toggling the pin from there.

## 1 december 2017
I've started participating in the [Advent of Code](adventofcode.com) coding challenge. I've created a IPython notebook with the same layout as Peter Norvig used for his 2016 AoC solutions. Peter will be participating this year as well.  My plan is to solve each days problems and then compare my solutions to his and record any insights I gain here.

Today we were asked to find the sum of all digits that match the next one. I figured that it meant iterating using pairs, and came up with this function:

    def pairs(seq):
        return zip(seq[:-1], seq[1:])

But Peter took advantage of Pythons ability to use negative indexes.

    sum(a[i] for i in range(N) if a[i] ==  a[i-1])

He used it for part two as well where we were asked to compare values at half the distance.

    N = len(a)
    sum(a[i] for i in range(N) if a[i] ==  a[i-N//2])

## 28 november 2017
When I modified the calling convention for the p-code compiler, I once again ran into trouble. The idea was to pass all arguments on the stack and return them on the stack as well. But since I hadn't yet added support for generating movs with displacement, I couldn't place the return value below the return address, so instead I passed it in rax and pushed rax after the call instruction. Or, I thought I had the push(rax) after the call instruction. In reality it was before and it took me some time to figure out why the function did not return.

I found the GEF project which provides a customized python file that uses the gdb python bindings. With that I could inspect the stack and quickly found my problem. I wonder how to do that in plain gdb?

## 25 november 2017
I ran into trouble when I tried to modify the calling convention of my toy VM interpreter. Tried inspecting the stack in gdb, but that didn't give me any insights. Enabled debug logging of interpreted instructions and current value stack. Then I almost immediately found my problems. A debugger gives me state inspection, but debug logs give me state over time. Maybe I just need to practice my gdb skills?

The problem was an off-by-one in the stack data structure for tracking args and locals relative to the frame pointer. Adding asserts helped me pinpoint the problem.  Asserts and debug logs, is what I usually cling to when troubleshooting.

## 24 november 2017
When I added a compiler for my toy VM, in addition to my preexisting interpreter, I realized that I needed to modify the calling convention and how I store arguments and locals. Like a P-code VM, the machine uses a single stack for callstack and operand stack. When I interpret the code, I can more or less choose the calling convention to my own liking, but when I emit machine code, I'm restricted to a schema that uses the call instruction and places the return address after the arguments. I could have used hacks like exploiting RIP-relative addressing with lea rax, [rip], but all other JIT compilers I've seen uses the call instruction.

## 23 november 2017
For my p-code look-alike toy virtual machine I've started adding a JIT compiler in addition to the existing interpreter. While doing so I realized that for the compiler I'll have to maintain a list of locations to patch up forward calls and forward branches. The destination operand of the calls/jmp encodes the bytecode position, not the machine code position. To solve that I see three possibilities:

1. Record jump/call targets on a first scan. Then scan again and patch the jump/call operands.
2. Introduce bytecode function opcodes. Backpatch the jump/call operands once the function has been emitted.
3. Annotate the bytecode with blocks to delimit functions and jumps. That's what's done for Webassembly. It limits the scope of where jump/call operands can point.

I choose option 2 for my toy OS.

## 16 november 2017
I watched Louis Brandys talk Curiously Recurring C++ Bugs at Facebook, from CppCon 2017. He says that std::map::operator[] is the number one newbies bug.  The problem is that if you're reading from a map with operator[] and pass in a non-existing key, then that key will be created with a default value.  Other languages has something similar to a get_or_default() method, but that would allow passing in a temporary for the default argument, you can't take a reference to a temp value, so we'd need to return by value, but that is more costly. So, looks like we're stuck were we are.

Yesterday, I needed to dereference labels in a bytecode compiler I'm writing.  There were OP_LABEL opcodes but their operands were positions in the bytecode, not the generated machine code. I solved that neatly by relying on std::map::operator[] to create missing values.

    map<BytecodePos, Label> labels;
    switch (opcode) {
    ...
    CASE OP_LABEL:
        masm.bind(labels[pos]);
    CASE OP_BR:
        addr = code[pos++];
        masm.jump(labels[addr]);
    ...
    }

## 9 november 2017
Remembering the calling convetion for SysV ABI can be difficult.  Here's a useful mnemonic found on the [CS:App blog](http://csappbook.blogspot.se/2015/08/dianes-silk-dress-costs-89.html)

    %rdi:   Diane's
    %rsi:   Silk
    %rdx:   dress
    %rcx:   costs
    %r8:    $8
    %r9:    9

## 30 november 2017
Subfields can be extracted with split() in awk

    echo "/path/to/foo 12K -rwx" | awk '{split($1, subfields, "/"); print(subfields[3], $2, $3); }'
    foo 12K -rwx

Printing the N last fields on a line can be done with

    echo "a b c d" | awk '{ print($(NF-2), $(NF-1), $(NF)); }'
    b c d

www.cs.princeton.edu/courses/archive/spring17/cos333/tutorials.html

## 28 november 2017
The joinreg in the Firefox wasm baseline compiler is used for transferring values out of blocks.

## 27 november 2017
I've never thought too much about the range different integer and float types.

* int32_t can hold about 2 billion positive or negative values.
* int64_t can hold about 2 billion billion positive or negative values
* float can hold about 10^38 positive or negative values
* double can hold about 10^308 positive or negative values.

## 16 october 2017
A short summary of the  I2C protocol: I2c messages consists of address frames and data frames. A transaction is started by the master by pulling SDA low while keeping SCL high. After the address bits have been sent, the sender waits for the receiver to pull SDA low to indicate that the message was succesfully received. After the address frame has been transmitted, data can be sent. If it's the master or slave that sends, depends on the R/W bit inn the address frame. The end of the message is signalled with a "stop condition"; A zero to one transition takes place on SDA after the  zero to one transition on SCL, with SCL remaining high.

## 11 october 2017
With two screens attached to my Dell XPS 15 9550 laptop, the screens goes blank after less than one minute of inactivity. An Internet search suggested that [Display Power Management](https://wiki.archlinux.org/index.php/Display_Power_Management_Signaling) might be the cause. I inspected the current state with ```qset -q```. It said that DPMS was activated, but all timeouts were set to zero. Still, I disabled dpms with ```qset -dpms``` and got rid of the problems. This problem did not occur yesterday. Are software updates the cause? A timing issue?  I had the screen cables plugged in at boot today. Yesterday I inserted them after I had logged in.

## 10 october 2017
I've finally bought a USB3 adapter cable for Displayport. My Dell XPS 15 has only one display output (a HDMI) so for over a year, I've had a second monitor on my desk that hasn't been used.

Tmux uses panes, sessions, windows. If you attach multiple tmux clients to the same session, then they will share window layout and current window. You might want to display different things... Can be donw with:

    tmux new-session -t 'original session name or number'

```setw -g aggressive-resize on````is useful for preventing two open sessions from syncing their sizes.

## 1 august 2017
The System V AMD64 calling convention passes arguments to function in these registers:

    rdi, rsi, rdx, rcx, r8, r9

I've wondered why exactly those registers are choosen and why they're choosen in that particular order. Peter Cordes [dug deep in the AMD64 mailing list archive](https://stackoverflow.com/a/35619528/582010) and summarized the decision as:

* the register usage should be optimized for code density. Registers that needs REX prefix, takes more code space and should be avoided.
* rdi and rsi was choosen since they are used often in string operations which come up often in real life code.
* registers with special purposes are not call preserved.
* rcx is placed late in the sequence since it's used commonly for special purposes and it can't be used for syscalls - they want the syscall paramters to match the libc parameters.


## 31 august 2017
Yasm (which uses the NASM syntax as default) does not allow reserved words in labels.

        global add
    add:
        add edi, esi
        mov eax, edi
        ret

The error:

    $ yasm -felf64 add.asm -Worphan-labels
    add.asm:3: error: unexpected `:' after instruction

Fix by prepending a '$' symbol before the label.

        global add
    $add:
        add edi, esi
        mov eax, edi
        ret

## 29 august 2017
Awk has range patterns. If you try to use identical markers for the beginning and end of a text, then awk will only print the markers.

    $ cat > foo.txt
    <<<
    a
    <<<
    ^D
    $ awk '/<<</,/<<</' foo.txt
    <<<
    <<<

You need to use different markers for begin and end of range or replace the use of range patterns with [one pattern and a state flag](https://www.gnu.org/software/gawk/manual/html_node/Ranges.html)

## 28 august 2017
The Webassembly VM is a structured stack machine. It has control flow instructions that closely mimics the if/while/switch statements of high level languages. Using a structured stack machine makes it easier to verify the code in an interpreter or baseline compiler.

A br, br_if or br_table is only allowed to jump to a containing block or loop.

A continue statement is emitted with a br to a loop; a break statement is a br to the containing block.

The stack height after a br, br_if and br_table is the height of the stack at the beginning of the surrounding block. In a loop, the stack is popped to the size upon entry on each iteration.

Webassembly producers generate many locals. This effectively means that Webassembly has an infinite number of registers, and can choose to spill values as it sees fit. The interaction between the structured stack machine and the internal stack of the VM is something that has had me scratching my head for quite some time. The SM baseline compiler adjust the execution stack when branching according to what was written above, but it also syncs the compiler stack that keeps track of which values resides in registers and which are spilled.

Here's an example of how a switch statement may be compiled into Wast:

    switch(x) {
        case 0:
            // do A
            break;
        case 1:
            // do B
            break;
        default:
            // do C
            break;
    }
    
    block $exit $A block $B block $default
        get_local $x
        i32.const 2
        i32.gt_u
        br_if $default
    
        get_local $x
        br_table $A $B $default
    end $default
        ;; do C
        br $exit
    end $B
        ;; do B
        br $exit
    end $A
        ;; do A
        br $exit
    end $exit

It provides
instructions that closely map the if/while/switch statements in high-level languages.

## 23 august 2017
The absolute value of a signed integer is not defined for the minimal value it can take. For uint8_t the range of possible values are [-128, 127]. Note the assymetry. Taking abs(-128) can not yield 128 since it's out of range.

The compilers clang, icc and gcc compiles an abs call to [different assembly](https://godbolt.org/g/92SkLp):

    icc_abs(int):
        mov       eax, edi
        cdq
        xor       edi, edx
        sub       edi, edx
        mov       eax, edi
        ret
    
    gcc_abs(int):
        mov edx, edi
        mov eax, edi
        sar edx, 31
        xor eax, edx
        sub eax, edx
        ret
    
    clang_abs(int):
      mov eax, edi
      neg eax
      cmovl eax, edi
      ret

## 22 august 2017
A gpio pin on most microprocessors is internally connected to two resistors and two transistors. The distinction betwen the different input and output types has always escaped me but thanks to Muhammed Saadis blogpost [How to use digital input/output](http://aimagin.com/blog/how-to-use-digital-input-output/) I finally understand the differences.

* *pull-up input* A resistor is connected to Vdd.
* *pull-down input* A resistor is connected to Gnd (Vss).
* *floating input* Both internal resistors are disconnected. Useful for circuits with external pull-up or pull-down resistors. Useful for reading analogue values.

* *push-pull output* A transistor is connected to Vdd and another is connected to Gnd (Vss). When needing output status 0, the system will open the Vdd transistor and close the Vss transistor. Visa-versa for output status 1. With push-pull the load can either be connected to the external Vss in which case the processor is sourcing current (active low). If the load is connected to external Gnd (Vdd) then the system is driving the load.

So for inputs there's a pin configuration distinction between active high and active low. For outputs the are pin configured in the same way but higher layers of the software stack needs to keep track of whether the signal is active high or active low.

* *open-drain output* The Vdd transistor is disabled, only the Vss transistor can be opened or closed. Here the processor can only drive the signal low.  Useful for or-logic where multiple chips are connected like on an i2c bus.

## 21 august 2017
The gnu linker generates secions called .glue7 and .glue7t. Those provides code for interworking between thumb format and regular ARM instructions [according to Ian Lance Taylor]( https://gcc.gnu.org/ml/gcc-help/2009-03/msg00306.html).

The .vfp11_veneer are for the vector floating point coprocessor. The .v4_bx section is for a patch of the ARMv4 core to support the BX instruction.

## 16 august 2017
I learned a few things about common typedefs for integer types in C/C++ today

    * size_t is for sizes: strlen, malloc..
    * ssize_t is for returning sizes and negative values to indicate errors.
    * off_t is for offsets in files: lseek.
    * ptrdiff_t is for distance between pointers.
    * clock_t is for clock ticks.
    * time_t is for time in seconds.

Some more typedefs can be found in [systypes.h](http://pubs.opengroup.org/onlinepubs/007908775/xsh/systypes.h.html)

## 13 july 2017
I wrote a string class to learn the internals of std::string. I had to write many operator overloads. There are about 40 operators in C++! I learned that operator + is often implemented in terms of operator +=; <=, >, >= are implemented in terms of <; != calls ==; the postfix ++ operator calls the prefix ++ operator.

This can get somewhat repeative. [Boosts operators library](http://www.boost.org/doc/libs/1_55_0/libs/utility/operators.htm) can help with filling in some of these boilerplate operator overloads. If you write a class like this:

    class MyInt
        : boost::operators<MyInt>
    {
        bool operator<(const MyInt& x) const;
        bool operator==(const MyInt& x) const;
        MyInt& operator+=(const MyInt& x);
        MyInt& operator-=(const MyInt& x);
        MyInt& operator*=(const MyInt& x);
        MyInt& operator/=(const MyInt& x);
        MyInt& operator%=(const MyInt& x);
        MyInt& operator|=(const MyInt& x);
        MyInt& operator&=(const MyInt& x);
        MyInt& operator^=(const MyInt& x);
        MyInt& operator++();
        MyInt& operator--();
    };

then the operators<> template adds more than a dozen additional operators, such as operator>, <=, >=, and (binary) +. Two-argument forms of the templates are also provided to allow interaction with other types.

A string class provides random access iterators. There's about 20 operators that needs to be overloaded for such an iterator. [Boosts iterator library](http://www.boost.org/doc/libs/1_55_0/libs/iterator/doc/index.html) provides the iterator_facade which simplifies the task of creating iterators.

## 15 june 2017
A CPU processes data in word-sized chunks. "Wordsize" is the size of general registers.

https://www.kernel.org/doc/Documentation/unaligned-memory-access.txt
http://lwip.wikia.com/wiki/Porting_for_an_OS
https://blog.regehr.org/archives/1307

## 14 june 2017
For micro benchmarks you sometimes want to run small parts of code in a timer loop. But if the result is unused, then the computation may be optimized out by DCE. One way around that is to put the computatation in a function in another translation unit, but then you record the overhead of the function call as well.

Another solution is to tell the compiler that the value can't be optimized out.  Declaring a variable volatile is one such solution, but it forces the variable to be allocated in memory.

A third solution is to use inline asm. The compiler can not optimize away a store inside inline asm. But then there's the question, whether the variable is allocated in memory or placed in a register. You can use the "r" constraint for storing the variable in a register. That leads to the least pertuberation of the generated code, but it may be so that it still allows the compiler to reorder statements. Using a memory clobber forces is an effective compiler fence.

Here is an [example program](https://godbolt.org/g/iAEDdb) showing all three different ways to force the compiler to not optimize away  variable.

    template <typename T>
    void doNotOptimizeScalar(T val) {
      asm volatile("" : : "r"(val));
    }
    
    template <typename T>
    void doNotOptimize(T const &val) {
      asm volatile("" : : "m"(val) : "memory");
    }
    
    int f(int x, int y) {
      int sum = x  + y;
      doNotOptimizeScalar(sum);
    }
    
    int g(int x, int y) {
      volatile int sum = x + y;
    }
    
    int h(int x, int y) {
      int sum = x + y;
      doNotOptimize(&sum);
    }

Facebooks folly library uses template metaprogramming for overloading doNotOptimizeAway to either place a variable in a register or in memory depending on the size and type of the variable. I wrote [a program that uses doNotOptimizeAway template specialization](https://godbolt.org/g/C8OvGm).

## 12 june 2017
A while back I watched [Chandler Carruth's CppCon 2015 presentation Tuning C++: Benchmarks, and CPUs, and Compilers! Oh My!](https://www.youtube.com/watch?v=nXaxk27zwlk).  He showed code similar to this:

    static void BM_vector_push_back(benchmark::State& state) {
      while (state.KeepRunning()) {
        std::vector<int> v;
        v.reserve(1);
        benchmark::DoNotOptimize(v.data()); // Allow v.data() to be clobbered.
        v.push_back(42);
        benchmark::ClobberMemory(); // Force 42 to be written to memory.
      }
    }

I didn't understand why the ClobberMemory() call would be neccessary after the DoNotOptimize. Today I found a [stackoverflow question Preventing Compiler Optimization While Benchmarking](https://stackoverflow.com/questions/40122141/preventing-compiler-optimizations-while-benchmarking). I interpret the answer as that there are two different optimizationn strategies that come to play here, dead code elimination and dead store elimination.

The scalar replacement of aggregates (SRA) will turn the vector into a len and ptr field. Then each of those two are subject to Dead Code Elimination (DCE).  The DoNotOptimize() call creates an artifical use of the ptr that is returned from data(). So the calls above can't be eliminated.

The push_back call will store a value, but the compiler will eliminate that store if there is no path in the program that could lead to a load. That's why we need ClobberMemory at the end. It tells the compiler to treat all memory as aliased.  Then the store in push_back must be emitted.

Another perspective: A conceptual model for how the compiler treats memory is a connected graph of blocks that begins with a root pointer. DoNotOptimize registers the ptr with that graph. Newly allocated memory are not registered in that graph; that happens upon the first store. That's why we need the DoNotOptimize(v.data()) call after v.reserve().

## 8 june 2017
Started writing a small javascript pomodoro timer SPA. Had to read up on how to use timers, how to maintain state (classes and closures) and how to do integer arithmetic (numbers are floating point in Javascript). Noticed that there's no sprintf or String.format function. This line creates a "%02d" string.

    var str = ('0' + num).slice(-2);

## 7 june 2017
Wrote a string class and had several bugs related to off by ones when accessing the buf, cap and len member variables. The problem was that I needed the string to be nullterminated for data(), but didn't make it clear if that byte was included in the length or capacity. I should have created a [checkRep helper method](http://pgbovine.net/programming-with-rep-invariants.htm) right from the start.

## 5 june 2017
Tcpdump doesn't understand ip fragmentation. If you want to filter on for example udp traffic to port 7, then...

    udp and port 12345

..Won't show fragmented packets. Only the first fragment will have the udp header, and tcpdump operates below the IP layer that does the reassembly. But tcpdump allows us to match against different fields in the IP header, so we can do

    (udp and port 12345) or (ip[6] & 0xe0 == 0x40) or (ip[6] & 0xe0 == 0x00 && ip[6:2] & 0x1f != 0)

The ip[6] & 0xe0 == 0x40 expression matches the More Fragments flag that is sent when IP fragmentation occurs. But the last frame doesn't have that flag set. In order to match the last frame as well, we mask out the offset part and checks if it's non-zero, indicating a fragmented packet.

I've always been intimitaded by complex pcap filters that relies on offsets in the headers and masking out flags. Now I can do it as well!  Wireshark has logic for reassembling IP fragments for us. Use that if possible.

## 2 june 2017
Tcpdump and Wireshark displays relative sequence numbers after the SYN packet in a TCP connection. The acknowledgement number is always one past the last sequence number received.

## 1 june 2017
More networking hazzle. I've been troubleshooting long reconnect times for a service that lost internet access.  I was under the impression that TCP Keep-Alive messages were sprinkled throughout the conversation between two peers. That's not the case. They are only sent when the connection has been idle for some time. The default value is 2 hours!

Through /proc/sys/net/ipv4, you can control time before first keep-alive probe is sent (tcp_keepalive_time), number of probes (tcp_keepalive_probes) and interval between each probe (tcp_keepalive_intvl). Those can also be accessed programmatically as socket options.

I thought that if I set those keepalive settings low enough, then I would detect connection drops timely. But what if we're constantly sending data?  Then, the keepalive mechanism won't trigger. Instead it's up to the retransmission timer to detect when the connection is dropped. The number of retransmits are controlled by tcp_retries2, which defaults to 15. The exact interval between each retry depends on the RTT and the exponential backoff algorithm. Details for all Linux sysctl settings can be found in man tcp.

TCP uses Automatic Repeat reQuests (ARQ) for transmission control. I stumbled upon [RFC 3366 - Advice to link designers on link Automatic Repeat reQuest](https://tools.ietf.org/html/rfc3366) that discusses the tradoffs when implementing some form of sliding window control for telling the transmitter which (if any) packets needs to be retransmitted.

Netstat has an --timer option (-o) that displays a column on the form timername (a/b/c). I've never really understood what those fields meant. Albert Zhangs blog post [Description of netstat timers](http://vzkernel.blogspot.se/2012/09/description-of-netstat-timers.html) dives into the kernel details. The types of timers are on, off, timewait and keepalive. The first field inside the parenthesis is the remaining time for the timer, the second is the number of times the timer has triggered and the last is the number of unanswered zero window probes (only for connected sockets).

## 31 may 2017
I found a [HN conversations about DJB's optimizing compilers talk](https://news.ycombinator.com/item?id=9397169) where Mike Pall, Josh Haberman and DannyBee (Compiler Lead at Google) discusses the limits of what optimizing compilers can achieve. Danny argued that compilers do perform optimizations that humans would miss and that with proper PGO-support it will always outperform a human.  Mike and Josh raised concerns.

Over the last couple of days I've revisited what I learned in my networking courses at the University. I've realized that the RFC's are more readable than the textbooks I was forced to read back then. Today I discovered [RFC 1122 - Requirements for Internet Hosts](https://tools.ietf.org/html/rfc1122) which defines how the different parts of the TCP/IP protocol suite fits together. I also found [RFC 793 - Transmission Control Protocol](https://tools.ietf.org/html/rfc793), that specifies TCP as it is used today (though there are a couple of follow-ups that address optimizations and security hardening, TCP is still focus of much research).

## 30 may 2017
Large packets needs to be fragmented on the IP level to not exceed the Maximal Transfer Unit (MTU). The Internet Protocol header has fields to mark a packet as fragmented and the algorithm for reassembly is outlined in [RFC 815](https://tools.ietf.org/html/rfc815).  But fragmented packets are non-desirable for higher protocols such as TCP, since one lost packet leads to lots of retransmissions. To remedy that, [RFC 1191](https://tools.ietf.org/html/rfc1191) describes an algorithm for doing Path MTU discovery when initializing a connection. The Internet Protocol sends a packet with the MTU of it's closest neighbour and then adjusts the MTU in response to ICMP packets triggered from hosts with smaller MTUs. Unfortunately, some IP hosts filters out ICMP traffic due to perceived security problems. That problem is referred to as Black Hole Detection. One way around it, is to rely on the MSS field of the TCP header, but that breaks the end-to-end principle of the TCP/IP design. See [RFC 2923](https://tools.ietf.org/html/rfc2923) for a discussion of TCP problems with the MTU Patch discovery algorithm.


## 29 may 2017
Started reading up on how do use UDP as transport layer for a protocol.  [RFC 5404](https://tools.ietf.org/html/rfc5405) provides guidelines for application protocol designers. That's a start. Glenn Fiedler, author of Gaffer On Games, has an article series titled [Game Networking](http://gafferongames.com/networking-for-game-programmers/) and another called [Building a Game Network Protocol](http://gafferongames.com/building-a-game-network-protocol/).  Plan 9 has a protocol called RUDP whose implementation can be found in [sys/src/9/ip/rudp.c](http://plan9.bell-labs.com/sources/plan9/sys/src/9/ip/rudp.c). Wikipedia has a [comparison of Transport Layer Protocols](https://en.wikipedia.org/wiki/Transport_layer#Comparison_of_transport_layer_protocols) that gives a good "map of the land". Google has the experimental protocol [QUIC](https://en.wikipedia.org/wiki/QUIC) that uses UDP but provide most of the functionality of TCP but with reduced latency.

Finally, Ilya Grigorik's book High Performance Browser Networking contains a chapter [Building Blocks of UDP](https://hpbn.co/building-blocks-of-udp/) that gives a good overview of what services UDP lacks and how to deal with NAT traversal.

## 24 may 2017
I got curious about how the IP blocks in a SoC communicates. Here's what I found:

System on a chips (SoC) contains microcontrollers, timing sources, memory, external interfaces, peripherals, voltage regulators and analog interfaces.  These blocks are connected through a bus. Arteris has published the article [the SoC Interconnect Fabric](http://www.arteris.com/blog/bid/99797/The-SoC-Interconnect-Fabric-A-Brief-History) that describes how that bus operates. In the beginning was the AMBA bus where one bus master at a time could communicate. An arbiter gives exclusive access to one master at a time. But the arbiters became a bottleneck. Then crossbar switches was invented. Multiple master can access the bus at the same time, but it requires wide datapaths. When the number of masters grew, bridges were added to the cross bar switches. Eventually that didn't scale enough. Then packet-based Network On Chip (NoC) started to be used. Transactions are sent as serialized data on one wire.

As for the AMBA bus, it typically consists of an AHB bus for high speed communications and and APB bus for peripherals. The specifications can be downloaded from the ARM website and is no longer than 20 pages or so.

## 12 may 2017
David P. Reed wrote in an [email to the end2end-interrest mailing list](http://www.postel.org/pipermail/end2end-interest/2005-February/004616.html) that the pseudo header used for calculating the TCP and UDP checksums exists for historical reasons. In the beginning, TCP and IP was one  monolithic protocol. The source and destination address are part of the meaning of the UDP/TCP frame. To avoid duplication, the psuedo header was invented.

There was also an idea that TCP could add end-to-end encryption:

> The plan was to leave the SA and DA in the clear, but encrypt the rest of the TCP payload, including the checksum.  This would protect against a man-n-the-middle attack that delivered valid packets with an incorrect source address, etc. (yes, to be truly reliable, we would have had to use a DSA instead of the current checksum).

> This was a careful design decision, wrecked irrevocably by the terrorists who invented NAT (which doesn't allow end--to-end encryption, because NAT is inherently a "man-in-the-middle" attack!).

## 11 may 2017
I learned a hack for determining endianess from the musl libc:

    bool isLittleEndian() {
        union {int i; char c; } u = { 1 };
        return u.c == 1;
    }

I made good use of [RFC 826](https://tools.ietf.org/html/rfc826) for arp, [RFC 791](https://tools.ietf.org/html/rfc791) for ip and [RFC 792](https://tools.ietf.org/html/rfc792) for icmp. I wish I had those protocol headers memorized.

I also stumbled upon a TCP/IP tutorial published as [RFC 1180](https://tools.ietf.org/html/rfc1180).  I wish I had that document when I started learning networking.

## 10 may 2017
Found Sami Niranens excellent blog serie [Let's code a TCP/IP stack](http://www.saminiir.com/lets-code-tcp-ip-stack-1-ethernet-arp/) It succinctly describes the main parts needed to implement the lower parts: ethernet, arp, icmp, ip. I managed to write a working ArpSend method. One hurdle for me is deciding how to write the buffer API for the stack. Linux and the BSD Unix derivates all have a skbuff structure that holds a buffer that is passed between the layers. But how do I choose which buffer to write to and how do I pass those buffers to the DMA engine?

I read Jason Robert Carrey Patterssons article [Modern microprocessors - a 90 minute guide](http://www.lighterra.com/papers/modernmicroprocessors/). It describes superscalar and pipelined architectures but goes into a lot more detail than the resources I've read before. Some takeaways: Each pipeline step in a processor consists of combinatorical logic and a latch. The clock drives the signals through the grid to the next latch. There are also some bypass paths that provides the results from the execution units before they've been written back to memory. The pipeline depth can vary a lot.

As for issue width, it's not the same as the number of functional units. That was a surprise to me! Issue width describes the typical number of instructions scheduled per clock. It can't always be one ALU, one FPU and one branch for instance. With more functional units it gets easier to saturate the scheduler.

I've been confused about what instruction latencies refers to in the cpu reference manuals. It's the number of cycles needed before the result is available for use. So for instance in a simple 5 stage pipeline. It might take five clocks to get an add instruction from fetched to retired, but due to the bypass stage, it's immediately available to the execution units after the execution step.

The article describes three walls that limits the performance a cpu can achieve: the power wall, the memory wall and the ILP wall. I hadn't thought about the latter before. Due to load latencies, branch misprediction and dependencies between instructions there are limits to how much instruction level parallelism  that can be squeezed out of a cpu.

## 9 may 2017
Peripheral registers for acknowledging, say, interrupts can come in three forms: As one single register that can be modified and read; as a pair of write+read registers or as a register where the read has side effects. I wonder how the later is done on the flip-flop level? How can reading from a register cause side effects?

## 5 may 2017
Switch debouncing times can vary widely. Jack Ganssle has written [a guide to debouncing](http://www.ganssle.com/debouncing.htm) where he measured the bouncing times for 18 different switches. He made 300 presses on each! The median value was 1.5ms, but I should consult the datasheet for my switch to be sure. There were outliers that took tenths of ms!

## 28 apr 2017
An empty std::unordered_map takes up 128 bytes. An empty std::map takes 16.  Bret Wilson [wrote](https://groups.google.com/a/chromium.org/forum/#!topic/chromium-dev/rdxOHKzQmRY):

    A lot of code uses std::unordered_map because "hash tables are faster." But it has high memory overhead for small sizes which most maps are. It's also slightly slower than std::map to insert into, has poor cache coherency (every insert is a malloc), and isn't as much faster at querying over the alternatives than you might expect (it can even be slower!).

Chromium has a document that discusses what map container to use. It can be found [here](https://chromium.googlesource.com/chromium/src/+/master/base/containers/README.md).

## 20 mar 2017
The Linux kernel splits ethernet drivers into MAC drivers and PHY drivers.  There is a fixed-PHY driver that can be used when communicating with network equipment that don't support MDIO, the management interface inside MII.

## 6 mar 2017
You can trigger the 'pure virtual method called' error even if you're not calling virtual method from the base class constructor or destructor. Here's a proof of concept.

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

It introduces a race between the updating of the vtable funtion pointers during destruction and the calling of those methods.

I listened to some more lectures from Barbara Oakleys MOOC "Learning how to learn". She talks about how interleaving different course sections produces better results than just reading them block by block - and she has published results to back up her claims.

## 21 feb 2017
Troubleshooted a wifi performance bug and learnt a few things about the structure of wifi drivers. They can either have a software MAC layer or let the firmware in the device handle the MAC layer. For the former, there's a mac80211 framework in the kernel. Access to the driver by userspace can either be through the data path (regular sockets) or control path (netlink sockets). For the later there's a cfg80211 framework in the kernel and on top of that sits yet another framework for dealing with netlink communication, nl80211.


## 31 jan 2017
For a long time, I've struggled with NetworkManager on my Ubuntu laptops. I want to use two ethernet-interfaces where one should be sharing the internet access and must use a specified ip address range. Today I found that there is a ip4.addresses configuration directive; if I specify an ip address (not a range, just an address) then I can have a shared connection that uses my preferred address range. For more information see this [askubuntu post](http://askubuntu.com/questions/844913/how-to-control-ip-ranges-of-network-managers-hotspots)

## 19 jan 2017
Made some [experiments](https://godbolt.org/g/sQETrj) with how C++ compilers optimize switch statements. I'm starting to get the hang of how br_table is generated. But I still don't understand what bug1316804 is about. What is the "cleanup" that we should avoid?

## 18 jan 2017
Wrestling with the syntax of wast; the S-expression textual format for wasm programs. I'm trying to simplify the generation of wasm jump tables through the br_table opcode. Was a little heureka moment when I finally figured out that labels for the block statement refers to the end of the block, not the beginning.

## 16 jan 2017
[Almost all](https://en.wikipedia.org/wiki/Comparison_of_smartphones) high-end phones uses QualComm SoC's. To match the specfications of a modern smartphone, I need to have ~2GHz clock frequency and atleast 2GB DDR3 memory. The 2016 phones uses ARMv8 cores, but the older are on ARMv7. I want to buy a dev board for running the spidermonkey test suite. Looks like Odroid-C2 gives the best performance/price ratio.

## 12 jan 2017
Watched first half of Cliff Clicks presentation about modern hardware.

## 11 jan 2017
Gdb can call functions but it can't deal with [converting const char* to const string&](http://stackoverflow.com/questions/16734783/in-gdb-i-can-call-some-class-functions-but-others-cannot-be-resolved-why), it needs the arguments to be of the exact right type. Gdb doesn't understand default arguments either.

## 10 jan 2017
The WasmExplorer has a query page. You can use this query for printing a histogram of constant args for i32.mul operations:

    ;; The $ sigil refers to the current expression. You can access the |value| property to
    ;; refer to its text value.
    (i32.mul * (i32.const {histogram("A", parseInt($.value)) }))

## 1 jan 2017
Made an experiment for determine how the  three compilers - icc, gcc and clang - [optimize multiplication by rhs constant](https://godbolt.org/g/LZpca2) They always convert multiplication by 2 to shifts as expected. For rhs < 10, they all generated add+shift. For rhs < 19, icc generates only add+shift but gcc and clang makes exceptions for cases where there are >2 bit set in rhs. They all treat 7, 15, and 31 as shift+sub. For rhs > 33, they all generate imul (except for the previously mentioned case, rhs is a multiple of 2)

So I can do any of these: provide fixed patterns for up to some threshold; optimize powers of two +- 1; optimize all factors with just N bits set by shl+add+shl.

## 30 dec 2016
Division expressions with 1 as divisor [may be emitted by simple compilers](http://logs.glob.uno/?c=mozilla%23ionmonkey&s=30+Dec+2016&e=30+Dec+2016#c127480)

## 27 dec 2016
Counting leading and trailing zeroes can be done by specialised x86 instructions. Bit-scan-reverse (bsr) counts leading zeroes.  I extracted mozilla::Floorlog2 into a [standalone file](https://godbolt.org/g/5zR0Fr) and was surprised to find out to see that all the templates was compiled down into a bsr and a neg. Though I don't know what clang does; how come there's no subtract of 63 or 31?

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
The AMD64 ABI uses rdi, rsi, rdx, rcx, r8, r9, xmm0-7 for passing function parameters. The Linux kernel uses the identical registers, but rcx is replaced by r10. Why? It's because [rcx is clobbered by the syscall instruction](http://stackoverflow.com/questions/38577609/difference-in-abi-between-x86-64-linux-functions-and-syscalls)

In the ARM instruction set, every instruction can be conditionally executed.  That has been [removed in ARMv8](http://stackoverflow.com/questions/22168992/why-are-conditionally-executed-instructions-not-present-in-later-arm-instruction), due to excessive code space usage. I'm looking at [bug 1316822](https://bugzilla.mozilla.org/show_bug.cgi?id=1316822). Looks like there should be something equivalent to setcc. ARMv8 has cset. I wonder what ARMv7 uses?

## 23 dec 2016
Finding a resource that lists most of the linux-related settings that I need to control before a benchmark hasn't been easy. But the CI tools for the Julia language has an wiki page [linuxtips.md](https://github.com/JuliaCI/BenchmarkTools.jl/blob/master/doc/linuxtips.md) that is very useful. Found two useful articles, [Robust benchmarking in noisy enviroments](https://arxiv.org/abs/1608.04295) and [Rigorous benchmarking in reasonable time](https://kar.kent.ac.uk/33611/7/paper.pdf).

I did some experiments with the [box2d benchmark](https://github.com/joelgwebber/bench2d/) compiled for WebAssembly in the [embenchen](https://github.com/kripken/embenchen) benchmark suite. It's supposed to stress floating point operations and is compute intensitive.  Noticed that there are large variantions when running turbo boost was not disabled. But high loads was an even larger factor for large variance.

## 21 dec 2016
Since Intel Sandy Bridge, there's a new cpu kernel driver, intel_pstate. The cpupower userspace tools do not work with that driver; you'll have to read and write to sysfs directly. You can't set a fixed frequency; for that you'd have to disable the intel_pstate driver from the kernel commandline.

There's a [long Google+ thread](https://plus.google.com/+TheodoreTso/posts/2vEekAsG2QT) where Theodore Tso and Arjan van den Veen discuss how Intels p-state management works.

I saw large variations when benchmarking cpu-bound wasm code with 1-10 seconds of runtime. Those went mostly away - range of measurements decreased by a 10x factor - when I disabled the turbo boost feature:

    echo "1" | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo

To inspect the current cpu frequency, this command can be used:

    watch -d grep \"cpu MHz\" /proc/cpuinfo

The powersave governor (the default) can be set with this command

    echo "powersave" | sudo tee /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor

## 20 dec 2016
Benchmarking problems. The cpu frequency varies wildly and I can't change governor or set a fixed frequency. This is because Linux uses the [pstate](https://wiki.archlinux.org/index.php/CPU_frequency_scaling#CPU_frequency_driver) driver on Intel Skylake and newer chips. Found a [redhat bug report](https://bugzilla.redhat.com/show_bug.cgi?id=1378529) that referenced a blog series, [Haypos notes on microbenchmarking](http://haypo-notes.readthedocs.io/microbenchmark.html)

## 17 dec 2016
Spidermonkey uses the terms nunboxing and punboxing for referring to the javascript value representation. Rob Sayre has written a good [overview](https://evilpie.github.io/sayrer-fatval-backup/cache.aspx.htm) and Andy Wingo has written an article, [value representation in javascript implementations](https://wingolog.org/archives/2011/05/18/value-representation-in-javascript-implementations) that contrast the Spidermonkey implementation with other js engines.

## 16 dec 2016
I've had problems when simultaneously installing multilib and cross compilers; gcc-multilib installs a /usr/include/asm symlink that messes up the include path for cross compilers. Hence, the deb packages for the cross compilers conflicts with gcc-multilib. These things make me so tired. I just don't want to understand. I want it to work. Luckily enough, it turns out that x86 gcc has /usr/local/include as search path while the cross-compilers don't. So I installed a symlink from /usr/local/include/asm to /usr/include/x86_64-linux-gnu/asm.

Managed to build the spidermonkey shell for x86, arm, x63 and arm64. Here are the configure commandlines:

    PKG_CONFIG_LIBDIR=/usr/lib/pkgconfig CC="gcc -m32" CXX="g++ -m32" AR=ar ../configure --target=i686-pc-linux --enable-debug --disable-optimize --without-intl-api
    AR=ar CC="gcc -m32" CXX="g++ -m32" ../configure --target=i686-pc-linux --enable-debug --disable-optimize --enable-simulator=arm --without-intl-api
    ../configure --enable-debug --disable-optimize --without-intl-api
    ../configure --enable-debug --disable-optimize --enable-simulator=arm64 --without-intl-api

## 15 dec 2016
Found Thomas Ptaceks [Application Security Reading List](https://www.amazon.com/gp/richpub/listmania/fullview/R2EN4JTQOCHNBA). A few books that I have high thoughts of like C Programming interfaces, The Practice of Programming and Javascript - the Good Parts; and then a dozen books that were unknown to me. I've added TOASSA to my TODO-reading list. Reversing: the Secrets of Reverse Engineering looks promising as well. Might be useful when I try to decipher Spidermonkey JIT code.

## 14 dec 2016
You can build a [40-hw thread xeon machine](http://www.techspot.com/review/1218-affordable-40-thread-xeon-monster-pc/) for about $1000. I have a 8-hw thread i7 Dell XPS 15 laptop. State of the art of 2016, but it still takes *a lot of time* to build the firefox source tree.  Sooner or later, I will buy a PC with enough cores (though I've been saying that for at least five years without much happening).

There's an environment variable that can be set to control how [Qt5 deals with high DPI displays](https://blog.qt.io/blog/2016/01/26/high-dpi-support-in-qt-5-6/).  Up until now I've just accepted that QtCreator looks horrible on my laptop. Those days are over! Next step: Fix Firefox too.

Wrote a small python script that mimics the golang net.Listen and net.Dial functions. The BSD socket API is way too low-level. I have to look up the functions each time I'm writing a small script.

Found QStandardItemModel; much simpler than inheriting from QAbstractItemModel.

Until now I hadn't ever messed up operator priorities in macros. But now I did big time.

    #define P(x, y) do { printf("%d %d %d\n", x, y, x/y); } while (0)

Always put parenthesis around every variable in a macro. Correct version:

    #define P(x, y) do { printf("%d %d %d\n", (x), (y), (x) / (y)); } while (0)
