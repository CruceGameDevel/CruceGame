CruceGame Coding Standard
=========

The CruceGame coding standard is based on linux kernel coding standard, which
can be found here: https://www.kernel.org/doc/Documentation/CodingStyle.
Please read this coding standard and follow it as close as possible.

Indentation
-----

* The indentations are 4 characters.
* Every statement inside a block (a block is delimited by braces) should be
indented.
* Do not put multiple statements on the same line. Also, avoid placing multiple
assignments on the same line. The basic rule you should follow is common sense:
avoid tricky expressions.

Long lines
-----

The lines should have at most 80 characters. Statements longer should be:
* Broken into shorter, simpler statements.
* Splitted into more lines, with double indentation.
* Splitted into more lines, aligning similar elements.

Placing braces
-----

The braces should be placed at the end of the line, except for functions,
where the opening brace should be placed at the beginning of the next line.  
As this is a project for beginners, we expect you to be a beginner. So you
should use braces, even this is not necessary. This is just a piece of advice.
If you are confident about your skills, you may not use them all the time.

Spaces
-----

* Use a space after the most keywords, except for unary operators like
```sizeof``` .
* Do not add spaces arround (inside) parenthesized expressions. This example is
wrong: ```s = sizeof( variable )``` .
* When declaring a pointer variable of a function that return a pointer, the '*'
should be adjacent to the data name or function name, not to the type. When
specify a type (like in function declarations), it may be adjacent to the data
type. For example: ```int *a;``` ```int *foo(char*)```. 
* Use spaces around most of the binary and ternary operators, but not after unary
operators (or before, in case of postfix operators).
* Do not use spaces around structure member access operators, like ```->``` and
```.``` .  
* Do not leave trailing whitespaces at the end of the lines.

Naming
-----

* Constant names should be ```UPPER_CASE``` .
* Variable and function names should start with a ```lowercase``` letter and use
```camelCase``` .  
* User-defined type names should start with ```Capital``` letter. Note that
typedefs are aliases, not user defined types (the use of typedefs is
discouraged).  
* Global variables and functions should have long, descriptive names (they
should be avoided). Local variables should have short names. In general, the
larger the scope where an object is to be used, the more descriptive its name
should be.

Functions
-----

Functions should be as modular as possible. Every function should do one thing,
and do it well. Please document your functions and test them before making a
pull request. Keep your functions as short and simple as possible.

Commenting
-----

* Every file should have a comment header. It must include a description of its
content.
* Every function should have a documentation before the function implementation.
It must include the function prototype, a function description, parameters
and return value.
* Before declaration, every structure should have a documentation. Global
variables should have a documentation too.
* Keep the documentation as clear and precise as possible.
* For documentation, Doxygen should be used. Please read the following
instructions about documenting the code:
http://www.stack.nl/~dimitri/doxygen/manual/docblocks.html.


Here is an example of a function documentation:
```
/**
 * int foo(int a)
 *
 * The function description
 *
 * @param a an integer value
 * @return an integer value
 */
```
* The documentation before implementation should describe the function or data
type or object completely. If someone wants to use it, that is to be read, not
the implementation.  
* Feel free to add short comments in your code if you think they are needed (to
explain a certain expression for example). Make your code as easy to read
and understand as possible.

Miscellaneous
-----
* Do not leave dangling pointers. After you free a memory location, make
sure you set the pointer to NULL.

* Make sure there is an empty line at the end of each file.

