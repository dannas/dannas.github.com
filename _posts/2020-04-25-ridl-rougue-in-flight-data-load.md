---
title: Paper, RIDL Rogue In-Flight Data Load
layout: post
---
[Schaik, Stephan van, et al. “RIDL: Rogue In-Flight Data Load.” 2019 IEEE Symposium on Security and Privacy (SP), 2019, pp. 88–105](https://mdsattacks.com/files/ridl.pdf)

I've intended to read up on the speculative execution attacks that have been
published last year but I always postponed it. But when Vaishali Thakkar
started a virtual [papers reading group](https://app.slack.com/client/T0114V1EM4Y/),
I finally found enough motivation.

RIDL is a recent exploit and is part of a set of microarchitectural data
sampling attacks, more information can be found at the [MDS Attacks web page](https://mdsattacks.com). I had to learn many new things about microprocessor internals to understand the exploit. Hopefully, it will be easier to read the Spectre, Meltdown, and Foreshadow papers now. 

I was surprised by how ineffective the exploits are: the paper describes how it took 24 hours to retrieve 26 bytes from a `/etc/shadow` file. On the other hand, as Stephen Röttger shows in [Escaping the Chrome Sandbox with RIDL](https://googleprojectzero.blogspot.com/2020/02/escaping-chrome-sandbox-with-ridl.html), it is possible to attack real systems!

# The Attack
The victim makes a load or store which causes data to be put in the Line Fill
Buffer (LFB). The attacker makes a speculative load and then a dependent load
into a flush-reload buffer. By timing accesses to the flush-reload buffer, the
attacker can infer which data was written or read by the victim.

Here is an example of an attacker code sequence.
```
/* Flush flush & reload buffer entries. */
for (k = 0; k < 256; ++k)
	flush(buffer + k * 1024);

/* Speculatively load the secret. */
char value = *(new_page);
/* Calculate the corresponding entry. */
char *entry_ptr = buffer + (1024 * value);
/*Load that entry into the cache. */
*(entry_ptr);

/* Time the reload of each buffer entry to see which entry is now cached. */
for (k = 0; k < 256; ++k) {
	t0 = cycles();
	*(buffer + 1024 * k);
	dt = cycles - t0;
	if (dt < 100)
		++ results[k];
}
```

What differentiates this attack from the earlier Spectre, Meltdown and
Foreshadow exploits is that the victim thread can leak data even if it is just
running straight-line code, no need for branches or page faults.

# The Line Fill Buffer
A modern Out-Of-Order CPU has lots of buffers and units. I had never heard of a Line Fill Buffer but here is my current understanding of how it works: When data is to be stored it follows this schema[^travdowns]:
1. A store instruction gets scheduled and has a store buffer entry allocated.
2. The instruction executes.
3. When all older instructions have retired, the store instruction retires
4. The store waits until it is at the head of the store buffer (it is the oldest not committed store) at which point it will commit (become globally visible) into the L1 if the associated cache line is present and in MESIF Modified or Exclusive state. 5. If the line is not present in the required state, a Line Fill Buffer (LFB) is allocated.
6. In the miss scenario, the LFB eventually comes back with the full content of
the line, which is committed to the L1, and the pending store can commit.

# How to determine that the Line Fill Buffer leaks?
The authors wrote a kernel module to mark memory in the victim thread as
write-back (WB), write-through (WT)[^petercordes], write-combine (WC) and uncacheable (UC).
For the first experiment, they compared the number of successful retrievals (hits)
of the victim data to the `lfb_hit` performance counter and saw a strong
correspondence.

For the second experiment, a victim thread initially writes a known value to a
fixed memory address and then reads it back in a loop, where each read is
followed by a `mfence`. The number of hits in the attacker was recorded. The
experiment was done for all four memory types, WB, WT, WC, UC. The experiment
was also done for all memory types with the cache flushed using the `clflush`
instruction.

```
*mem_address = value

while (true) {
	*(volatile char*)mem_address;
	_mm_mfence()	
	if (flush_experiment)
		_mm_clflush(mem_address);
}
```
The memory types that stores data in the cache (WB, WT) did not cause a hit in
the attacker, but when the memory was flushed they did hit. This suggests that
the cache was not the cause of the leak. And store-to-load forwarding can not
be the cause since we're leaking from a load.

In the third experiment, the victim thread writes four different values to
four sequential cache lines, followed by a `mfence`. 

```
while (true) {
	*(volatile char*)buf = val1;
	*(volatile char*)buf+64 = val2;
	*(volatile char*)buf+2*64 = val3;
	*(volatile char*)buf+3*64 = val4;
	_mm_mfence();
	if (flush_experiment)
		_mm_clflush(buf);
		_mm_clflush(buf+64);
		_mm_clflush(buf+2*64);
		_mm_clflush(buf+3*64);
}
```
The authors saw leaks for WT, WC and UC memory. They also saw leaks for all four
memory types when flushing was enabled. Since both flushing and the WT, WC and
UC memory types enforce direct invalidation of writes, they must go through the
LFB.

# What Exploits are possible in the wild?
The paper has an example of an exploit where they leak the content of
`/etc/shadow`. They repeatedly invoke the passwd program. They filter the leaked
data on printable ASCII-characters and align on the string "root". After 24
hours of running the passwd program in a loop, they managed to retrieve 26
characters. Uhm, that doesn't sound so impressive, to me.

They also made a cross-VM version where a victim VM runs passwd and the attacker
runs in another VM. They were then able to retrieve 16 characters in 24 hours.

Kernel context can also be attacked. They wrote a victim program that read 0
bytes from `/proc/self/maps`, so no bytes were copied to userspace. But the
attacker could still retrieve the first 64 bytes of the victim process memory
mapping.

Of course, they also targeted SGX (I sometimes think that SGX was created so that
security researchers had something to write papers about, on the other hand
protecting a system from itself is hard).

Finally, the sandbox environment running Javascript can be targetted. There is
no `clflush` instruction available so they had to rely on the much noiser
Evict+Reload technique instead. High-resolution timers are disabled, but there
are ways around that (by using SharedArrayBuffer or GPU-based counters). They
used an old version of Firefox with the high-resolution timers still enabled.
That sounds like cheating to me. Nevertheless, they were able to leak data from
a victim process running on the same system.

# How To Mitigate RIDL?
The authors recommend that SMT be disabled. The Intel mitigation involves updated microcode which allows the software to flush several types of in-flight buffers (LFBs, load ports, and store buffers) using the `verw` instruction. The updated microcode also flushes these buffers when flushing the L1 cache and when leaving/exiting SGX.

---
[^petercordes]: I thought that write-back and write-through were properties of the hardware caches and not something that could be set on memory pages. I asked [When to use write-through cache policy for pages](https://stackoverflow.com/questions/61129142/when-use-write-through-cache-policy-for-pages) and Peter Cordes gave a precise exhaustive answer. It looks like there are very few real-world uses for write-through caches and the only real advantage to them is that they make the hardware designer's job easier.
[^travdowns]: I was puzzled about how the store buffer and LFB interact and asked [How do the store buffer and line fill buffer interact on Stackoverflow](https://stackoverflow.com/questions/61129773/how-do-the-store-buffer-and-line-fill-buffer-interact-with-each-other). Travis Downs gave a detailed explanation (which I've only summarized). In particular, the exact point when the different steps (issue, retire, commit) happens has lots of details.
