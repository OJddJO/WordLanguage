# Math
| Expression | Description |
|---|---|
| `a plus b` | Add `a` and `b` |
| `a minus b` | Subtract `b` from `a` |
| `a times b` | Multiply `a` and `b` |
| `a div b` | Divide `a` by `b` |
| `a mod b` | Remainder of `a` divided by `b` |
| `a ediv b` | Euclidean division of `a` by `b` |
| `a power b` | `a` raised to the power of `b` |
| `a root b` | `a`th root of `b` |
| `a factorial` | Factorial of `a` |
| `abs a` | Absolute value of `a` |
| `a .plus b times c` | `a + b` times `c` (priority) |

# Variables
| Expression | Description |
|---|---|
| `int var assign a` | Assign `a` to a new integer variable `var` |
| `int var assign a plus b` | Assign `a + b` to a new integer variable `var` |
| `float var assign a` | Assign `a` to a float new variable `var` |
| `str var assign "Hello World"` | Assign `Hello World` to a new string variable `var` |
| `bool var assign true` | Assign `true` to a new boolean variable `var` |
| `var assign a` | Assign the value of `a` to the variable `var` without redifining variable |
| `delete var` | Delete the variable `var` |

# Arrays (Type safe)
| Expression | Description |
|---|---|
| `int array var size 10` | Create a new integer array variable `var` of size 10 |
| `var get index a` | Get the value at index `a` from the array variable `var` |
| `var set index a assign b` | Set the value at index `a` in the array variable `var` to `b` |
| `var length` | Get the length of the array variable `var` |
| `delete var` | Delete the array variable `var` |

# Lists (Not type safe)
| Expression | Description |
|---|---|
| `list var` | Create a new list variable `var` (not type safe, can contain any type of data) |
| `var get index a` | Get the value at index `a` from the list variable `var` |
| `var set index a assign b` | Set the value at index `a` in the list variable `var` to `b` |
| `var append a` | Append `a` to the list variable `var` (not type safe) |
| `var remove a` | Remove the value at index `a` from the list variable `var` |
| `var length` | Get the length of the list variable `var` |
| `delete var` | Delete the list variable `var` |

# Output
| Expression | Description |
|---|---|
| `print "Hello World"` | Print `Hello World` in the terminal |
| `print var` | Print the value of the variable `var` in the terminal |
| `print int var sep " " end "\n"` | Print the value of the variable `var` with a space separator and a newline at the end |

# Input
| Expression | Description |
|---|---|
| `int var input` | Take an integer input and assign it to the variable `var` |
| `str var input` | Take a string input and assign it to the variable `var` |
| `str var input ask "Enter your name"` | Take a string input with the prompt `Enter your name` and assign it to the variable `var` |

# Logic
| Expression | Description |
|---|---|
| `a and b` | `a` and `b` |
| `a or b` | `a` or `b` |
| `not a` | not `a` |
| `a .or b and c` | (`a` or `b`) and `c` (priority)|

# Comparison
| Expression | Description |
|---|---|
| `a equal b` | Returns true if `a` is equal to `b`, else false |
| `not a equal b` | Returns true if `not a` is equal to `b`, else false |
| `a greater b` | Returns true if `a` is greater than `b`, else false |
| `a less b` | Returns true if `a` is less than `b`, else false |
| `a gEqual b` | Returns true if `a` is greater than or equal to `b`, else false |
| `a lEqual b` | Returns true if `a` is less than or equal to `b`, else false |
| `.not a ..equal b and b equal c` | Returns true if (`a` is not equal to `b`) and `b` is equal to `c`, else false (priority) |

# Control Flow
| Expression | Description |
|---|---|
| `if a equal b do` | If `a` is equal to `b`, do |
| `if not a gEqual b and c equals a plus b do` | If `a` is not greater than or equal to `b` and `c` is equal to `a+b`, do |
| `elif a equal b do` | Else if `a` is equal to `b`, do |
| `else` | Else |
| `endif` | End if |

# Loops
| Expression | Description |
|---|---|
| `for i in range 1 to 10 do` | Loop from 1 to 10 |
| `for i in range 1 to 10 step 2 do` | Loop from 1 to 10 with a step of 2 |
| `for i in range 10 to 1 step -1 do` | Loop from 10 to 1 with a step of -1 |
| `while <boolean expression> do` | While the `boolean expression` is verified (example: `a less b`) , do |
| `break` | Break loop |
| `endloop` | End loop |

# Functions
| Expression | Description |
|---|---|
| `def void funcName ` | Define a function `funcName` that returns nothing |
| `def int funcName with bool a with int b` | Define a function `funcName` with arguments `a` (boolean) and `b` (integer) that returns an integer |
| `return a` | Return `a` from the function |
| `return a plus b` | Return `a + b` from the function |
| `enddef` | End function |

