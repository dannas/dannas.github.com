---
layout: default
title: TIL - Today I've Learned
---
# Today I've learned

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
