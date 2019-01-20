---
layout: post
title: Reading binary data fast
---

## Using fread

* fread uses the stdio buffers
* stdio has a default value of BUFSIZ=4096
* reading buffered data does not cause a system call
* setvbuf can be used for providing your own buffer
* when are the buffers flushed after an fseek?

## Using memcpy

* There may be alignment requirements on the data
* Using memcpy avoid possible problems with fread causing undesired reads

## Using struct overlays

* As in Richard Stevens TCP/IP the implementation
* Typecast the char* buffer to your struct
* Requires that the the indata is properly aligned
* Requires that the struct has no padding
* Requires that the struct is properly aligned
* Requires that the endianess in the indata and the struct matches
* The ethernet header is 14 bytes, that's 2 bytes less to make ip header properly aligned
* Many network stacks lets the DMA engine point to a location that is off by 2 bytes
* Then ip is aligned, but requires DMA to unaligned addresses to be allowed and efficient
* An ELF file has headers that are properly aligned
* The Linux kernel loader reads the header into the struct directly
* But that won't work if we intend on reading an ELF file that is not for the host platform

## Zero copy serialization

* Capn'Proto is described as zero copy https://capnproto.org/
* Flatbuffers is zero-copy https://google.github.io/flatbuffers/flatbuffers_white_paper.html
* https://ithare.com/marshalling-and-encodings/
* SBE https://mechanical-sympathy.blogspot.com/2014/05/simple-binary-encoding.html
* MsgPack https://gist.github.com/frsyuki/2908191
* https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.io.zero_copy_stream

## Examples

* qelfparser
* https://gafferongames.com/post/serialization_strategies/
* https://capnproto.org/
* http://cbor.io/
* https://en.wikipedia.org/wiki/Comparison_of_data_serialization_formats
* http://blog.qt.io/blog/2018/05/31/serialization-in-and-with-qt/
* http://ithare.com/ultra-fast-serialization-of-c-objects/
* http://ithare.com/marshalling-and-encodings/
* http://ithare.com/idl-encodings-mappings-and-backward-compatibility/
* [010 Editor has binary templates for struct overlays](http://www.sweetscape.com/010editor/templates.html)
* 