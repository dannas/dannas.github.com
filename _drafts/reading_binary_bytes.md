---
layout: post
title: Reading binary bytes
---



https://godbolt.org/z/odxDne

// Some ways to read ints from a big-endian bytestream.

// I was surprised to see that they all generate the same assembly code.

```
// The portable clean way, see Rob Pikes article

// https://commandcenter.blogspot.com/2012/04/byte-order-fallacy.html

uint32_t r1(uint8_t *a) {
   return (a[3] << 0) | (a[2] << 8) | (a[1] << 16) | (a[0] << 24); 
}
```

// Casting through pointers is not safe. It breaks strict aliasing and a is not guarenteed 

// to be aligned on 4 bytes. The same holds for C++ static_cast and reinterpret_cast.

```
uint32_t r2(uint8_t *a) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return bswap_32(*(uint32_t*)a);
#else
    return *(uint32_t*)a;
#endif
}
```

// Type punning through union is implementation defined. It's allowed by both gcc and

// clang. It's hard to find an authoritive source, though. Some say it's allowed in

// C99 but not C++, others says C++ allows it as well.

```
uint32_t r3(uint8_t *a) {

​    union {

​        uint8_t c[4];

​        uint32_t d;

​    } u;

​    memcpy(u.c, a, 4);

\#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

​    return ntohl(u.d);

\#else

​    return u.d;

\#endif

}


```

// Memcpy guarentees alignment and does not break strict aliasing. It's used for the C++20

// bit_cast implementation.

```
uint32_t r4(uint8_t *a) {

​    uint32_t val;

​    memcpy(&val, a, 4);

\#if __BYTE_ORDER__ ==  __ORDER_LITTLE_ENDIAN__

​    return bswap_32(val);

\#else

​    return val;

\#endif

}
```

// A last variation.

// nothl is really just bswap_32 and ifdefs for byte order, under the hood.

```
uint32_t r5(uint8_t *a) {

​    uint32_t val;

​    memcpy(&val, a, 4);

​    return ntohl(val);

}
```

