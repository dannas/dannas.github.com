---
layout: post
title: Repair Microsoft Sculpt mouse buttons
---

<!-- excerpt start -->
The Microsoft Sculpt mouse that comes with the Sculpt keyboard has been a trusty companion for me. But the left button wears out. Here's how to fix it.
<!-- excerpt end -->

The mouse would only respond to left mouse clicks a quarter of the times. Not acceptable. Buying a new is one way of solving it. Unfortunately it's only sold bundled with the Microsoft Sculpt keyboard (which still works flawlessly for me). And the usb-tranceiver is hardcoded to your specific keyboard and mouse. So can we fix the mouse?

Removing the plastic cover is a little fiddly, check some you-tube videos if you're worried you might  break it. Once you've removed the outer cover and unscrewed an inner plastic details you should see the tactile switch. It's a Panasonic (Matsushita) EVQ-P0N02B. I ordered [ten parts from Digi-key](https://www.digikey.se/en/products/detail/EVQ-P0N02B/P12965SCT-ND/762955).  Unsolder the old one (make sure you don't melt the outer plastic that is located dangerously close to switch). Solder the new one and "clickety-click" your mouse is as good as new.

**Side note**: I happen to have two sets of Sculpt keyboard+mouse, one for work and one at home. For the one at work, my Ubuntu 22.10 computer hangs during upstart until I remove the transceiver and then re-insert it. This is not a problem on my mouse at home. At some point I should investigate it.

**Side note 2:**: The Microsoft Sculpt mouse has a blue button with a Windows logo on it. That button is not mapped to anything by default on Ubuntu. At some point I should map it, but so far I haven't had the need.