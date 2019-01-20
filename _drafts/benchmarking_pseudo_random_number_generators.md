---
layout: post
title: Benchmarking Pseudo Random Number Generators
---

Fabien Giesens tweet from Apr 28, 2018: "Mersienne Twister: small code, small state, high throughput, high-quality random numbers: pick any 0"

## Pitfalls of limiting range for rand()

Julienne at EternallyConfuzzled has an article [Using rand()](http://www.eternallyconfuzzled.com/arts/jsw_art_rand.aspx) that explains how to properly seed rand; the fact that rand does not give any guarantees about distribution; and how to adjust the range of generated random numbers. She also has written [An Introductionto Pseudo-Number Generation](http://www.eternallyconfuzzled.com/tuts/algorithms/jsw_tut_rand.aspx)

Stephan T. Lavavej discusses why rand is bad and why many algorithms for scaling uniform random ranges are bad as well https://channel9.msdn.com/Events/GoingNative/2013/rand-Considered-Harmful

rand % N causes bias if N % RAND_MAX != 0. It's the pigenhole principle. If you generate numbers in say [0, 7] and you want to scale it to [0, 3]  then 0 will be generated one time more than the others.

https://codereview.stackexchange.com/questions/159604/uniform-random-numbers-in-an-integer-interval-in-c says that Linear Congrentual Generators have lower entropy in the lower bits. an answer gives suggestions on how to implement rand).

Paul Hsieh [Misconceptions about Rand](http://www.azillionmonkeys.com/qed/random.html) describes how to fix the bias. He also gives an implementation of a generator for a uniform distribution.

[CppCon 2016: Cheinan Marks “I Just Wanted a Random Integer!"](https://www.youtube.com/watch?v=4_QO1nm7uJs) gives a good presentation about random number generators and uniform distributions.

[CppCon 2016: Walter E. Brown “What C++ Programmers Need to Know about Header ＜random＞"](https://www.youtube.com/watch?v=6DPkyvkMkk8) gives some background on the development of the random-header, but does not describe algorithms.

A youtube presentation ["PCG: A Family of Better Random Number Generators" - Melissa O'Neill of Harvey Mudd College](https://www.youtube.com/watch?v=45Oet5qjlms)

Melissa O'Neill discusses the amount of bias introduced by `rand() % N`. It's for most cases negligable. 

Melissa O'Neill shows pictures of prng output: http://www.pcg-random.org/posts/visualizing-the-heart-of-some-prngs.html

Melissa ONeill on [Efficiently generating a Ranom Number in Range](http://www.pcg-random.org/posts/bounded-rands.html)

Daniel Lemire Fast Random Integer Generation in an Interval https://arxiv.org/pdf/1805.10941.pdf

http://burtleburtle.net/bob/rand/smallprng.html

http://burtleburtle.net/bob/rand/talksmall.html

David Blackmans Scrambled Linear Pseudorandom Number Generators https://arxiv.org/pdf/1805.01407.pdf

Jerry Coffin describes in https://stackoverflow.com/a/2999130/582010 how to

```
int rand_lim(int limit) {
/* return a random number between 0 and limit inclusive.
 */

    int divisor = RAND_MAX/(limit+1);
    int retval;

    do { 
        retval = rand() / divisor;
    } while (retval > limit);

    return retval;
}
```

https://cs.stackexchange.com/questions/2605/is-rejection-sampling-the-only-way-to-get-a-truly-uniform-distribution-of-random

## Different Random Number Generators

http://xoshiro.di.unimi.it/

The random-number generators in C++ standard library are insufficient

https://lemire.me/blog/2017/08/15/on-melissa-oneills-pcg-random-number-generator/. He has also published a benchmark https://github.com/lemire/testingRNG

http://www.pcg-random.org/other-rngs.html discusses Mersienne Twister, Linar Congrentual Generators, Linear Feedback Shift Register.

Knuth Seminumerical algorithms

JONES, David; UNIT, UCL Bioinformatics. Good practice in (pseudo) random number generation for bioinformatics applications. *URL http://www. cs. ucl. ac. uk/staff/d. jones/GoodPracticeRNG. pdf*, 2010. http://www0.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf

PARK, Stephen K.. ; MILLER, Keith W.. . Random number generators: good ones are hard to find. *Communications of the ACM*, 1988, 31.10: 1192-1201.

## Cryptographically secure RNG vs pseduo-random

## Benchmarks

