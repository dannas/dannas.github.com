---
llayout: post
title: Let's write an ELF parser
---

## Steps

* Parse the elf hdr
* Parse the segment hdr
* Parse the section hdr
* Parse the string tables
* Parse the symbol tables

## The nm tool

* First verify the elf hdr
* Then find the section hdr table
* Find the .shstrtab that holds the strings for the section names
* Find the .symtab and .strtab by matching against section names
* Store pointers to all symbol entries and sort them by name
* Determine type by checking the section header flags for each symbol: is it instructions, constants, initialized data or uninitialized data? 

## Caveats

* fuzzing often use binutils as examples since there are so many edge cases