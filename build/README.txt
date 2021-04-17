BASICS
================================================================================

to run an example program, visit the tests folder

they are example programs that demonstrate some of the syntax and semantics

the simplest program possible is the following:

fn entry() {
    return 0;
}

to run a program save a program as <name>.td

in the build/ folder there is tadashi.exe, open cmd.exe from the start menu and
navigate to the build/ folder using cd

from there, typing "tadashi -file <path to your program>" in the cmd window
will execute the program and print all relevant output data

adding "-ast" to the command line arguments will output a file called "ast.txt"
which is a textual representation of the abstract syntax tree. It is lisp like.


SYNTAX
================================================================================

much of the syntax is C-like, but there are some differences. The example programs
should demonstrate most of the syntax features, but there are examples of each
below.

functions:

fn test(arg1, arg2, arg3){
    return arg1 + arg2;
}


variables:

a = 10;
b = a + 2;


arrays/strings:

string = "test";
array = [10];
string[1] = 'a';


if statements:

x = 10;
if x > 2 {
    x = 19;
}else {
    x = 20;
}

for loops:

x = 1;
for i = 0; i < 10; i = i + 1 {
    x = x*x;
}

while loops

x = 1;
y = 0;
while x {
    y = y + 2;
}

SOURCE CODE
================================================================================

the source code is included and fully documented.

main() is in tadashi.cc which is where everything is setup and compiled as well as
where the virtual machine is located.

parser.h contains the structures used to describe the AST as well as the associated
enums. parser.cc contains all the functions necessary to take a tokenised source file
and convert it into the tree structure

lexer.h contains the structures used to describe tokens in a source file. lexer.cc
turns a source file (array of characters) into tokens which are a pointer + length.
these tokens usually ignore most whitespace.

memory.h/memory.cc are helper files that handle memory allocations tadashi makes
during compilation and execution.

vm.h contains the structures used in the virtual machine, which all the related functions
are in tadashi.cc

compiler.h contains the structures for scopes, compiled functions, compiled variables,
and the compiler itself. compiler.cc contains the functions used to emit the bytecode
necessary to execute the original source file as intended.



