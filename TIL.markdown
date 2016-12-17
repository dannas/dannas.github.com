# Today I've learned

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
