---
layout: post
title: First Steps Learning Rust
---

* Joe Duffy on errors in [The Error Model](http://joeduffyblog.com/2016/02/07/the-error-model/)

* Eric Lipperts take in [Vexing Exceptions](https://blogs.msdn.microsoft.com/ericlippert/2008/09/10/vexing-exceptions/)

* Rob Pike in [Errors are Values](https://blog.golang.org/errors-are-values)

* Another article from Rob Pike [Error Handling in Upspin](https://commandcenter.blogspot.com/2017/12/error-handling-in-upspin.html)

* https://stevedonovan.github.io/rust-gentle-intro/6-error-handling.html

* Per Vognsen discusses syntac structures for error handling and how avoiding fine-grained error-handling simplifies the design. Things like having sets of open files and arenas for allocations allows non-local jumps to a sync-point (setjmp/longjmp) similar to exceptions. https://bitwise.handmade.network/episode/bitwise/bitwise006/#5537 https://bitwise.handmade.network/episode/bitwise/bitwise006/#5855 https://bitwise.handmade.network/episode/bitwise/bitwise006/#6062

* Fabien Giesen describes in [an aside for a gist note](https://gist.github.com/anonymous/2edc9e9d52a93c126ff486cfb4a2c65b) how he views error handling. He points out that it may be beneficial to only provide a small set of error codes and that the selection of those should be dictated by
  the question "what should I do next?". E.g. there are many ways a network connection can fail but providing a giant taxonomy of error codes won't help the calling code to decide what to do. Logging should be as specific as possible but the users of an API just needs to decide what to do next.

  Fabien wrote [in a blog comment](http://cbloomrants.blogspot.se/2010/06/06-07-10-exceptions.html?showComment=1275971007229#c4849051810850075252) that having stack unwinding do the cleanup on errors is a bad design that costs lots of resources and is hard to control.

  One problem I've had with error handling is that it introduces a lot of noise that hides the main  computation in the source code. Fabien suggests in the article [Buffer Centric I/O](https://fgiesen.wordpress.com/2011/11/21/buffer-centric-io/) that you can use something similar to the null object pattern that just does nothing after the first error has been encountered. Rob Pike describes a similar approach in [Errors are Values](https://blog.golang.org/errors-are-values).



