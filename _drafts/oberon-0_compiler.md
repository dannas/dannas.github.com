---
layout: post
title: Let's write a Oberon-0 compiler
---

## What is a Compiler

* Takes a a string of text and a grammar as input
* Consumes the text
* Checks if the program is "well-formed"
* Translates source text to instructions sequences
* The text is decomposed into its components according to the given syntax

## The Compilation steps

* Scan for tokens - lexing
* Scan for subcomponents  - parsing
* Verify that the subcomponents matches certain rules - Type checking
* Generate code

## Language and Syntax

A grammar consists of syntax rules, or productions.

The set of sentences that can be generated from the productions is the **language**.



A language is defined by:

* The set of terminal symbols
* The set of non-terminal symbols
* The set of syntactic equations
* The start symbol

## Backus-Naur form

```
syntax     = production syntax | ∅.
production = identifier "=" expression "." .
expression = term | expression "|" term.
term       = factor | term factor.
factor     = identifier | string.
identifier = letter | identifier letter | identifier digit.
string     = stringhead """.
stringhead = """ | stringhead character.

letter     = "A" | ... | "Z".
digit      = "0" | ... | "9".
```

Extended Backus-Naur form simplifies the expression of recursion.

## Regular Languages

