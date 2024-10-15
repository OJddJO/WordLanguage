# WordLanguage Documentation

## Introduction
WordLanguage is a programming language built to use the least amount of symbols possible. It is designed to be easy to read and write. The goal is to create a language that is easy to learn and use, while still being powerful enough to create complex programs.

## Syntax
The syntax of WordLanguage is simple and easy to understand. It is designed to be as close to natural language as possible.

Here is an example:
```
str name assign input "Enter your name: "
print "Hello" str name 
```

- create a string variable `name` and assign it the input from the user with the prompt `Enter your name: `
- print Hello and the string value in `name`

> [!NOTE]
> One line of code can only contain one statement. You cannot perform multiple tasks on the same line.
> For example, the following code is invalid:
> ```
> str name assign input "Enter your name: " print "Hello" str name
> ```

## Documentation
The documentation is divided into several sections:
- [Operations](operations.md)
- [Variables](variables.md)
- [Constructed Types](constructed_types.md)
    - [Arrays](constructed_types.md/#arrays-type-safe)
    - [Lists](constructed_types.md/#lists-not-type-safe)
- [I/O Expressions](io_expressions.md)
    - [Output](io_expressions.md/#output)
    - [Input](io_expressions.md/#input)
- [Boolean Expressions](boolean_expressions.md)
    - [Comparison Operators](boolean_expressions.md/#comparison)
    - [Logical Operators](boolean_expressions.md/#logic)
- [Control Flow](control_flow.md)
    - [Conditionnal Statements](control_flow.md/#conditional-statements)
    - [Loops](control_flow.md/#loops)
- [Functions](functions.md)