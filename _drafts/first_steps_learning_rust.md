---
layout: post
title: First Steps Learning Rust
---

## First acquaintance

* rust-koans
* rustlings
* Steve Donovans [Gentle Introduction to Rust](https://stevedonovan.github.io/rust-gentle-intro/readme.html)
* Nick Camerons [Rust for C++ Programmers](https://github.com/nrc/r4cppp#contents)
* https://doc.rust-lang.org/stable/rust-by-example/
* The book [The Rust Programming Language](https://doc.rust-lang.org/book/)
* Jim Blandys book Programming Rust

## First things I've learned

* borrow operator can coerce arrays  into slices
* borrow operator can coerces vectors  into slices
* borrow operator can coerce strings into string slices
* iterators can be converted to containers via collect()

## First projects

The programs from Software Tools in Pascal are good self-contained start programs.

* echo
* cat
* wc
* sort
* parse json
* s-expression
* rpn calculator
* translit (tr)
* copy (cp)
* grep
* archive (ar)
* diff and compare (cmp)
* unique (uniq)
* kwic
* edit (ed)
* format (troff)
* macro processor (define)

## Performance

From [Perf Pitfalls](https://llogiq.github.io/2017/06/01/perf-pitfalls.html)

* default is compilation in debug mode
* default is unbuffered I/O
* you can avoid the cost of a lock for each stdio call by holding the mutex for the duration of all I/O
* Read::lines() iterator allocates a String for each line
* Use read_line and your own preallocated string instead (which you clear in between calls to read_line).
* strings are always encoded as utf-8. You pay a cost where the string is traversed for checking of utf-8 validity. You can use raw arrays or vectors instead.
* Make sure you don't call collect() unneccesary
* Look out for needless allocations. Make sure you understand your data layout and use slices instead of String/Vec. For static data prefer an Array.