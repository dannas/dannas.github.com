---
layout: post
title: Timing Wheels
---

## The problem

* How organize pending timer jobs effectively?

## The Paper



## Some resources

* Adrian Colyer describes the original 1987 paper in his review post [Hashed and Hierarchical Timing Wheels: Data Structures for the Efficient Implementation of a Timer Facility](https://blog.acolyer.org/2015/11/23/hashed-and-hierarchical-timing-wheels/)

* Paul Cavallaro also wrote a review of the 1987 paper in [Hashed and Hierarchical Timing Wheels](https://paulcavallaro.com/blog/hashed-and-hierarchical-timing-wheels/)

* LWN.net originally described the timer wheel in 2005 in [A new Approach to Kernel Timers](https://lwn.net/Articles/152436/)

* LWN.net then in 2015 wrote a new article about some changes done [Reinventing the timing wheels](https://lwn.net/Articles/646950/)

* Julian Squires made a [20min presentation for the conference Systems We Love in 2017](https://systemswe.love/archive/minneapolis-2017/julian-squires)

* Facebooks Folly library has an implementation of a timer wheel in [io/async/HWWheelTimer.cpp](https://github.com/facebook/folly/blob/master/folly/io/async/HHWheelTimer.cpp)

