---
layout: post
title: An introduction to linking
---

## Fundamental concepts
Linking is done at build time and is then called static linking. At runtime it's referred to as dynamic linking, or loading.

During static linking the linker first builds a symbol table of all defined and called functions. It then makes sure that all referenced symbols have definitions. After that the linker assigns addresses to the sections and
offsets for the symbols within each section.

For position-independent code, the symbols can't be given an address. Instead the functions are recorded in a PLT-section and the global variables are recorded in GOT-sections.

## Loading of position-independent code

TODO

## Symbol visibility

The .symtab-section contains all global and local symbols. From the view of a linker, global symbols are the functions that are not marked static and the non-static global variables. The static functions and variables are referred to as locals. Not that local C variables are not handled by the linker, they are handled by the compiler and is allocated in registers or on the stack.

## Library versioning

A library is versioned using the soname. It's recorded in the shared library, and is used by the dynamic linker for finding what library to load. There should exist a symbolic file system link for the soname of  a library which points to the actual library object file. 

The GNU gcc compiler provides extensions for managing versioning of individual symbols.

## Relocatable object files

Some system ABi:s allow relocation at runtime (x86) and some don't (x86_64). For x86_64 relocation information is needed nevertheless during static linking. An external undefined function will be assigned to a relocation block during compilation. Then during static linking a PLT and GOT entry will be created from that relocation entry.

## Information to digest

* https://gist.github.com/DhavalKapil/2243db1b732b211d0c16fd5d9140ab0b
* [GNU Hash section format](https://sourceware.org/ml/binutils/2006-10/msg00377.html)
* [overlinking](https://wiki.mageia.org/en/Overlinking_issues_in_packaging)
* [kickers of ELF](http://www.muppetlabs.com/~breadbox/software/elfkickers.html)
* [Bloaty McBloatface ELF parser](https://github.com/google/bloaty/blob/master/src/elf.cc)
* [gimli/object elf.rs](https://github.com/gimli-rs/object/blob/master/src/elf.rs)
* [m4b/goblin src/elf](https://github.com/m4b/goblin/tree/master/src/elf)
* 

