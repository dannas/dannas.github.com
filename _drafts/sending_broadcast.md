---
layout: post
title: Sending Broadcast Packets
---

## History
* Used by arp?
* Used by dhcp
* Would create DoS if allowed past routers

# Sending to 255.255.255.255 on single interface
* sockopt for allowing broadcast
https://francisfish.com/error-permission-denied-sending-udp-broadcast-packets/

## Sending from multi-homed host on Windows

According to [MSDN:s sendto documentation][msdn] a INADDR_BROADCAST packet will
be sent to all interfaces.

>To send a broadcast (on a SOCK_DGRAM only), the address pointed to by the to
>parameter can be constructed to contain the special IPv4 address
>INADDR_BROADCAST (defined in Winsock2.h), together with the intended port
>number. If the address pointed to by the to parameter contains the
>INADDR_BROADCAST address and intended port, then the broadcast will be sent
>out on all interfaces to that port. 


## Sending from multi-homed host on Linux

Stevens 20.2 Broadcast addresses

>Limited broadcast address: {1, 1, 1} or
>255.255.255.255 dagrams destined to this address must never be forwarded
>by a router.
>255.255.255.255 is to be used as the destination address during the bootstrap
>process by applications such as BOOTP and DHCP, which do not yet know the
>node's IP address.  The question is: What does a host do when an application
>sends a UDP datagram to 255.255.255.255? Most hosts allow this (assuming the
>process has set the SO_BROADCAST socket option) and convert the destination
>address to the subnet-directed broadcast address of the outgoing interface. It
>is often necessary to access the datalink directly (Chapter 29) to send a
>packet to 255.255.255.255.  

>Another question is: What does a multihomed host do
>when the application sends a UDP datagram to 255.255.255.255? Some systems send
>a single broadcast on the primary interface (the first interface that was
>configured) with the destination IP address set to the subnet-directed
>broadcast address of that interface (p. 736 of TCPv2). Other systems send one
>copy of the datagram out from each broadcast-capable interface. Section 3.3.6
>of RFC 1122 [Braden 1989] "takes no stand" on this issue. For portability,
>however, if an application needs to send a broadcast out from all
>broadcast-capable interfaces, it should obtain the interface configuration
>(Section 17.6) and do one sendto for each broadcast-capable interface with the
>destination set to that interface's broadcast address.

## Ways to resolve the Linux problem

* Select one interface to send on setsock
* Iterate over network interfaces and send to the broadcast address for that subnet

https://stackoverflow.com/questions/683624/udp-broadcast-on-all-interfaces

## Receiving broadcast messages when listening on a specific port
See this questions https://stackoverflow.com/questions/35130743/how-to-create-a-service-that-sends-receives-udp-broadcasts-on-multiple-interface

[msdn]:https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-sendto?redirectedfrom=MSDN
[rfc919]:https://tools.ietf.org/html/rfc919

https://blog.packagecloud.io/eng/2017/02/06/monitoring-tuning-linux-networking-stack-sending-data/#udp_sendmsg

The Linx kernel function used for detecting 255.255.255.255 is `ipv4_is_lbcast`.
