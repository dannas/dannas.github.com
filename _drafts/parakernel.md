---
layout: post
title: Parakernel
---

## MICA
What is MICA?
	in-memory key-value store
How is it different from other kv?
	per-core req handling
	sockets have high overhead for small payloads
		But batching increases latency
	kernel bypass for network stack

What is a kv-store?
	cache semantics (LRU)
	store semantics (explicit del needed for removal)
	ops
		put(key, val)
		get(key)
	in-front of db
		communicate via sockets

What is the question it tries to answer?
	How much can we improve perf for single-node kv-store for small items (fit in packet), independent of distribution and with consistent low latency?
	It's a horse-race paper: Who's faster and why?
What are their claims?
	4-14x faster than state-of-the-art.
	Consistent perf over mixed read- and write-payloads
How do they prove their claims?
	Does it perform well under diverse workloads?
	Does it provide good latency?
	How does it scale with more cores and NIC ports?
	How does each component affect performance?
What are the parameters and variables?
What is good latency?
	How measure it?
How express skewness in a distribution?

What are the "other horses" in the race?
	* Memcached
	* MemC3
	* RAMCloud
	* Masstree

What is novel about their ideas? Refinement or break-throughs?

How can I verify their claims?

How is it connected to Enbergs Parakernel (one queue per proc, move socket to
user space, remove page cache)

### Data Access
Concurrent access is used by many, but involves contention
Exclusive access (only one core can access data) can be faster
	But skewed data distributions slows down the program.
How does MICA do?
	Exclusive access and somehow gives burst I/O more runtime (TODO)
	Falls back to concurrent reads for extremely skewed loads
