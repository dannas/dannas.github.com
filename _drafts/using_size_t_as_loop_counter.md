---
layout: default
title: Using size_t as Loop Counter
---

# Using size_t as Loop Counter

* Mixing unsigned and signed numbers in C and C++ can lead to bugs.

* In C and C++ the `sizeof` operator returns `size_t` and in C++ the containers `size()` method returns `size_t` . Even more the `std::string` uses `size_t` in many parameters and it uses `(size_t)-1` as a sentinel for value not found.
* I like the idea of using `size_t` for showing intent: this variable represents an index into memory. The `size` part is a bit misleading though since many people use `size` for representing number of bytes and `len` for number of elements.
* It works find for looping forward over a collection `i = [0, n)`. Just write `for (size_t i = 0; i < N; i++) use(a[i])`. 
* But looping forward and keeping track of the successor does not work as expected. This code `for (size_t i = 0; i < N-1; i++) use(a[i], a[i+1])` breaks down if the collection is empty.
* Looping backwards is also problematic. This code `for (size_t i = N-1; i >= 0; i--) use(a[i])` will have `i` wrap around leading to an endless loop. Some suggestions: 

```
for (size_t i = N-1; i < N; i--) {  //  1
    use(a[i]);
}
for (size_t i = N; i > 0; i--) {     //  2
    size_t pos = i-1;
    use(a[pos]);
}
for (size_t i = N; i-->0; ) {    //  3
    use(a[i]).
} 
```

I have never seen Alternative 1 in any code base that I've worked with. Most probably wouldn't recognize it as valid code. Alternative 2 is more common but requires one extra loop variable.  Alternative 3 works but breaks down if we want to loop from `N` to `1`. We can start the loop from `N+1` but that's still awkward.

```
for (size_t i = N+1; i-->0; ) {
    size_t len = i;
    use(len);
}
```



## Other Problems with mixing size_t and int

Comparing `size_t` to a `int` leads to unexpected results.

```
int max_len = INT_MIN;
if (str.size() > max_len) {
    max_len = str.size();
}
```

