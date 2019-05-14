---
layout: post
title: Let's Implement System
---

## The standard

https://pubs.opengroup.org/onlinepubs/9699919799/functions/system.html

> Ignoring SIGINT and SIGQUIT in the parent process prevents coordination problems (two processes reading from the same terminal, for example) when the executed command ignores or catches one of the signals. It is also usually the correct action when the user has given a command to the application to be executed synchronously (as in the `'!'` command in many interactive applications). In either case, the signal should be delivered only to the child process, not to the application itself. There is one situation where ignoring the signals might have less than the desired effect. This is when the application uses *system*() to perform some task invisible to the user. If the user typed the interrupt character ( `"^C"`, for example) while*system*() is being used in this way, one would expect the application to be killed, but only the executed command is killed. Applications that use *system*() in this way should carefully check the return status from *system*() to see if the executed command was successful, and should take appropriate action when the command fails.

> Blocking SIGCHLD while waiting for the child to terminate prevents the application from catching the signal and obtaining status from *system*()'s child process before *system*() can get the status itself.

## Call libc system

* Discuss input sanitizing

## Close files

* TAOSSA 7.6.4 discusses file descriptor leaks
* Loop over fdtablesize, but what if a thread has set RLIMIT?

## Drop Privilegies

* setgid
* Check return values seteuid to avoid https://thesnkchrmr.wordpress.com/2011/03/24/rageagainstthecage/
* setuid
* prctl(PR_SET_NO_NEW_PRIVS, ...)

## Sanitize environment

* TAOSSA 7.6.4 discusses env
* Can't call malloc after fork

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
* https://www.oreilly.com/library/view/secure-programming-cookbook/0596003943/ch01s07.html chapter 1.7
* https://www.oreilly.com/library/view/secure-programming-cookbook/0596003943/ch01s06.html chapter 1.6
* https://www.oreilly.com/library/view/secure-programming-cookbook/0596003943/ch01s05.html chapter 1.5
* https://www.oreilly.com/library/view/secure-programming-cookbook/0596003943/ch01s01.html chapter 1.1 sanitize environment
* https://www.oreilly.com/library/view/secure-programming-cookbook/0596003943/ch01s03.html chapter 1.3 dropping privilegies
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

