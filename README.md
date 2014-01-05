"Cruce" Game
=========

This is an open source effort to create a PC Game based on the Romanian card
game "Cruce".

The Project's main goal is teaching students how to develop software from start
to finish.

"How to build" Game
=========

Tools required:
- You must have GNU C Compiler.
- You must have autotools on your machine (Includes automake and autoconf
  packages).

Instructions to build:
- You need to run ./autogen.sh file (It will generate configure script).
  ```$ ./autogen.sh```
- On successful run of ./autogen.sh, you will have configure file. Run it.
  ```$ ./configure```
- Next step, is to run make. ```$ make```
- Now install the binaries, using make install. ```$ make install```
