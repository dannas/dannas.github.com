---
layout: post
title: Standard C loop idioms
---

<!-- excerpt start -->
What's your standard idioms for doing loops in C? A summary of a Twitter
thread betweeen me, Per Vognsen and Shachaf. 
<!-- excerpt end -->
<div id="inline_toc" markdown="1">

* TOC
{:toc}

</div>

## Looping over Arrays
One of the most common idioms in a programming language is the form of a loop.
The idiomatic form is like this:

```
for (int i = 0; i < n; i++)  // or i += stride
    a[i] = 1.0;
```
And for pointers:
```
for (T *p = start; p != end; p++)  // or p += stride
    *p = 1.0
```

This is not an arbitrary choice. It visits each member of an n-element array
indexed from `0` to `n-1`. It places all loop control in the for itself, runs
in increasing order, and uses the very idiomatic ++ operator to update the loop
variable. It leaves the index variable at a known value just beyond the last
element.

But what about backwards iteration? How do you do that? Some use the `-->` "operator":
```
for (int i = n; i-->0;)
    a[i] = 1.0;
```

Others prefers to start iterating at `n-1` (it was my preferred way in the past):
```
for (int i = n-1; i >= 0; i--)
    a[i] = 1.0;
```

[In a twitter thread, Per Vognsen
said](https://twitter.com/pervognsen/status/1252099900193435650) that he
prefers to start at n due to it being analoguous to the pointer case[^indices-between]:  
```
for (int i = n; i > 0; i--)
    a[i-1] = 1.0;

for (T *p = end; i != start; p++)
    p[-1] = 1.0;
```
His motivation:
>The `n-1` and `i >= 0` form doesn't translate to the pointer case without
>invoking undefined behavior (you can't point past the start of an object in
>C). [...] BTW, this is all related to the choice of using 0-based
>indices/addresses and lower-inclusive, upper-exclusive ranges. With 1-based
>indexing and lower-inclusive, upper-exclusive you have mirror symmetry.[^EWD831]

## Using Unsigned Index Variables
In C the `sizeof` operator returns `size_t` and malloc takes a `size_t`
parameter. In C++ the containers `size()` method[^size-rant] returns `size_t`
. Even more the `std::string` uses `size_t` in many parameters and it uses
`(size_t)-1` as a sentinel for value not found. So C and C++ programmers need
to deal with using unsigned index variables.

I like the idea of using `size_t` for showing intent: this variable represents
an index into memory. But it's easy to end up with unintended underflow in the
for condition expression. Using `size_t` works fine for looping forward over a
collection `i = [0, n)`.

```
for (size_t i = 0; i < n; i++) 
    a[i] = 1.0;
```
But statements where the condition expression might be negative causes endless
loops, such as this when `offset` is larger than `n`:
```
for (size_t i = 0; i < n-offset; i++) 
    a[i] = 1.0;
```

So you have to guard each such for-statement inside an if-statement. Clumsy! I
haven't found a way around that. 
```
size_t len = n - offset;
if (len < n) {
    for (size_t i = 0; i < len; i++) {
        a[i] = 1.0;
    }
}
```
## Iterators
I've, [like twitter user Shachaf](https://twitter.com/shachaf/status/1256772996154286082) done C iterators in this style:
```
Iterator it = foo_start();
while (foo_next(&it)) {
    ...it.value...
}
```
[Per pointed out](https://twitter.com/pervognsen/status/1256775864366194690) that this is a conflation of iterator state vs iterator value.
He places the return value of the next call inside the struct and ends up
with an API similar to this:

```
for (Iterator it = iterator_init(); iterator_next(&val);) 
    use(val);
```

Per has written a [line, word and token iterator using his Ion language in a
gist](https://gist.github.com/pervognsen/fed0b1328b10b63f51ad42f961fc4058). A
C implementation of a line iterator in his style would be: 
 ```
typedef struct StrLinesIter {
    const char *next;
    const char *start;
    const char *end;
} StrLinesIter;

StrLinesIter str_lines(const char *str) {
    return (StrLinesIter){ .next = str };
}

bool str_lines_next(StrLinesIter *iter) {
    if (!*iter->next) {
        return false;
    }
    iter->start = iter->next;
    iter->end = iter->next;
    while (*iter->end && *iter->end != '\n') {
        iter->end++;
    }
    iter->next = *iter->end ? iter->end + 1 : iter->end;
    return true;
}
```
That iterator can be called like this:
```
for (StrLinesIter iter = str_lines(buf); str_lines_next(&iter);) {
        printf("...'%.*s'\n", (int)(iter.end - iter.start), iter.start);
    }
```

---
[^EWD831]: Numbering can start at 0 or 1. Edgar Djikstras [Why Numbering Should Start at Zero](https://www.cs.utexas.edu/users/EWD/transcriptions/EWD08xx/EWD831.html) prefers [a,b) and (a,b] when choosing between [a, b), (a, b], [a, b] or (a, b) for denoting ranges. The [a, b) and (a, b] cases has the advantage of the difference b-a being equal to the size of the range. Djikstras argument for choosing [a, b) over (a, b] is that the range 0 to N otherwise would have to be specified as (-1, N]. A mathematician don't want to use unnatural numbers unless he has to. Lastly, he prefers the range to start at 0, since it gives the better-looking range [0, N) instead of [1, N+1). But as Per points out, 1-based indexing shares the same properties as 0-based.
[^size-rant]: The STL `size()` functions have ambiguous naming: many people use `size` for representing number of bytes and `len` for number of elements.
[^indices-between]: Another way of thinking about it is if we let [Indices Point Between Elements](https://blog.nelhage.com/2015/08/indices-point-between-elements/). Then the end index does not point one past the last element, but it points directly at the end. So when traversing an array backward, we start at the end and scan it until we reach the start.
