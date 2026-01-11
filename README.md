# Formula Parser

This library is intended to be used as part of a digital asset creation program (either as an internal element or as part of a plugin). It provides the ability for a user to use mathematical formulas to define a part, or the whole, of the asset's data points (such as notes in a MIDI file, vertex positions in a 3D model, bone position in an animation, etc). 

The functionality is provided in the Formula class. It can parse a given string containing a mathematical expression into a calculable in-memory AST, keep track of the variables available to that expression and their values, update those values, and calculate that expression using them. The goal is to be able to evaluate that expression for many different values as fast as possible.

## Features

**Expanded set of binary arithmetic operators**: Adds comparative operators. A comparative operator gives 1 if the comparison is true and 0 if it isn't.

**Access to an indexable numeric data structure through the data[] operator**: A Formula can use numbers from any indexable data structure (with constant dimensions) as operands using the data[] operator. It will read the value stored at the index(es) equal to the value(s) of the expression(s) contained within the brackets.     

**Configurable variable set**: The developer using this library can define which variables are

### Full operator list

data[]

sin(), cos(), tan(), asin(), acos(), atan(), sinh(), cosh(), tanh(), log(), ln(), sqrt(), ceil(), floor(), abs(), -()

=, !=, <, >, +, -, *, /, %, ^

sum(), mult()

## To Do
