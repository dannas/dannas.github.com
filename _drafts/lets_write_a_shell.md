---
layout: post
title: Let's write a shell
---

## Steps

I'll start of with the csapp-book lab

- parsing
- exec fg
- exec bg
- jobs
- sighcild reaping

If I have time then I'll continue with.

- pipelines
- fd redirection
- handle quoting
- process groups
- sessions
- exit values
- list of commands separated by ';'
- sub shells '(' and ')'
- process substitution `$(...)`
- command substitution `<(...)`
- HERE docs
- globbing
- tilde expansion `~` is expanded to `/home/$USER/`
- disown SIGHUP
- PS1 and PS2 prompt
- line editing
- history
- completion

## Links related to signal handling

* https://ldpreload.com/blog/signalfd-is-useless and a very useful associated HN discussion
* http://web.cecs.pdx.edu/~jrb/cs201/lectures/handouts/onwait.txt
* https://netfuture.ch/2016/02/pselect-pitfalls/
* From LWN.net [Attaching file descriptors to processes with CLONE_FD](https://lwn.net/Articles/638613/)
* https://cr.yp.to/docs/selfpipe.html
* From LWN.net [The new Pselect System Call](https://lwn.net/Articles/176911/)

* FreeBSD has pdfork https://www.freebsd.org/cgi/man.cgi?query=pdfork&sektion=2
* From LWN.net [Ghosts of UNIX pasts](https://lwn.net/Articles/414618/)

* From LWN.net [Ghosts of UNIX pasts signalfd comment](https://lwn.net/Articles/415684/)
* [EINTR and what it is good for](http://250bpm.com/blog:12)

## Some relevant links

The xv6 shell is a good starting point for redirection and pipelines

* https://pdos.csail.mit.edu/6.828/2018/homework/xv6-shell.html
* https://github.com/tokenrove/build-your-own-shell
* https://www.cipht.net/2017/10/17/build-your-own-shell.html
* http://www.aosabook.org/en/bash.html
* https://www.gnu.org/software/libc/manual/html_node/Implementing-a-Shell.html#Implementing-a-Shell
* https://blog.nelhage.com/2010/01/a-brief-introduction-to-termios-signaling-and-job-control/
* https://www.cons.org/cracauer/sigint.html

* http://www.linusakesson.net/programming/tty/

