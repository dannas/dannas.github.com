---
layout: post
title: Let's Implement System
---

As a learning exercise for myself, I was writing a shell and started thinking about the security implications of running a subprocess from within a process. Here are my findings. I'll write a function that executes a program and blocks for the duration of it's execution, like `system(3)`.

## A first unsafe implementation of system(3)

A basic implementation of `system` will call `fork` then do an `exec` of `sh` in the child and call `waitpid`  in the parent. 

```
int system1(const char *cmd) {
    int status;
    pid_t pid;

    switch (pid = fork()) {
    case -1:
        return -1;
    case 0:
        execl("/bin/sh", "sh", "-c", (char **)cmd, NULL);
        _exit(127);

    default:
        if (waitpid(pid, &status, 0) == -1) {
            return -1;
        }
    }
    return status;
}
```

The [system(3)](https://pubs.opengroup.org/onlinepubs/9699919799/functions/system.html) standard specifies that `SIGCHLD` should be blocked in the parent and that `SIGINT` and `SIGQUIT` should be ignored. This leads to a much more verbose implementation. I will base future changes on `system1` to to improve readability.

```
int system2(const char *cmd) {
    int status;
    pid_t pid;
    sigset_t block_mask;
    sigset_t orig_mask;
    struct sigaction sa_ignore;
    struct sigaction sa_orig_quit;
    struct sigaction sa_orig_int;
    struct sigaction sa_default;
    int saved_errno;

    // Block SIGCHLD
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &block_mask, &orig_mask);

    // Ignore SIGINT and SIGQUIT
    sa_ignore.sa_handler = SIG_IGN;
    sa_ignore.sa_flags = 0;
    sigemptyset(&sa_ignore.sa_mask);
    sigaction(SIGINT, &sa_ignore, &sa_orig_int);
    sigaction(SIGQUIT, &sa_ignore, &sa_orig_quit);

    switch (pid = fork()) {
    case -1:
        status = -1;
        break;
    case 0:
        sa_default.sa_handler = SIG_DFL;
        sa_default.sa_flags = 0;
        sigemptyset(&sa_default.sa_mask);

        if (sa_orig_int.sa_handler != SIG_IGN) {
            sigaction(SIGINT, &sa_default, NULL);
        }
        if (sa_orig_quit.sa_handler != SIG_IGN) {
            sigaction(SIGQUIT, &sa_default, NULL);
        }
        sigprocmask(SIG_SETMASK, &orig_mask, NULL);

        execl("/bin/sh", "sh", "-c", (char **)cmd, NULL);
        _exit(127);
        break;
    default:
        if (waitpid(pid, &status, 0) == -1) {
            return -1;
        }
        break;
    }

    saved_errno = errno;

    sigprocmask(SIG_SETMASK, &orig_mask, NULL);
    sigaction(SIGINT, &sa_orig_int, NULL);
    sigaction(SIGQUIT, &sa_orig_quit, NULL);

    errno = saved_errno;

    return status;
}
```

## Ensure files are closed

File descriptors are shared across `fork+exec`. Any files that has been opened in the parent remains open in the child, unless the file was opened with the `O_CLOEXEC` flag set.  If the parent calls a program with `/etc/passwd` open, then it may be accessible to the child as well.

```
int main() {
    int fd = open("/etc/passwd", O_RDONLY);
    (void)fd;
    system("./exploit_fd");
}
```

The child can then read or write to the file depending on what mode it was opened with in the parent. For the program above, the `fd` for `/etc/passwd` will be `3` so this program will write the content of the file to `stdout`.

```
int main() {
    char buf[BUFSIZ]; 
    ssize_t num_read = read(3, buf, BUFSIZ);
    write(1, buf, num_read);
    return 0;
}
```

To avoid file descriptor leaks we need to close all descriptors except for number 0, 1 and 2. Those are `stdin`, `stdout` and `stderr` and are required to be open. If they are closed, then functions such as `printf` will fail and there often isn't any error handling for those. Worse, if say `stderr` is closed and a new file is opened then that file will be assigned the lowest available file descriptor in the system, in this case 2. So code that writes to `stderr` will instead write to the unrelated file.

Figuring out how many files are opened by a process is not easy on Linux. The Posix standard defines the static constant`OPEN_MAX` but it can be as low as 20.  The dynamic `_SC_OPEN_MAX` has a more sensible value but it can give the wrong value if `setrlimit(RT_NOFILE)` has been called with a lower value. And on some systems it has a huge value, causing a large number of close calls to be issued. Traversing the directory `/proc/self/fd` is a third option but the API:s `opendir` and `readdir` are not async-signal safe and should not be used between `fork` and `exec` (more on the async signal-safe requirement below).

```
int system3(const char *cmd) {
    // ...
    case 0: {

        // Determine max number of open files.
        // _SC_OPEN_MAX is bound by the resource limit RT_NOFILE and may
        // change during runtime.
        int num_fd = sysconf(_SC_OPEN_MAX);
        if (num_fd ==-1) {
            num_fd = OPEN_MAX;
        }

        for (int fd = 3; fd < num_fd; fd++) {
            close(fd);
        }
    // ...
}
```

* https://gitlab.freedesktop.org/libbsd/libbsd/blob/master/src/closefrom.c
*  [**Bug 10353**](https://sourceware.org/bugzilla/show_bug.cgi?id=10353) - Methods for deleting all file descriptors greater than given integer (closefrom)    
* https://stackoverflow.com/questions/899038/getting-the-highest-allocated-file-descriptor
* https://stackoverflow.com/questions/18513454/is-there-an-async-signal-safe-way-of-reading-a-directory-listing-on-linux
* getdtablesize
* https://github.com/python/cpython/blob/9e4f2f3a6b8ee995c365e86d976937c141d867f8/Modules/_posixsubprocess.c#L261
* TAOSSA 7.6.4 discusses file descriptor leaks

## Drop Privilegies

Linux has an all or nothing security model. Either you're root and are allowed to do everything on the system, or you're a regular user. A process inherits its user id and group id from its parent, unless the setuid flag is set on the executable. Then it's running as whatever user owns the file.

You should drop all privilegies that are not needed when calling `exec`.  The API for that is not cross platform and has a certain number of wrinkles which mostly have been smoothed out on linux with the introduction of `setreuid` and `setregid`.

* Each file has an owner. 

* Each file has permissions. Those dictate what the owner, group and others can do with the file.

* A process inherits the user-id from its parent (it's not the owner set on the executable)

* But it can be overridden if the set-user bit is set on the executable file.

* Privilege checks are done against the effective user id.
* A process also has a saved user id.
* There are also ancillary groups



- setgid
- Check return values seteuid to avoid https://thesnkchrmr.wordpress.com/2011/03/24/rageagainstthecage/
- setuid
- prctl(PR_SET_NO_NEW_PRIVS, ...)

## Sanitize environment

- TAOSSA 7.6.4 discusses env
- Can't call malloc after fork

## Call libc system

* Discuss input sanitizing

* 

## Block signals during fork

* To avoid race condition parent/child when child kills signal handlers.

## Ensure that killed subprograms get reaped

Use prctl(PR_SET_PDEATHSIG, ...) and possibly prctl(PR_SET_CHILD_REAPER)



## The Security Implications

* https://wiki.sei.cmu.edu/confluence/pages/viewpage.action?pageId=87152177
* http://redhat-crypto.gitlab.io/defensive-coding-guide/#sect-Defensive_Coding-Shell-Edit_Guard

* A list of vulnerabilities https://www.security-database.com/cwe.php?name=CWE-88
* Another list https://www.security-database.com/cwe.php?name=CWE-78

* https://www.exploit-db.com/papers/13197 about returning into libc attacks

* [Don't mix Threads and Forks](https://rachelbythebay.com/w/2011/06/07/forked/)
* https://chriswarrick.com/blog/2017/09/02/spawning-subprocesses-smartly-and-securely/
* Fork-one safety problems related to locks (pthread_atfork workaround) https://docs.oracle.com/cd/E19455-01/806-5257/6je9h0334/index.html#gen-92888
* The Art of Software Security Assessment chapter 10 UNIX processes

## The Implementation

* musl libc
* glibc
* https://git.uclibc.org/uClibc/tree/libc/stdlib/system.c
* https://github.com/ensc/dietlibc/blob/6be0ee4bb020b51d7e24ca90e4c8f86e9d11e6dd/libugly/system.c
* https://code.woboq.org/qt5/qtbase/src/corelib/io/qprocess.cpp.html
* chromium launch_posix.cc
* folly subprocess
* https://searchfox.org/mozilla-central/source/ipc/chromium/src/base/process_util_linux.cc
* https://searchfox.org/mozilla-central/source/gfx/angle/checkout/src/common/system_utils_posix.cpp
* libvirt https://github.com/libvirt/libvirt/blob/master/src/util/vircommand.c
* systemd exec_spawn https://github.com/systemd/systemd/blob/5238e9575906297608ff802a27e2ff9effa3b338/src/core/execute.c#L3716
* Chrome and the zygote process http://neugierig.org/software/chromium/notes/2011/08/zygote.html and https://chromium.googlesource.com/chromium/src/+/HEAD/docs/linux_zygote.md

