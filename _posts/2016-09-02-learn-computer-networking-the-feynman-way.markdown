---
layout: post
title: Learn computer networking the Feynman way
---

A friend of mine has just started a computer networks course. He's somewhat
bewildered by all the new concepts and the sheer amount of information that
he's expected to learn: the OSI model, pipelining, sliding windows, keepalive.

I've read TCP/IP Illustrated and Tanenbaums Computer Networks. I feel as if
they're forcing a solution down my throat before I've had a chance to think
about the problem properly. Can I succintely describe the major problems when
trying to make two computers communicate and sketch out the solutions?

So, imagine you're police constaple X and need to contact Batman in his cave.
No Batman light invented yet, no phone, no nothing: You have a conducting wire
between your your office and the cave.  How do you tell him to come over?  Add
a switch on your side and a light on his. 

Now, what if you want to arrange to meet at a different location? One solution
would  be to use N wires for N different locations. So far so good.

But then you realize that you want to tell Batman upfront the reason for
meetinng up with him. Every parent knows how painful it is to hear "daddy can
you please come" and not get to know the reason why you're supposed to drop
everything and rush over. The problem is: N wires can only carry N different
messages in the current scheme. We can increase the number of wires but sooner
or later, keeping track of all wires will becomre cumbersome. Let's encode the
message using the wires. Each wire can be in two state, high or low. Two wires
can be in four different states {00, 01, 10, 11}, three wires can have 8
states {000, 001, 010, 011, 100, 101, 110, 111}. It's the binary number
system. N wires can then carry 2^N messages. This is the same scheme used to
wire a register to the ALU.

The schemes up until now has asssumed that the number of allowed messages in
the system is fixed. What if you want to invent new messages; after all there
may be other criminals than two-face and the Joker. If you decide that a
certain combination of wire signals does not describe the entire message but
just a subpart, then you can combine those subparts into a message. You could
let each combination describe a word, but characters is probably a better
choice. Now you can send the first character, wait a while, then send the next
and so on.

After a while I expect that both you and Mr Batman will become somewhat
frustrated by the delays between each sent characters and the tedious process
of encoding and decoding them. Let's assume that you've figured out a way for
the machine to store the received characters. Now you're no longer bound by
the processing time of a human; the sending frequency can be significantly
increased. But before we do that, we have two problems that needs to be
resolved. Firstly, machines do not understand the instruction "wait a while
between received characters". How long is "a while"? If we decide that it's N
clock cycles, then whose clock is it we're relying on? Clocks will drift over
time, causing hard to diangnose errors. Let's use one of the wires as a
strobe. Whenever that strobe is activated, we're allowed to read one more
character. We now have something rougly similar to the now extint parallelport
a.k.a. printer port. The same sort of communication is also common when
directly interfacing integrated circuits such as memory.

One problem though: wires affect each other. Long wires, bunded together will
cause transmission errors. Another problem: electrons travels fast through wires but
they still take some time. If the wires are not of exactly the same length the
you may face timing issues. For these causes, parallel transmissions has been
abandoned in several areas: usb instead of the printerport, PCI-express
instead of PCI.

We'll have to back up a bit. Instead of N wires let's just concentrate on one.
How can you send different messages through just one wire? At any point in
time the wire can be high or low (ignoring rise times). If we can say that at
each timestep, we're ready to read the variable then we're good to go. Just
change that switch in rapid succession. Unfortunately - once again - it's har
to keep clocks in sync. If they drift, then the data sent won't be interpreted
correctly at the receiver. What to do? One way of solving the problem is by
prepending a start bit and an end bit to each character byte sent. Then the
receiver can synchronize its clock using the transition from end to start bit.
That's how an UART does it. UARTs are obigious in most commputers. They're
used for troubleshooting and simple communication. A typical PC no longer has
a serialport but they're still widely used.


# Things to consider for inclusion

The need for rs232, ethernet, can phys to increase the signal

Use MII as an example of a paralell bus

I want to reach out to my friend. How? Use a button, a wire and a led.

I want to be able to tell him different messages? Use more than one wire.

What if there can be errors?

What if the receiver can be swamped?

What if packets can get lost?

What if there are more than two peers?

How allow everyone to talk on a shared medium?

How make sure that the one with highest priority is allowed to speak first?

What if there are more subnetworks than one?

What if we don't know the end destination?

What if there is a long latency between packets and we want to increase throughput?

What if each node has more than one communicating peers: How identify each?

What if packets can arrive out of order?

How determine where a message start and ends?

How make sure that a session is not reused before the previous communication has ended?

How make sure a connection is still alive?

How make sure the receiver is not swamped?

How make sure the sending window of a session is properly sized?
