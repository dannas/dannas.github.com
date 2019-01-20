---
layout: post
title: Hashing
---

## History of hash functions

P(31) from K&R. They didn't remember where they got it from
https://bugs.java.com/bugdatabase/view_bug.do?bug_id=4045622

djb2 37 choosen since easy on hw.
https://stackoverflow.com/questions/10696223/reason-for-5381-number-in-djb-hash-function/13809282#13809282

## Designing hash functions

Thomas Wang Integer Hash Function copy-paste from archive.org: https://gist.github.com/badboy/6267743

http://ticki.github.io/blog/designing-a-good-non-cryptographic-hash-function/

http://ticki.github.io/blog/a-general-construction-for-rolling-hash-functions/

http://ticki.github.io/blog/seahash-explained/

## Benchmarking hash functions

http://aras-p.info/blog/2016/08/02/Hash-Functions-all-the-way-down/

http://aras-p.info/blog/2016/08/09/More-Hash-Function-Tests/

## Comparing hash_uint64 vs murmurmash3

Per Vognsens hash functions from bitwise:

```
uint64_t hash_uint64(uint64_t x) {
    x *= 0xff51afd7ed558ccd;
    x ^= x >> 32;
    return x;
}

uint64_t hash_ptr(const void *ptr) {
    return hash_uint64((uintptr_t)ptr);
}

uint64_t hash_mix(uint64_t x, uint64_t y) {
    x ^= y;
    x *= 0xff51afd7ed558ccd;
    x ^= x >> 32;
    return x;
}

uint64_t hash_bytes(const void *ptr, size_t len) {
    uint64_t x = 0xcbf29ce484222325;
    const char *buf = (const char *)ptr;
    for (size_t i = 0; i < len; i++) {
        x ^= buf[i];
        x *= 0x100000001b3;
        x ^= x >> 32;
    }
    return x;
}

```

