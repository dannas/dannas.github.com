---
layout: post
title: Paper&#58; A fork() in the road
---



[Baumann, Andrew, et al. "A fork () in the road." *Proceedings of the Workshop on Hot Topics in Operating Systems*. 2019](https://www.microsoft.com/en-us/research/uploads/prod/2019/04/fork-hotos19.pdf).



## Buffered I/O and Fork

One critique of `fork` is that it doesn't compose, correct fragments may fail when combined with fork. What do you think this program will print?

```
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    printf("Hello World! ");
    pid_t pid = fork();
    return 0;
}
```

It prints the output twice, `Hello World! Hello World!`, since the stdio buffers are flushed both in the parent and in the child. So `fork` doesn't compose, since the calling code needs to handle buffers differently when it is called.

## Threads and Fork

Here's another example of how `fork` fails to compose. This program prints `_` every time a new subprocess has been created and `x` when that subprocess hasn't exited within 1 second. What do you think it will print and why?

```
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

static pthread_mutex_t thread_mutex = PTHREAD_MUTEX_INITIALIZER;

static void* worker(void* arg) {
    pthread_detach(pthread_self());

    for (;;) {
      pthread_mutex_lock(&thread_mutex);
      usleep(1000);
      pthread_mutex_unlock(&thread_mutex);
    }
    return NULL;
}

int main() {
    pthread_t thread;
    pthread_create(&thread, NULL, worker, 0);

    usleep(1000);
    pid_t pid = fork();

    if (pid == 0) {
        pthread_mutex_lock(&thread_mutex);
        const char buf[] = "lock acquired\n";
        write(fileno(stderr), buf, strlen(buf));
        pthread_mutex_unlock(&thread_mutex);
        exit(0);
    }

    wait3(NULL, WNOHANG, NULL);

    return 0;
}
```

On my system the subprocess always deadlocks before printing the `lock acquired` message.

## Large address spaces and Fork

https://bugs.chromium.org/p/chromium/issues/detail?id=819228

## Heterogeneous Address Spaces and Fork



## Using Fork for Copy-On-Write







* Fork is considered a good design, but is it?
* not thread-safe
* inefficient and unscalable
* security concerns
* lost its original simplicity - no longer orthognal but affects everything
* conflates processes and their address spaces - hostile to user-mode impl of OS functionality
* fork doesn't compose - every layer must support it
* non monolith kernels needs to duplicated per-process state lazily
## Fork began as a hack
PDP-7 shortcut due to lack of virtual memory

## Advantages
* fork was simple: no arguments
* creating a process was orthgonal to starting a process. Could be used for remapping fd:s and reduce permissions.
* Concurrency via pre-forking servers as was used in the past

## Fork in the modern era
* No longer simple. 25 special cases in the POSIX spec: file locks; timers; async I/O; tracing...
* Other API:s affected: madvise MADV_DONTFORK/DOFORK/WIPEONFORK, file descriptors O_CLOEXEC, pthread_atfork
* user-mode  library must be prepared for their state to be forked at any time
* fork doesn't compose: Buffered I/O must explicitly flush prior to for, or it's duplicated
* fork isn't thread-safe: malloc lock will deadlock if child also allocates
* fork is insecure: must close fd; isolating namespaces; scrub memory from secrets
* fork is slow: even copy-on-write can be slow (chrome saw 100ms delays, node.js blocked for seconds)
## Resources

* https://blog.famzah.net/2009/11/20/a-much-faster-popen-and-system-implementation-for-linux/
* https://sourceware.org/git/gitweb.cgi?p=glibc.git;a=commitdiff;h=9ff72da471a509a8c19791efe469f47fa6977410;hp=1eb8930608705702d5746e5491bab4e4429fcb83
* posix_spawn should use vfork() in more cases than presently https://sourceware.org/bugzilla/show_bug.cgi?id=10354
* https://github.com/famzah/popen-noshell/issues/11
* https://gist.github.com/nicowilliams/a8a07b0fc75df05f684c23c18d7db234
* https://www.microsoft.com/en-us/research/publication/a-fork-in-the-road/
* https://sigsegv.me/2013/12/02/why-threads-and-fork-dont-mix/
* [vfork considered dangerous](http://ewontfix.com/7/)
* close-on-fork https://lwn.net/ml/linux-kernel/20200515152321.9280-1-nate.karstens@garmin.com/