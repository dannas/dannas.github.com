---
layout: post
title: PowerPC Assembly
---

## Cheatsheet
li Rd, c                    Rd := c
lis Rd, c                   Rd := (c << 16)
addi Rd, Rs, c              Rd := Rs + c
mr Rd, Rs                   Rd := Rs
stw Rs, disp(Rb)            M[disp+Rb] := Rs
lwz Rd, disp(Rs)            Rd := M[disp + Rs]
lwzu Rd, disp(Rs)           Rd := M[disp + Rs]
                            Rs := disp + Rs
rlwinm Rd, Rs, n, mb, me    Rd := (Rd ROL n) AND mask(mb..me):
mflr Rd                     Rd := lr
mfcr Rd                     Rd := CR[0..7]
cmp R1, R2                  CR0 := R1 CMP R2
b target_address            pc := pc + displacement
bl target_address           lr := pc + 4
                            pc: pc + displacement

## History and ABI

AIM alliance
    Apple, IBM, Motorola
1991
    PowerPC ISA v2.02
2005
    Apple abondoned PowerPC for Intel
Power.org
    IBM, Freescale, AMCC.
    Power architecture

https://www.ibm.com/developerworks/library/pa-archguidev1/pa-archguidev1-pdf.pdf

https://en.wikipedia.org/wiki/Power_Architecture#Power_ISA_v.2.06

EREF 2.0: A Programmer’s Reference Manual for Freescale Power Architecture® Processors

Variable-Length Encoding (VLE) Programming Environments Manual: A Supplement to the EREF

Power Architecture® 32-bit Application Binary Interface Supplement 1.0 - Embedded

## MPC57xx
Qorivva
    Power ISA 2.03
        based on v2.02 and Book E extensions
    1-2 PowerPC e22 cores
    automatic self-tess
    ECC
    lock-step (not MPC5748G)

## From Zero to Main
TODO: To be moved to separate blog

e200 ref. 2.6 Reset Settings

## Data Types

## Endianess

## Registers

## Condition Code Registers

## PowerPC and VLE

## Introduction to PowerPC instructions

## Memory Instructions: Load and Store

## Addressing Modes

## Using Immediate Values on PowerPC

## Load/Store Multiple

## Push and Pop

## Conditional execution

## Branches

## Stack

## Functions



