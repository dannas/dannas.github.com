---
layout: post
title: Embedded Online Conference
---

I've attended the [Embedded Online
Conference](https://www.embeddedonlineconference.com/index.php#tracks). Here
are my notes from five talks I watched from the Embedded Systems Security track.

<div id="inline_toc" markdown="1">

* TOC
{:toc}

</div>


## Common Cryptography mistakes for Software Engineers
[Aljoshcha Lautenbach](https://se.linkedin.com/in/aljoscha-lautenbach-311b9913)

Aljoschca described six common cryptographic mistakes. Many of the problems
boil down usability problems with crypto libraries. Here's a paper that he
recommends:

[Acar, Yasemin, et al. “Comparing the Usability of Cryptographic APIs.” 
2017 IEEE Symposium on Security and Privacy (SP), 2017, pp. 154–171.][1]

[1]:(https://www.cl.cam.ac.uk/~rja14/shb17/fahl.pdf)

The mistakes he listed are:

* **Confusing checksums and cryptographic hash function**. A checksum can be
  reversed and are not necessarily collision-resistant.

* **Confusing confidentiality with integrity**: Just because something is
  encrypted, doesn't mean someone can't change it.

* **Wrong cipher modes**. There are a bunch of cipher modes like Electronic Code
  Book mode (ECB), Cipher Block Chaining mode (CBC), Counter mode (CTR). Here
  ECB leaks information about structure and  patterns and CBC is vulnerable to
  padding oracle attacks (POODLE, Lucky13).

* **Outdated crypto algorithms**. Many algorithms that still have widespread
  support are insecure or deprecated. For example MD5 and SHA-1 should be
  avoided as cryptographic hashes; DES, 3DES, and Blowfish should not be used
  as block ciphers; RC4, A5/1 and A5/2 should not be used as stream ciphers;
  and RSA PKCS#1 v1.5 and CBC should not be used as padding.

* **Lack of entropy**. Aljoshcha used the [Debian SSL bug][2] as an example of
  how a small change of the code can drastically reduce the amount of entropy.
  Lots of things can reduce entropy and it's an area where it's easy to make
  mistakes.

  [2]:https://research.swtch.com/openssl

* **Reuses nonces or IV**. To avoid replay attacks a freshness value is
  included in a message. It may be a counter, timestamp, or nonces. If you use
  Counter mode (CTR) and reuse your nonces, then secrets will leak from your 
  ciphertext.

My main take away from this presentation is that I need to use recent
algorithms and that seeding entropy and reusing nonces or IV has a big
impact. It also left me longing to know more, and for that Aljoshcha
recommended these resources

* [cryptopals.com](https://cryptopals.com/)
* [opensecuritytraining.info](http://opensecuritytraining.info/)
* [offensive-security.com](https://www.offensive-security.com/community-projects/)
* [hackthebox.eu](https://www.hackthebox.eu/individuals)
* [vulnhub.com](https://www.vulnhub.com/about/)

I wished that the presentation had listed the strengths and weaknesses of some
common libraries. Can I use WolfSSL? Should I use libsodium?

## Linux Kernel Security - Inside Linux Security Modules
[Vandana Salve](https://www.linkedin.com/in/vandana-salve-31b0586)

The Linux Kernel has a framework in place for handling Mandatory Access
Control (MAC). In contrast to Discrete Access Control (DAC) which can be
overridden, MAC is enforced on a system level.

The LSM framework adds hooks in common places for authorization queries. Many
kernel objects have void pointer members used by LSM like task_struct,
superblock, inode, file, sk_buff.

The hooks return an int, where 0 represents success, and failure can be
represented by ENOMEM, EACCESS, EPERM. As soon as a hook returns a non-zero
value, the operation is aborted.

The majority of the code lives in security/security.c 

The most used LSMs are SELinux and AppArmor. SMACK is used by many embedded
Linux implementations.

I asked Vandana about any papers/studies that compare the effectiveness
of Mandatory Access Control to alternatives such as sandboxing and DAC. She
recommended this paper:

[Loscocco, Peter A., et al. The Inevitability of Failure: The Flawed 
Assumption of Security in Modern Computing Environments. 2000.][3]

[3]:https://www.nsa.gov/Portals/70/images/resources/everyone/digital-media-center/publications/research-papers/the-inevitability-of-failure-paper.pdf

Here are some examples of LSM hooks on common Linux Kernel objects:

* **super_block**: Represents a file system. LSM hooks for `sb_mount`, 
  `sb_umount`, `sb_remount` and `sb_statfs`.
* **file**: Represents an open file. LSM hooks for `file_permission`, 
  `file_locks`, `file_ioctl`.
* **inode**: Represents kernel file objects such as file, directories or
  symlinks. Hooks exists for most actions taken on the inode such as: `create`,
  `mkdir`, `link`, `readlink`, `getattr`, `permissions`.
* **task_struct**: Represents a kernel schedulable task. Hooks for `task_alloc`,
  `task_kill`, `task_fix_setuid`.
* **IPC**: Hooks for `ipc_permission`, `msg_queue_msgrcv`, `shm_shmat`,
  `sem_semctl`.
* **Networking**: Socket-related hooks and more fine-grained hocks for IPv4,
  Unix domain sockets, netlink and other protocols
* **Module and system hooks**: Module loading. System time, accessing kernel
  message ring. Hooks for Audit framework. Hooks for eBPF.

The existing Linux Security Modules as of 5.6

* **SELinux**: The default MAC implementation on RedHat and Android. Consists
  of a LSM and a set of trusted services for administration. 
* **SMACK**: Attribute-based like SELinux. Simpler to administrate. The default
  in Automotive Grade Linux (AGL) and Tizen.
* **AppArmor**: The default on Debian. Path-based. Rules can be created for any
  filesystem and for files that may not exist yet.
* **TOMOYO**: Path-based.
* **LoadPin**: Merged in Linux 4.7. A minor LSM. Ensures all kernel-loaded
  files originate from the same filesystem. Intended to simplify embedded
  systems that don't need any of the kernel module signing infrastructure.
* **Yama**: Intended to collect system-wide DAC security restrictions that are
  not handled by the core kernel. Currently used for reducing the
  scope of ptrace.
* **SafeSetID**: Merged in Linux 5.1. Restricts UID/GID transistions.
* **LockDown**: Merged in Linux 5.4. Disables unsigned module loading; access
  to /dev/{mem,kmem,port}; kexec of unsigned images; raw I/O access and more.

Vandanas presentation was a thorough introduction to the internals of the LSM
framework. I just wished that I had a bit more background about how to use
SELinux/SMACK/AppArmor before this session: many of the implementation details
were lost on me. I  asked her about advice on how to choose between the four
major LSMs and she pointed me to the Tomoyo wiki [Secure OS Comparison At a
Glance][4].

[4]: http://tomoyo.osdn.jp/wiki-e/?WhatIs#comparison

## Hardware Hacking Hands-On
[Colin O'Flynn](https://twitter.com/colinoflynn)

The hardware vendors have improved their security offerings through techniques
such as TrustZone and Platform Security Architecture (PSA), but still there
are a lot of exploits happening. So Colin asks: What went wrong?

* Were solutions not deployed?
* Were solutions misapplied?
* Do the solutions work?

It's the third, question in particular that he is interested in. With tools
that he has developed he can investigate what's going on inside the chip. PSA
states that "advanced hardware invasive attacks" where the attacker can infer
fuse settings or perform differential power analysis is out of scope. But the
tools that Colin has developed are both cheap and easy to use! Whoa, that was
news to me!

If you try to prevent access to your firmware by not adding pins headers for
the JTAG signals, you're very much out of luck. It's very easy to figure out
where those signals are located and solder on a pin header yourself.

Even if the MCU is hidden underneath a physical shield and the JTAG pins are
not connected, the chip can still be desoldered with standard equipment.

If you have a shared key stored on the device, that key can be retrieved through
Differential Power Analysis (DPA). By measuring power consumed, we can infer
what data was present on a data bus. It involves sending in a known message,
encrypting it and measuring the power consumed. It can be done very fast through
devices such as the ChipWhisperer that Colin has developed. You can work around
these types of exploits by using more expensive hardware or, the preferred way:
By not placing shared keys on your devices and by using asymetric crypto for 
validation.

For part two Colin showed how to circumvent read protection by injecting
faults through VCC glitches. He can get the hardware to jump over branches
and access memory that shouldn't be accessible.

The whole presentation was an eye-opener. I so much wanna get one of his tools
and start experimenting myself.

More information about the ChipWhisperer can be found on the [NewAE wiki about
ChipWhisperer](https://wiki.newae.com/Main_Page)

## Securing the IoT from Chip to Cloud
[Jacob Beningo](https://twitter.com/Jacob_Beningo)

Jacob described how ARMs Platform Security Architecture (PSA) is organized.
There are three parts

* Threat Model
* Security Architecture
* Implementation

A lot of the presentation felt a little corporate cheezy, but the gist of it
is that the methodology allows us to identify which assets to protect and
use chip-level isolation inside the MCU for shielding those assets.

Jacob uses a IoT door lock as an example and creates a threat model a table
where he maps security objectives against the 
[STRIDE](https://en.wikipedia.org/wiki/STRIDE_(security)) threats. Then he
partitions the system architecture into a Secure Processing Environment (SPE)
and a Non-secure Processing Environment (NSPE). The actual isolation is
provided by the Memory Protection Unit (MPU) and TrustZone.

The PSA organization has published example code at
[TrustedFirmware.org](https://www.trustedfirmware.org). It looks like a great
starting point for designing my own secure system.

Jabcobs presentation was based on Cypress whitepaper [Ogawa J., Mann. B.
Thread-based Analysis Method for IoT Devices.
2018](https://www.cypress.com/file/447056/download)

The presentation gave me a good birds-view of how PSA is structured. A lot of
security-sensitive code needs to be written to implement a PSA-system and I
agree with Jacob that you really need to rely on third-party code to get done
within reasonable time limits.

## Defending Against Hackers: Exploit Mitigations and Attacks on ARM Cortex-A Devices
[Maria Markstedter](https://twitter.com/@fox0x01,%20@azeria_labs)

Maria starts off by presenting a [tweet thread from Andrew
Tierney](https://threadreaderapp.com/thread/931826171003723776.html). Many
IoT devices still don't employ exploit mitigations against buffer overflows.
That in combination with running as root, provides many opportunities for
exploits.

Many projects uses unsafe functions such as `strcpy`/`sprintf`/`gets`. Without
mitigations, those are easily exploited. The canonical example of a buffer
overflow:

```
void func1(char *s) {
	char buffer[12];
	strcpy(buffer, s);
}
```

If an attacker can control the `s` string, he may overwrite `buffer` and thereby
take control of the return address. If he can point the return address to another
buffer that contains shell code, he can get root access. Pwned!

Having a Execute-Never (NX) stack prevents this exploit from happening. But
then there is Return-Oriented Programming (ROP). The attacker can't execute
code from XN-marked pages (e.g. stack) but they can chain addresses of
instructions (gadgets) and can invoke library functions or API calls.

To make ROP chains harder, Address Space Layout Randomization (ASLR) is used.
Now, previously chosen ROP gadgets won't work anymore, since the base address
of the library changes. That's quite an obstacle. But bypass techniques do
exist.

One option is brute-forcing ASLR. On 32-bit platforms, the entropy is quite low
since there are so few available addresses. Another option is to find a second
bug for leaking exploit primitives.

Stack canaries is a third way of preventing buffer overflows. A secret value
is written at the top of the stack and is compared before returning from a 
function. If the known value (the canary) has changed, the program aborts. To
bypassing the stack canary, an attacker needs a second exploit to figure out
what the stack canary is.

As an example of how a hacker creates an exploit, Maria recommends Andy Nguyens
write-up about a fully chained exploit for the PS Vita consisting of six
vulnerabilities: [Trinity: PSP Emulator
Escape](https://theofficialflow.github.io/2019/06/18/trinity.html).

I knew about NX, ASLR and stack canaries and ROP and NOP sleds, but it was very
illuminating for me to get a sense of how much effort is required on the hackers
part to bypass the mitigations. A great presentation and I wished that I could
attend more of Marias sessions.