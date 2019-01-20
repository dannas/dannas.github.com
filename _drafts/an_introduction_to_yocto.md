---
layout: post
title: An Introduction to Yocto
---

## Concepts

* bitbake - a build tool like make
* layers
* recipes
* poky,  a skeleton for Board Support Package (BSP) and kernel
* build step
* search paths
* syntax of bb-files

## How start

* Do [Harald Achitz bitbake tutorial](https://a4z.bitbucket.io/docs/BitBake/guide.html)

## What is yocto?

* Yocto is a tool for building Linux distributions from scratch. 
* Yocto is a set of scripts and configuration files written in a mix of Shellscript and Python. 
* The software to be installed into the Linux distribution are are referred to as packages.
* For each package, Yocto specifies how to download, patch, build, install and deploy that package.
* The scripts and configuration files are organized in layers.
* Layers can override and extend lower layers.

## Some Variables

* PN
* PROVIDES

