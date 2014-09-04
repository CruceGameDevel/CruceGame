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
- You must have ncurses library installed on your machine. 
- You must have cutter framework installed (```cutter-test_framework```
package) in order to be able to run tests, but this is not required to build
the package.

Instructions to build:
- You need to run ./autogen.sh file (it will generate configure script).
  ```$ ./autogen.sh```
- On successful run of ./autogen.sh, you will have configure file. Run it.
  ```$ ./configure```
- Next step, is to run make. ```$ make```

Instructions for testing:
- To run tests you can run either the script provided in tests/run-test.sh:
  ```$ ./test/run-test.sh```
  or use make:
  ```$ make check```

Build options
------

The method described above will generate a release package (with no debug
information or debug options). In the development process and for debugging,
it is recommended to use the debug version. It will also use -O0 and -g flags
for gcc. To be able to generate it, simply pass --enable-debug="yes" parameter 
to the configure script:

```$ ./configure --enable-debug="yes"```

Configure will search for cutter, and if it is available, will build tests
too. If you don't want that, use:

```$ ./configure --with-cutter="no"```

There is also a "hidden" flag, BORDERS. If it is used, a border will be added
around curses windows, useful in designing the window layout. To use it, you
should specify it explicitly to configure:

```$ ./configure CFLAGS=-DBORDERS```

Documentation
------

We use Doxygen for documentation. There is a html version of the documentation
for the last release available on branch ```gh-pages```. There is a Doygen
configuration file, under docs/Doxyfile. To generate a new version of the
documentation, simply run:

```$ doxygen path-to-docs/Doxyfile```

This will generate 2 directories inside your current directory named ```html```
and ```latex```, containing 2 versions of the documentation.

