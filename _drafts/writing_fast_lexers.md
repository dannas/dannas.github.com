---
layout: post
title: Writing Fast Lexers
---

## How measure

http://aras-p.info/blog/2019/01/12/Investigating-compile-times-and-Clang-ftime-report/

gcc -ftime-report

## Prior Art

* Sean Barrett has written [Some Strategies for Fast Lexical Analysis when Parsing Languages](http://nothings.org/computer/lexing.html)
* Per Vognsen writes a lexer for C in [Bitwise Day 2: C Programming and Parsing](https://bitwise.handmade.network/episode/bitwise/bitwise002/) and [Bitwise Day 3: More Programming and Parsing](https://bitwise.handmade.network/episode/bitwise/bitwise003/)
* Dave Hansens lexer for lcc, a retargetable compiler can be found in [lex.c in the lcc github repository](https://github.com/drh/lcc/blob/master/src/lex.c)
* The Tiny CC compiler by Fabrice Bellard
* Rob Pikes blogpost [Regular Expressions in Lexing and Parsing](https://commandcenter.blogspot.com/2011/08/regular-expressions-in-lexing-and.html)
* Rob Pikes [presentation at Google FOSS Sydey 2011 Lexical Scanning in Go](https://www.youtube.com/watch?v=HxaD_trXwRE). Not designed to be fast but an example of how to write a lexer with coroutines
* The [Ragel State Machine Compiler](http://www.colm.net/open-source/ragel/)
* David Hermans [Javacript lexer Joker written in Rust](https://github.com/dherman/esprit/blob/master/crates/joker/src/lexer.rs). Maybe not fast but an example of a Rust lexer



## Very slow

* Write a grammar and tokenizer in Python using regular expressions Like Peter Norvig does here https://classroom.udacity.com/courses/cs212/lessons/48738183/concepts/486649850923
* Write lexer using lex and flex
* Write lexer using coroutines
* Write lexer using if-statements
* Write lexer using switch-statement
* Write lexer that does string interning
* Write zero-copy lexer