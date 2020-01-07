---
layout: post
title: Zero Configuration Networking
---

## Overview
* Three layers
* Why?
* Who?
* When?
* How many users?
[Pentest mdns]: https://blog.hyperiongray.com/multicast-dns-service-discovery/
[Steinberg book]: https://www.amazon.com/Zero-Configuration-Networking-Definitive-Guide/dp/0596101007

## Implementation on Linux

### Avahi

[avahi]: https://wiki.archlinux.org/index.php/Avahi

systemd-resolved

[reddit avahi]: https://www.reddit.com/r/archlinux/comments/djg602/are_avahi_and_systemdresolved_really_incompatible/

## Setup the lab environment

TUN/TAP like https://www.saminiir.com/lets-code-tcp-ip-stack-1-ethernet-arp/

[Q&A zeroconf]: https://unix.stackexchange.com/questions/551088/linux-virtual-network-for-experimenting-with-zeroconf-networking
[Q&A dummy eth]: https://unix.stackexchange.com/questions/152331/how-can-i-create-a-virtual-ethernet-interface-on-a-machine-without-a-physical-ad

https://developers.redhat.com/blog/2018/10/22/introduction-to-linux-interfaces-for-virtual-networking/

## Link local addressing

* Choose random IP from 169.254/16
* Probe 3 times with ARP requests
* Claim with gratious ARP
* Upon conclicts, both parties restart the probing phase

## Multicast DNS

* Choose random name with .local TLD

* Probe 3 times with DNS requests using multicast DNS to 224.0.0.151
* Claim with gratious DNS reply
* Upon conflicts, both parties restart the probing phase

## DNS-SD

