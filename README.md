"Cruce" Game
=========

This is an open source effort to create a free PC Game based on the Romanian
card game "Cruce".

The Project's main goal is teaching students how to develop software from start
to finish.

"How to build" Game
=========

Tools required
------
- You must have GNU C Compiler ```gcc```.
- You must have autotools on your machine (```automake```, ```autoconf``` and
```libtool``` packages). Under Ubuntu they are found in ```build-essential```
package.
- You must have cutter framework installed (```cutter-test_framework```
package) in order to be able to run tests, but this is not required to build
the package.

Instructions to build:
- You need to run ./autogen.sh file (it will generate configure script).
  ```$ ./autogen.sh```
- On successful run of ./autogen.sh, you will have configure file. Run it.
  ```$ ./configure```
- Next step, is to run make. ```$ make```
- To install the binaries, using make install. ```$ make install```

Instructions for testing:
- To run tests you can run either the script provided in tests/run-test.sh:
  ```$ ./test/run-test.sh```
  or use make:
  ```$ make check```

